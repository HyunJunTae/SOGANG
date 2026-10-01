#ifndef USERPROG_SYSCALL_H
#define USERPROG_SYSCALL_H

#include "threads/synch.h"

// synchronization 여기 구현 - 파일 시스템 접근 동기화를 위한 전역 락
extern struct lock filesys_lock;

void syscall_init (void);
void is_valid_address (const void *addr);
void exit (int status);
// 여기를 수정해야함
bool create (const char *file, unsigned initial_size);
bool remove (const char *file);
int open (const char *file);
void close (int fd);
int read (int fd, void *buffer, unsigned size);
int write (int fd, const void *buffer, unsigned size);
int filesize (int fd);
void seek (int fd, unsigned position);
unsigned tell (int fd);

#endif /* userprog/syscall.h */
