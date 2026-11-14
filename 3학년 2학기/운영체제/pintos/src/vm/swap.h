#ifndef VM_SWAP_H
#define VM_SWAP_H

// ch3 구현 - 스왑 디스크(Swap Disk) 및 스왑 테이블 관리 헤더 정의
#include <stddef.h>

// ch3 구현 - 스왑 테이블 초기화 및 페이지 단위 스왑 인/아웃 함수 선언
void swap_table_init (void);
void swap_read_page (size_t slot_idx, void *kframe_addr);
size_t swap_write_page (void *kframe_addr);
void swap_clear_slot (size_t slot_idx);

#endif /* vm/swap.h */
