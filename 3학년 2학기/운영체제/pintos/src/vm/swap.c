#include "vm/swap.h"
#include <bitmap.h>
#include <debug.h>
#include "devices/block.h"
#include "threads/synch.h"
#include "threads/vaddr.h"

// ch3 구현 - 1개 페이지(4KB)당 디스크 섹터(512B) 개수 상수 (8개 섹터)
#define SECTORS_PER_PAGE (PGSIZE / BLOCK_SECTOR_SIZE)

// ch3 구현 - 스왑 블록 장치 포인터, 슬롯 관리용 비트맵, 스왑 동기화 락
static struct block *swap_disk_dev;
static struct bitmap *swap_slot_map;
static struct lock swap_io_lock;

// ch3 구현 - BLOCK_SWAP 장치를 탐색하고 스왑 비트맵 및 락 초기화
void
swap_table_init (void)
{
  lock_init (&swap_io_lock);
  swap_disk_dev = block_get_role (BLOCK_SWAP);
  if (swap_disk_dev == NULL)
    {
      swap_slot_map = NULL;
      return;
    }

  size_t total_slots = block_size (swap_disk_dev) / SECTORS_PER_PAGE;
  swap_slot_map = bitmap_create (total_slots);
}

// ch3 구현 - 스왑 디스크의 특정 슬롯(slot_idx)에서 4KB 데이터를 읽어 물리 프레임(kframe_addr)에 복구
void
swap_read_page (size_t slot_idx, void *kframe_addr)
{
  ASSERT (swap_disk_dev != NULL && swap_slot_map != NULL);

  lock_acquire (&swap_io_lock);

  if (!bitmap_test (swap_slot_map, slot_idx))
    PANIC ("Attempted to swap in from an unoccupied swap slot!");

  size_t sec_idx;
  for (sec_idx = 0; sec_idx < SECTORS_PER_PAGE; sec_idx++)
    {
      block_read (swap_disk_dev,
                  slot_idx * SECTORS_PER_PAGE + sec_idx,
                  (uint8_t *) kframe_addr + sec_idx * BLOCK_SECTOR_SIZE);
    }

  bitmap_set (swap_slot_map, slot_idx, false);
  lock_release (&swap_io_lock);
}

// ch3 구현 - 빈 스왑 슬롯을 할당받아 물리 프레임(kframe_addr)의 4KB 데이터를 스왑 디스크에 저장
size_t
swap_write_page (void *kframe_addr)
{
  ASSERT (swap_disk_dev != NULL && swap_slot_map != NULL);

  lock_acquire (&swap_io_lock);

  size_t free_slot = bitmap_scan_and_flip (swap_slot_map, 0, 1, false);
  if (free_slot == BITMAP_ERROR)
    PANIC ("Swap disk is completely full!");

  size_t sec_idx;
  for (sec_idx = 0; sec_idx < SECTORS_PER_PAGE; sec_idx++)
    {
      block_write (swap_disk_dev,
                   free_slot * SECTORS_PER_PAGE + sec_idx,
                   (const uint8_t *) kframe_addr + sec_idx * BLOCK_SECTOR_SIZE);
    }

  lock_release (&swap_io_lock);
  return free_slot;
}

// ch3 구현 - 프로세스 종료 시 스왑 디스크에 남아있던 슬롯 비트를 반환(초기화)
void
swap_clear_slot (size_t slot_idx)
{
  if (swap_slot_map == NULL)
    return;

  lock_acquire (&swap_io_lock);
  bitmap_set (swap_slot_map, slot_idx, false);
  lock_release (&swap_io_lock);
}
