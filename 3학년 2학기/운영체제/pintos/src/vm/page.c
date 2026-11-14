#include "vm/page.h"
#include <stdio.h>
#include <string.h>
#include "filesys/file.h"
#include "threads/malloc.h"
#include "threads/palloc.h"
#include "threads/thread.h"
#include "threads/vaddr.h"
#include "userprog/pagedir.h"
#include "userprog/process.h"
#include "userprog/syscall.h"
#include "vm/frame.h"
#include "vm/swap.h"

// ch3 구현 - 보조 페이지 해시 테이블의 해시값 계산 함수 (가상 페이지 주소 기반)
static unsigned
spt_hash_compute (const struct hash_elem *elem, void *aux UNUSED)
{
  const struct spt_entry *spte = hash_entry (elem, struct spt_entry, spt_elem);
  return hash_int ((int) spte->user_vaddr);
}

// ch3 구현 - 보조 페이지 해시 테이블의 키(user_vaddr) 대소 비교 함수
static bool
spt_addr_less (const struct hash_elem *elem_a,
               const struct hash_elem *elem_b,
               void *aux UNUSED)
{
  const struct spt_entry *spte_a = hash_entry (elem_a, struct spt_entry, spt_elem);
  const struct spt_entry *spte_b = hash_entry (elem_b, struct spt_entry, spt_elem);
  return spte_a->user_vaddr < spte_b->user_vaddr;
}

// ch3 구현 - 프로세스 종료 시 각 보조 페이지 엔트리가 점유한 물리 프레임 및 스왑 슬롯 해제
static void
spt_entry_cleanup (struct hash_elem *elem, void *aux UNUSED)
{
  struct spt_entry *spte = hash_entry (elem, struct spt_entry, spt_elem);

  if (spte->in_memory)
    {
      void *kaddr = pagedir_get_page (thread_current ()->pagedir, spte->user_vaddr);
      if (kaddr != NULL)
        release_user_frame (kaddr);
    }
  else if (spte->page_type == PAGE_SOURCE_ANON)
    {
      swap_clear_slot (spte->swap_index);
    }

  free (spte);
}

// ch3 구현 - 스레드별 보조 페이지 테이블(SPT) 초기화
void
spt_init (struct hash *spt_table)
{
  hash_init (spt_table, spt_hash_compute, spt_addr_less, NULL);
  thread_current ()->spt_initialized = true;
}

// ch3 구현 - 스레드 종료 시 보조 페이지 테이블 전체 파괴 및 자원 회수
void
spt_destroy (struct hash *spt_table)
{
  if (!thread_current ()->spt_initialized)
    return;
  hash_destroy (spt_table, spt_entry_cleanup);
  thread_current ()->spt_initialized = false;
}

// ch3 구현 - 가상 주소(vaddr)가 속한 페이지의 보조 페이지 엔트리 검색
struct spt_entry *
spt_find_page (void *vaddr)
{
  if (!thread_current ()->spt_initialized)
    return NULL;

  struct spt_entry query_entry;
  struct hash_elem *found_elem;

  query_entry.user_vaddr = pg_round_down (vaddr);
  found_elem = hash_find (&thread_current ()->spt_table, &query_entry.spt_elem);

  if (found_elem == NULL)
    return NULL;

  return hash_entry (found_elem, struct spt_entry, spt_elem);
}

// ch3 구현 - 보조 페이지 테이블에 새로운 가상 페이지 엔트리 등록
bool
spt_insert_page (struct hash *spt_table, struct spt_entry *spte)
{
  return hash_insert (spt_table, &spte->spt_elem) == NULL;
}

// ch3 구현 - 보조 페이지 테이블에서 특정 가상 페이지 엔트리 제거 및 해제
bool
spt_delete_page (struct hash *spt_table, struct spt_entry *spte)
{
  if (hash_delete (spt_table, &spte->spt_elem) == NULL)
    return false;

  if (spte->in_memory)
    {
      void *kaddr = pagedir_get_page (thread_current ()->pagedir, spte->user_vaddr);
      if (kaddr != NULL)
        release_user_frame (kaddr);
    }
  else if (spte->page_type == PAGE_SOURCE_ANON)
    {
      swap_clear_slot (spte->swap_index);
    }

  free (spte);
  return true;
}

// ch3 구현 - 실행 파일(ELF)의 특정 오프셋에서 물리 프레임으로 페이지 데이터를 읽어오는 함수
bool
load_segment_from_file (void *kframe_addr, struct spt_entry *spte)
{
  if (spte->read_bytes > 0)
    {
      bool lock_held_by_me = lock_held_by_current_thread (&filesys_lock);
      if (!lock_held_by_me)
        lock_acquire (&filesys_lock);

      off_t actual_read = file_read_at (spte->mapped_file, kframe_addr,
                                        spte->read_bytes, spte->file_offset);

      if (!lock_held_by_me)
        lock_release (&filesys_lock);

      if (actual_read != (int) spte->read_bytes)
        return false;
    }

  memset ((uint8_t *) kframe_addr + spte->read_bytes, 0, spte->zero_bytes);
  return true;
}

// ch3 구현 - Page Fault 발생 시 물리 프레임을 할당하고 파일/스왑에서 데이터를 복구한 뒤 하드웨어 페이지 테이블에 매핑
bool
handle_mm_fault (struct spt_entry *spte)
{
  if (spte->in_memory)
    return true;

  struct frame_slot *new_frame = allocate_user_frame (PAL_USER);
  if (new_frame == NULL)
    return false;

  new_frame->spte = spte;
  bool load_ok = false;

  switch (spte->page_type)
    {
    case PAGE_SOURCE_ELF:
    case PAGE_SOURCE_MMAP:
      load_ok = load_segment_from_file (new_frame->kernel_vaddr, spte);
      break;

    case PAGE_SOURCE_ANON:
      swap_read_page (spte->swap_index, new_frame->kernel_vaddr);
      load_ok = true;
      break;

    default:
      load_ok = false;
      break;
    }

  if (!load_ok)
    {
      release_user_frame (new_frame->kernel_vaddr);
      return false;
    }

  if (!install_user_page (spte->user_vaddr, new_frame->kernel_vaddr, spte->is_writable))
    {
      release_user_frame (new_frame->kernel_vaddr);
      return false;
    }

  spte->in_memory = true;
  return true;
}
