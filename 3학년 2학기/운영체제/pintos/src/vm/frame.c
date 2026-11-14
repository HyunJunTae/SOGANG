#include "vm/frame.h"
#include <stdio.h>
#include "threads/malloc.h"
#include "threads/palloc.h"
#include "threads/synch.h"
#include "threads/thread.h"
#include "threads/vaddr.h"
#include "userprog/pagedir.h"
#include "vm/page.h"
#include "vm/swap.h"
// ch3 구현 - MMAP 페이지 교체 시 파일 기록(write-back)을 위한 헤더 포함
#include "filesys/file.h"
#include "userprog/syscall.h"

// ch3 구현 - 전역 물리 프레임 관리 리스트, 동기화 락 및 Clock 알고리즘 핸드 포인터
static struct list global_frame_list;
static struct lock frame_table_lock;
static struct list_elem *clock_hand_ptr;

static void *evict_and_alloc_frame (enum palloc_flags flags);

// ch3 구현 - 프레임 테이블 및 락 초기화
void
frame_table_init (void)
{
  list_init (&global_frame_list);
  lock_init (&frame_table_lock);
  clock_hand_ptr = NULL;
}

// ch3 구현 - Clock 알고리즘 포인터를 다음 프레임 노드로 순환 이동시키는 헬퍼 함수
static struct list_elem *
advance_clock_hand (void)
{
  if (list_empty (&global_frame_list))
    return NULL;

  if (clock_hand_ptr == NULL || clock_hand_ptr == list_end (&global_frame_list))
    clock_hand_ptr = list_begin (&global_frame_list);
  else
    {
      clock_hand_ptr = list_next (clock_hand_ptr);
      if (clock_hand_ptr == list_end (&global_frame_list))
        clock_hand_ptr = list_begin (&global_frame_list);
    }

  return clock_hand_ptr;
}

// ch3 구현 - User Pool에서 물리 프레임 할당 (부족 시 Second Chance 알고리즘으로 기존 프레임 교체)
struct frame_slot *
allocate_user_frame (enum palloc_flags flags)
{
  ASSERT ((flags & PAL_USER) != 0);

  lock_acquire (&frame_table_lock);

  void *kpage_addr = palloc_get_page (flags);
  while (kpage_addr == NULL)
    {
      kpage_addr = evict_and_alloc_frame (flags);
    }

  struct frame_slot *f_slot = malloc (sizeof (struct frame_slot));
  if (f_slot == NULL)
    {
      palloc_free_page (kpage_addr);
      lock_release (&frame_table_lock);
      return NULL;
    }

  f_slot->kernel_vaddr = kpage_addr;
  f_slot->spte = NULL;
  f_slot->owner_thread = thread_current ();

  list_push_back (&global_frame_list, &f_slot->clock_elem);

  lock_release (&frame_table_lock);
  return f_slot;
}

// ch3 구현 - 커널 가상 주소(kernel_vaddr)에 해당하는 물리 프레임을 검색하여 해제
void
release_user_frame (void *kernel_vaddr)
{
  lock_acquire (&frame_table_lock);

  struct list_elem *it;
  for (it = list_begin (&global_frame_list);
       it != list_end (&global_frame_list);
       it = list_next (it))
    {
      struct frame_slot *f_slot = list_entry (it, struct frame_slot, clock_elem);
      if (f_slot->kernel_vaddr == kernel_vaddr)
        {
          __release_user_frame (f_slot);
          break;
        }
    }

  lock_release (&frame_table_lock);
}

// ch3 구현 - 프레임 리스트에서 슬롯을 제거하고 하드웨어 매핑 및 물리 페이지 반환 (락 보유 상태에서 호출)
void
__release_user_frame (struct frame_slot *f_slot)
{
  if (clock_hand_ptr == &f_slot->clock_elem)
    {
      clock_hand_ptr = list_next (clock_hand_ptr);
      if (clock_hand_ptr == list_end (&global_frame_list))
        clock_hand_ptr = NULL;
    }

  list_remove (&f_slot->clock_elem);

  if (f_slot->spte != NULL && f_slot->owner_thread != NULL && f_slot->owner_thread->pagedir != NULL)
    {
      pagedir_clear_page (f_slot->owner_thread->pagedir, f_slot->spte->user_vaddr);
      f_slot->spte->in_memory = false;
    }

  palloc_free_page (f_slot->kernel_vaddr);
  free (f_slot);
}

// ch3 구현 - Pseudo-LRU (Second Chance / Clock) 페이지 교체 알고리즘을 통해 희생 프레임을 스왑 아웃하고 새 페이지 할당
static void *
evict_and_alloc_frame (enum palloc_flags flags)
{
  size_t total_frames = list_size (&global_frame_list);
  size_t scanned = 0;

  while (scanned <= total_frames * 2)
    {
      struct list_elem *cand_elem = advance_clock_hand ();
      if (cand_elem == NULL)
        break;

      struct frame_slot *victim = list_entry (cand_elem, struct frame_slot, clock_elem);
      scanned++;

      // 커널 I/O 중이거나 아직 연결 중인 프레임은 교체 대상에서 제외
      if (victim->spte == NULL || victim->spte->is_pinned)
        continue;

      struct thread *owner = victim->owner_thread;
      void *uaddr = victim->spte->user_vaddr;

      // Accessed 비트가 1이면 0으로 내리고 한 번의 기회(Second Chance) 부여
      if (pagedir_is_accessed (owner->pagedir, uaddr))
        {
          pagedir_set_accessed (owner->pagedir, uaddr, false);
          continue;
        }

      // Victim 선정 완료: Dirty 상태이거나 익명(스택/스왑) 페이지면 스왑 디스크 또는 매핑 파일에 기록
      bool is_dirty = pagedir_is_dirty (owner->pagedir, uaddr);
      if (victim->spte->page_type == PAGE_SOURCE_MMAP)
        {
          if (is_dirty)
            {
              bool held = lock_held_by_current_thread (&filesys_lock);
              if (!held)
                lock_acquire (&filesys_lock);
              file_write_at (victim->spte->mapped_file, victim->kernel_vaddr,
                             victim->spte->read_bytes, victim->spte->file_offset);
              if (!held)
                lock_release (&filesys_lock);
            }
        }
      else if (victim->spte->page_type == PAGE_SOURCE_ANON || is_dirty)
        {
          victim->spte->page_type = PAGE_SOURCE_ANON;
          victim->spte->swap_index = swap_write_page (victim->kernel_vaddr);
        }

      victim->spte->in_memory = false;
      __release_user_frame (victim);

      return palloc_get_page (flags);
    }

  return NULL;
}

// ch3 구현 - 시스템 콜 I/O 버퍼에 포함된 가상 페이지들이 도중에 스왑 아웃되지 않도록 고정(Pin)
void
pin_user_buffer (const void *buffer, unsigned size)
{
  if (size == 0)
    return;

  uint8_t *start_page = pg_round_down (buffer);
  uint8_t *end_page = pg_round_down ((const uint8_t *) buffer + size - 1);
  uint8_t *cur_page;

  for (cur_page = start_page; cur_page <= end_page; cur_page += PGSIZE)
    {
      struct spt_entry *spte = spt_find_page (cur_page);
      if (spte != NULL)
        {
          spte->is_pinned = true;
          if (!spte->in_memory)
            handle_mm_fault (spte);
        }
    }
}

// ch3 구현 - 시스템 콜 I/O 완료 후 버퍼 페이지들의 고정(Pin) 해제
void
unpin_user_buffer (const void *buffer, unsigned size)
{
  if (size == 0)
    return;

  uint8_t *start_page = pg_round_down (buffer);
  uint8_t *end_page = pg_round_down ((const uint8_t *) buffer + size - 1);
  uint8_t *cur_page;

  for (cur_page = start_page; cur_page <= end_page; cur_page += PGSIZE)
    {
      struct spt_entry *spte = spt_find_page (cur_page);
      if (spte != NULL)
        spte->is_pinned = false;
    }
}
