#ifndef VM_FRAME_H
#define VM_FRAME_H

// ch3 구현 - 물리 프레임 테이블(Frame Table) 및 페이지 교체 알고리즘 헤더 정의
#include <list.h>
#include "threads/palloc.h"
#include "threads/synch.h"
#include "threads/thread.h"
#include "vm/page.h"

// ch3 구현 - 물리 메모리(User Pool)의 각 4KB 프레임을 추적하는 구조체
struct frame_slot
  {
    void *kernel_vaddr;         /* 물리 프레임과 매핑된 커널 가상 주소 */
    struct spt_entry *spte;     /* 이 프레임을 사용 중인 가상 페이지의 보조 엔트리 */
    struct thread *owner_thread;/* 이 프레임을 소유한 프로세스(스레드) */
    struct list_elem clock_elem;/* 전역 프레임 리스트(Clock 순환 큐) 연결 원소 */
  };

// ch3 구현 - 전역 프레임/스왑/페이지폴트 동기화 락 선언
extern struct lock vm_global_lock;

// ch3 구현 - 프레임 관리 초기화, 할당, 해제 및 고정(Pinning) 함수 선언
void frame_table_init (void);
struct frame_slot *allocate_user_frame (enum palloc_flags flags);
void release_user_frame (void *kernel_vaddr);
void __release_user_frame (struct frame_slot *f_slot);

void pin_user_buffer (const void *buffer, unsigned size);
void unpin_user_buffer (const void *buffer, unsigned size);

#endif /* vm/frame.h */
