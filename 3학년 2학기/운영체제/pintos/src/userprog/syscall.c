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
// 파일을 열고 고유한 File Descriptor(fd) 번호를 반환하는 시스템 콜
int
open (const char *file)
{
  is_valid_address (file);
  if (*file == '\0')
    return -1; // 빈 파일 이름은 에러 처리

  struct file *f = filesys_open (file);
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

  file_close (cur->fd_table[fd]);
  cur->fd_table[fd] = NULL;
}

// 파일 또는 키보드에서 데이터를 읽는 시스템 콜
int
read (int fd, void *buffer, unsigned size)
{
  is_valid_address (buffer);
  if (size > 0)
    is_valid_address ((const char *) buffer + size - 1);

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

  return file_read (cur->fd_table[fd], buffer, size);
}

// 파일 또는 화면에 데이터를 쓰는 시스템 콜
int
write (int fd, const void *buffer, unsigned size)
{
  is_valid_address (buffer);
  if (size > 0)
    is_valid_address ((const char *) buffer + size - 1);

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

  return file_write (cur->fd_table[fd], buffer, size);
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

  return file_length (cur->fd_table[fd]);
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

  file_seek (cur->fd_table[fd], position);
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

  return file_tell (cur->fd_table[fd]);
}


// pintos가 맨 처음에 컴퓨터를 켜면, 이 함수를 실행 -> 0x30번 인터럽트가 발생되면, syscall_handler()를 호출하도록 설정
void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}


// 시스템콜 종합 상담원.
static void
syscall_handler (struct intr_frame *f) 
{
  // 1. 유저 스택 포인터(f->esp) 자체의 유효성 검사
  is_valid_address (f->esp);

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
      is_valid_address ((int *) f->esp + 1);
      exit (*((int *) f->esp + 1));
      break;

    // 여기를 수정해야함
    case SYS_OPEN:
      is_valid_address ((int *) f->esp + 1);
      f->eax = open (*((char **) f->esp + 1));
      break;

    case SYS_CLOSE:
      is_valid_address ((int *) f->esp + 1);
      close (*((int *) f->esp + 1));
      break;

    case SYS_READ:
      is_valid_address ((int *) f->esp + 1);
      is_valid_address ((int *) f->esp + 2);
      is_valid_address ((int *) f->esp + 3);
      f->eax = read (*((int *) f->esp + 1), 
                     *((void **) f->esp + 2), 
                     *((unsigned *) f->esp + 3));
      break;

    case SYS_WRITE:
      is_valid_address ((int *) f->esp + 1);
      is_valid_address ((int *) f->esp + 2);
      is_valid_address ((int *) f->esp + 3);
      f->eax = write (*((int *) f->esp + 1), 
                      *((void **) f->esp + 2), 
                      *((unsigned *) f->esp + 3));
      break;

    case SYS_FILESIZE:
      is_valid_address ((int *) f->esp + 1);
      f->eax = filesize (*((int *) f->esp + 1));
      break;

    case SYS_SEEK:
      is_valid_address ((int *) f->esp + 1);
      is_valid_address ((int *) f->esp + 2);
      seek (*((int *) f->esp + 1), *((unsigned *) f->esp + 2));
      break;

    case SYS_TELL:
      is_valid_address ((int *) f->esp + 1);
      f->eax = tell (*((int *) f->esp + 1));
      break;

    default:
      // 아직 구현되지 않은 시스템 콜인 경우 종료
      exit (-1);
      break;
    }
}
