#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "devices/shutdown.h"
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

    default:
      // 아직 구현되지 않은 시스템 콜인 경우 종료
      exit (-1);
      break;
    }
}
