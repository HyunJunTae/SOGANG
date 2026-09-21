#ifndef USERPROG_SYSCALL_H
#define USERPROG_SYSCALL_H

void syscall_init (void);
void is_valid_address (const void *addr);
void exit (int status);
// 여기를 수정해야함
int open (const char *file);
void close (int fd);
int read (int fd, void *buffer, unsigned size);
int write (int fd, const void *buffer, unsigned size);
int filesize (int fd);
void seek (int fd, unsigned position);
unsigned tell (int fd);

#endif /* userprog/syscall.h */
