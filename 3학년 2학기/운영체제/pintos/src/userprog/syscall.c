#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "devices/shutdown.h"
#include "devices/input.h"
#include "filesys/filesys.h"
#include "filesys/file.h"
#include "lib/kernel/console.h"
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/vaddr.h"
#include "userprog/pagedir.h"

static void syscall_handler (struct intr_frame *);
void halt (void);

// synchronization 여기 구현 - 전역 락 실제 선언 (메모리 할당)
struct lock filesys_lock;

// 주소가 유효하지 않으면 즉시 프로세스를 안전하게 종료(-1)
void
is_valid_address (const void *addr) 
{
  // 1. NULL 포인터이거나
  // 2. 커널 가상 주소 영역(PHYS_BASE 이상)이거나
  // 3. 아직 물리 페이지에 매핑되지 않은 주소인 경우
  if (addr == NULL || !is_user_vaddr (addr) || pagedir_get_page (thread_current ()->pagedir, addr) == NULL)
    {
      exit (-1); // 즉시 강제 종료
    }
}

// [Issue 3 해결] 버퍼가 여러 페이지에 걸쳐 있을 경우를 대비해 페이지 단위로 메모리 유효성 검사 (최적화)
void
check_valid_buffer (void *buffer, unsigned size)
{
  if (size == 0) return;
  
  char *ptr = (char *) buffer;
  is_valid_address (ptr); // 시작 주소 검사
  
  // 버퍼가 걸쳐있는 페이지들을 순회하며 각 페이지의 시작점만 검사
  void *curr_page = pg_round_down (ptr);
  void *end_page = pg_round_down (ptr + size - 1);
  
  for (curr_page += PGSIZE; curr_page <= end_page; curr_page += PGSIZE)
    {
      is_valid_address (curr_page);
    }
}

// [Issue 4 해결] 문자열 포인터가 페이지 경계를 넘을 경우를 대비해 널 문자(\0)까지 전체 검사 (최적화)
void
check_valid_string (const void *str)
{
  is_valid_address (str); // 첫 글자 주소 검사
  char *ptr = (char *) str;
  while (*ptr != '\0')
    {
      ptr++;
      // 포인터가 페이지의 시작점(오프셋 0)에 도달했을 때만 새로운 페이지 유효성 검사
      if (pg_ofs (ptr) == 0)
        is_valid_address (ptr);
    }
}

// 프로세스를 종료시키는 시스템 콜 함수
void
exit (int status)
{
  struct thread *cur = thread_current ();
  cur->exit_status = status;
  printf ("%s: exit(%d)\n", cur->name, status);
  thread_exit ();
}

// 핀토스를 종료시키는 시스템 콜 함수
void
halt (void)
{
  shutdown_power_off ();
}

// 여기를 수정해야함
// synchronization 여기 구현 - 파일 생성
bool
create (const char *file, unsigned initial_size)
{
  check_valid_string (file); // [Issue 4 해결] 문자열 전체 검증으로 변경

  // synchronization 여기 구현 - 임계 구역 락 획득
  lock_acquire (&filesys_lock);
  bool success = filesys_create (file, initial_size);
  lock_release (&filesys_lock);

  return success;
}

// synchronization 여기 구현 - 파일 삭제
bool
remove (const char *file)
{
  check_valid_string (file); // [Issue 4 해결] 문자열 전체 검증으로 변경

  // synchronization 여기 구현 - 임계 구역 락 획득
  lock_acquire (&filesys_lock);
  bool success = filesys_remove (file);
  lock_release (&filesys_lock);

  return success;
}

// 파일을 열고 고유한 File Descriptor(fd) 번호를 반환하는 시스템 콜
int
open (const char *file)
{
  check_valid_string (file); // [Issue 4 해결] 문자열 전체 검증으로 변경
  if (*file == '\0')
    return -1; // 빈 파일 이름은 에러 처리

  // synchronization 여기 구현 - 임계 구역 락 획득
  lock_acquire (&filesys_lock);
  struct file *f = filesys_open (file);
  lock_release (&filesys_lock);
  
  if (f == NULL)
    return -1; // 파일이 없거나 오픈 실패

  struct thread *cur = thread_current ();
  int fd = -1;
  int i;

  // 빈 fd 슬롯 탐색 (next_fd부터 순회)
  for (i = cur->next_fd; i < 128; i++)
    {
      if (cur->fd_table[i] == NULL)
        {
          fd = i;
          break;
        }
    }
  if (fd == -1)
    {
      for (i = 2; i < cur->next_fd; i++)
        {
          if (cur->fd_table[i] == NULL)
            {
              fd = i;
              break;
            }
        }
    }

  // fd 테이블(최대 128개)이 꽉 찬 경우
  if (fd == -1)
    {
      file_close (f);
      return -1;
    }

  cur->fd_table[fd] = f;
  cur->next_fd = fd + 1;
  if (cur->next_fd >= 128)
    cur->next_fd = 2;

  return fd;
}

// 열려 있는 파일을 닫는 시스템 콜
void
close (int fd)
{
  // 0(STDIN), 1(STDOUT) 또는 범위를 벗어난 fd는 무시
  if (fd < 2 || fd >= 128)
    return;

  struct thread *cur = thread_current ();
  if (cur->fd_table[fd] == NULL)
    return; // 이미 닫혔거나 열리지 않은 fd

  // synchronization 여기 구현 - 임계 구역 락 획득
  lock_acquire (&filesys_lock);
  file_close (cur->fd_table[fd]);
  lock_release (&filesys_lock);
  
  cur->fd_table[fd] = NULL;
}

// 파일 또는 키보드에서 데이터를 읽는 시스템 콜
int
read (int fd, void *buffer, unsigned size)
{
  check_valid_buffer (buffer, size); // [Issue 3 해결] 버퍼 전체 페이지 검증으로 변경

  if (size == 0)
    return 0; // 읽을 크기가 0이면 0 반환

  if (fd == 1)
    return -1; // STDOUT(화면)에서는 읽을 수 없음

  // STDIN(키보드 입력) 처리
  if (fd == 0)
    {
      uint8_t *buf = (uint8_t *) buffer;
      unsigned i;
      for (i = 0; i < size; i++)
        {
          buf[i] = input_getc ();
        }
      return size;
    }

  // 일반 파일 읽기 처리
  if (fd < 0 || fd >= 128)
    return -1;

  struct thread *cur = thread_current ();
  if (cur->fd_table[fd] == NULL)
    return -1;

  // synchronization 여기 구현 - 임계 구역 락 획득
  lock_acquire (&filesys_lock);
  int bytes_read = file_read (cur->fd_table[fd], buffer, size);
  lock_release (&filesys_lock);
  
  return bytes_read;
}

// 파일 또는 화면에 데이터를 쓰는 시스템 콜
int
write (int fd, const void *buffer, unsigned size)
{
  check_valid_buffer ((void *) buffer, size); // [Issue 3 해결] 버퍼 전체 페이지 검증으로 변경

  if (size == 0)
    return 0; // 쓸 크기가 0이면 0 반환

  if (fd == 0)
    return -1; // STDIN(키보드)에는 쓸 수 없음

  // STDOUT(화면 콘솔 출력) 처리
  if (fd == 1)
    {
      putbuf (buffer, size);
      return size;
    }

  // 일반 파일 쓰기 처리
  if (fd < 0 || fd >= 128)
    return -1;

  struct thread *cur = thread_current ();
  if (cur->fd_table[fd] == NULL)
    return -1;

  // synchronization 여기 구현 - 임계 구역 락 획득
  lock_acquire (&filesys_lock);
  int bytes_written = file_write (cur->fd_table[fd], buffer, size);
  lock_release (&filesys_lock);
  
  return bytes_written;
}

// 파일의 크기(바이트 수)를 반환하는 시스템 콜
int
filesize (int fd)
{
  if (fd < 2 || fd >= 128)
    return -1;

  struct thread *cur = thread_current ();
  if (cur->fd_table[fd] == NULL)
    return -1;

  // synchronization 여기 구현 - 임계 구역 락 획득
  lock_acquire (&filesys_lock);
  int len = file_length (cur->fd_table[fd]);
  lock_release (&filesys_lock);

  return len;
}

// 파일의 읽기/쓰기 위치(offset)를 변경하는 시스템 콜
void
seek (int fd, unsigned position)
{
  if (fd < 2 || fd >= 128)
    return;

  struct thread *cur = thread_current ();
  if (cur->fd_table[fd] == NULL)
    return;

  // synchronization 여기 구현 - 임계 구역 락 획득
  lock_acquire (&filesys_lock);
  file_seek (cur->fd_table[fd], position);
  lock_release (&filesys_lock);
}

// 파일의 현재 읽기/쓰기 위치(offset)를 반환하는 시스템 콜
unsigned
tell (int fd)
{
  if (fd < 2 || fd >= 128)
    return 0;

  struct thread *cur = thread_current ();
  if (cur->fd_table[fd] == NULL)
    return 0;

  // synchronization 여기 구현 - 임계 구역 락 획득
  lock_acquire (&filesys_lock);
  unsigned pos = file_tell (cur->fd_table[fd]);
  lock_release (&filesys_lock);
  
  return pos;
}


// pintos가 맨 처음에 컴퓨터를 켜면, 이 함수를 실행 -> 0x30번 인터럽트가 발생되면, syscall_handler()를 호출하도록 설정
// 시스템콜 여기 구현 - exec, wait, 서강대 추가 시스템 콜 커널 내부 로직 함수
tid_t
exec (const char *cmd_line)
{
  check_valid_string (cmd_line); // [Issue 4 해결] 문자열 전체 검증으로 변경
  return process_execute (cmd_line);
}

int
wait (tid_t pid)
{
  return process_wait (pid);
}

int
fibonacci (int n)
{
  if (n < 0) return -1;
  if (n == 0) return 0;
  if (n == 1 || n == 2) return 1;
  
  int a = 1, b = 1, c = 2;
  int i;
  for (i = 3; i <= n; i++)
    {
      c = a + b;
      a = b;
      b = c;
    }
  return c;
}

int
max_of_four_int (int a, int b, int c, int d)
{
  int max1 = a > b ? a : b;
  int max2 = c > d ? c : d;
  return max1 > max2 ? max1 : max2;
}

void
syscall_init (void) 
{
  // synchronization 여기 구현 - 파일 시스템 접근 동기화를 위한 전역 락 초기화
  lock_init (&filesys_lock);
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}


// 시스템콜 종합 상담원.
static void
syscall_handler (struct intr_frame *f) 
{
  // 1. 유저 스택 포인터(f->esp) 자체의 유효성 검사
  check_valid_buffer (f->esp, 4);

  // 2. 시스템 콜 번호 읽어오기
  int syscall_nr = *(int *) f->esp;

  // 3. 시스템 콜 번호에 따른 분기 처리
  switch (syscall_nr)
    {
    case SYS_HALT:
      halt ();
      break;

    case SYS_EXIT:
      // exit 인자(status)가 위치한 스택 주소 유효성 검사 (esp + 4바이트)
      check_valid_buffer ((int *) f->esp + 1, 4);
      exit (*((int *) f->esp + 1));
      break;

    // 시스템콜 여기 구현 - exec, wait, 서강대 추가 시스템 콜 인터럽트 분기 및 유효성 검증
    case SYS_EXEC:
      check_valid_buffer ((int *) f->esp + 1, 4);
      f->eax = exec (*((char **) f->esp + 1));
      break;

    case SYS_WAIT:
      check_valid_buffer ((int *) f->esp + 1, 4);
      f->eax = wait (*((tid_t *) f->esp + 1));
      break;

    case SYS_FIBONACCI:
      check_valid_buffer ((int *) f->esp + 1, 4);
      f->eax = fibonacci (*((int *) f->esp + 1));
      break;

    case SYS_MAX_OF_FOUR_INT:
      check_valid_buffer ((int *) f->esp + 1, 4);
      check_valid_buffer ((int *) f->esp + 2, 4);
      check_valid_buffer ((int *) f->esp + 3, 4);
      check_valid_buffer ((int *) f->esp + 4, 4);
      f->eax = max_of_four_int (*((int *) f->esp + 1),
                                *((int *) f->esp + 2),
                                *((int *) f->esp + 3),
                                *((int *) f->esp + 4));
      break;

    // 여기를 수정해야함
    // synchronization 여기 구현 - 핸들러 분기
    case SYS_CREATE:
      check_valid_buffer ((int *) f->esp + 1, 4);
      check_valid_buffer ((int *) f->esp + 2, 4);
      f->eax = create (*((char **) f->esp + 1), *((unsigned *) f->esp + 2));
      break;

    case SYS_REMOVE:
      check_valid_buffer ((int *) f->esp + 1, 4);
      f->eax = remove (*((char **) f->esp + 1));
      break;

    case SYS_OPEN:
      check_valid_buffer ((int *) f->esp + 1, 4);
      f->eax = open (*((char **) f->esp + 1));
      break;

    case SYS_CLOSE:
      check_valid_buffer ((int *) f->esp + 1, 4);
      close (*((int *) f->esp + 1));
      break;

    case SYS_READ:
      check_valid_buffer ((int *) f->esp + 1, 4);
      check_valid_buffer ((int *) f->esp + 2, 4);
      check_valid_buffer ((int *) f->esp + 3, 4);
      f->eax = read (*((int *) f->esp + 1), 
                     *((void **) f->esp + 2), 
                     *((unsigned *) f->esp + 3));
      break;

    case SYS_WRITE:
      check_valid_buffer ((int *) f->esp + 1, 4);
      check_valid_buffer ((int *) f->esp + 2, 4);
      check_valid_buffer ((int *) f->esp + 3, 4);
      f->eax = write (*((int *) f->esp + 1), 
                      *((void **) f->esp + 2), 
                      *((unsigned *) f->esp + 3));
      break;

    case SYS_FILESIZE:
      check_valid_buffer ((int *) f->esp + 1, 4);
      f->eax = filesize (*((int *) f->esp + 1));
      break;

    case SYS_SEEK:
      check_valid_buffer ((int *) f->esp + 1, 4);
      check_valid_buffer ((int *) f->esp + 2, 4);
      seek (*((int *) f->esp + 1), *((unsigned *) f->esp + 2));
      break;

    case SYS_TELL:
      check_valid_buffer ((int *) f->esp + 1, 4);
      f->eax = tell (*((int *) f->esp + 1));
      break;

    default:
      // 아직 구현되지 않은 시스템 콜인 경우 종료
      exit (-1);
      break;
    }
}
