#ifndef USERPROG_PROCESS_H
#define USERPROG_PROCESS_H

#include "threads/thread.h"

tid_t process_execute (const char *file_name);
int process_wait (tid_t);
void process_exit (void);
void process_activate (void);

// ch3 구현 - 하드웨어 페이지 테이블 매핑 및 스택 동적 확장 함수 선언
bool install_user_page (void *upage, void *kpage, bool writable);
bool verify_stack_growth (const void *fault_addr, void *esp);
bool expand_user_stack (void *fault_addr);

#endif /* userprog/process.h */
