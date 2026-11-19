#ifndef VM_PAGE_H
#define VM_PAGE_H

// ch3 구현 - 보조 페이지 테이블(Supplemental Page Table) 헤더 및 자료구조 정의
#include <hash.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "filesys/off_t.h"

// ch3 구현 - 가상 페이지의 원본 데이터 출처 타입 정의
#define PAGE_SOURCE_ELF  0  /* 실행 파일(ELF)로부터 지연 로딩되는 페이지 */
#define PAGE_SOURCE_ANON 1  /* 스왑 영역 또는 0으로 초기화되는 익명 페이지(스택 등) */
#define PAGE_SOURCE_MMAP 2  /* 메모리 매핑된 파일(mmap)로부터 로드되는 페이지 */

// ch3 구현 - 스택 최대 확장 크기 제한 (8MB)
#define MAX_STACK_BOUNDARY (1 << 23)

// ch3 구현 - 각 가상 페이지(4KB)의 메타데이터를 관리하는 보조 페이지 엔트리 구조체
struct spt_entry
  {
    uint8_t page_type;          /* PAGE_SOURCE_ELF, PAGE_SOURCE_ANON, PAGE_SOURCE_MMAP */
    void *user_vaddr;           /* 페이지 정렬된 유저 가상 주소 (해시 키) */
    bool is_writable;           /* 쓰기 권한 여부 (true: R/W, false: Read-Only) */
    bool in_memory;             /* 현재 물리 프레임(RAM)에 적재되어 있는지 여부 */
    bool is_pinned;             /* 커널 I/O 도중 프레임 교체(Eviction) 방지 플래그 */

    /* 지연 로딩(Lazy Loading) 및 파일 매핑 정보 */
    struct file *mapped_file;   /* 데이터를 읽어올 파일 포인터 */
    size_t file_offset;         /* 파일 내 읽기 시작 오프셋 */
    size_t read_bytes;          /* 파일에서 실제로 읽어들일 바이트 수 */
    size_t zero_bytes;          /* 페이지 나머지 공간을 0으로 채울 바이트 수 */

    /* 스왑 디스크(Swap Disk) 위치 정보 */
    size_t swap_index;          /* 스왑 아웃되었을 때 저장된 스왑 슬롯 번호 */
    bool in_swap;               /* 현재 스왑 디스크 슬롯을 실제로 점유 중인지 여부 */

    struct hash_elem spt_elem;  /* 프로세스별 보조 페이지 해시 테이블 연결 원소 */
    struct list_elem mmap_elem; /* 메모리 매핑 파일(mmap_desc) 소속 페이지 리스트 원소 */
  };

// ch3 구현 - 메모리 매핑된 파일(Memory Mapped File) 디스크립터 구조체
struct mmap_desc
  {
    int map_id;                 /* 매핑 고유 식별자 (mapid_t) */
    struct file *reopened_file; /* 독립적인 파일 오프셋/수명 유지를 위해 file_reopen된 포인터 */
    struct list mapped_pages;   /* 이 매핑에 속한 spt_entry들의 리스트 */
    struct list_elem desc_elem; /* 스레드의 mmap_list 연결 원소 */
  };

// ch3 구현 - 보조 페이지 테이블 초기화, 검색, 삽입, 삭제 및 폴트 처리 함수 선언
void spt_init (struct hash *spt_table);
void spt_destroy (struct hash *spt_table);

struct spt_entry *spt_find_page (void *vaddr);
bool spt_insert_page (struct hash *spt_table, struct spt_entry *spte);
bool spt_delete_page (struct hash *spt_table, struct spt_entry *spte);

bool load_segment_from_file (void *kframe_addr, struct spt_entry *spte);
bool handle_mm_fault (struct spt_entry *spte);

#endif /* vm/page.h */
