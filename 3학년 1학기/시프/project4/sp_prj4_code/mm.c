/*
 * Segregated Free List 활용
 *
 * Segregated Free List 방식을 사용하여 구현되었습니다.
 * Free block들은 2의 거듭제곱 단위로 나뉜 20개의 크기별 Size class에 
 * 독립적인 이중 연결 리스트 형태로 저장됩니다.
 *
 * 1. Malloc
 *    요청된 크기(오버헤드 포함, 8바이트 정렬)에 맞는 서랍을 찾아 First-fit 방식으로 탐색합니다.
 *    발견된 블록이 충분히 크다면, 메모리 낭비를 막기 위해 필요한 만큼만 잘라내고(Splitting)
 *    남은 공간은 다시 free list로 돌려보냅니다.
 *
 * 2. Free & Coalesce
 *    블록이 free되면, 앞뒤로 인접한 free block들을 확인하여 coalescing합니다.
 *    이를 통해 외부 단편화를 최소화하고 거대한 빈 블록을 유지합니다.
 *
 * 3. 블럭 최소 크기 및 정렬
 *    모든 블록은 4바이트 헤더와 4바이트 푸터를 가집니다.
 *    free block의 경우 페이로드 영역에 앞/뒤 빈 블록을 가리키는 8바이트 포인터 2개를 저장합니다.
 *    allocated block이 free되어서 free block이 되면, 4바이터 헤더, 4바이터 푸터,
 *    8바이트 prev, 8바이트 next를 저장해야 하므로, 24바이트를 최소한으로 필요로 하게 됩니다.
 *    따라서 블록의 최소 크기는 24바이트로 제한되며 철저하게 8바이트 정렬(Alignment)을 유지합니다.
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>

#include "mm.h"
#include "memlib.h"

/*********************************************************
 * NOTE TO STUDENTS: Before you do anything else, please
 * provide your information in the following struct.
 ********************************************************/
team_t team = {
    /* Your student ID */
    "20221627",
    /* Your full name*/
    "Hyunjun Tae",
    /* Your email address */
    "juny000041@sogang.ac.kr",
};

// 기본 상수 및 매크로 정의
#define WORDSIZE 4              // 헤더/푸터 크기 (바이트)
#define DOUBLE_WORDSIZE 8       // 더블 워드(Double word) 크기 (바이트)
#define CHUNKSIZE  (1<<12)      // 힙 확장 시 초기 요청 크기 (바이트)
#define LIST_COUNT  20          // Segregated list 개수


// p - 특정 주소를 가리킴. ex)헤더의 시작 주소, 푸터의 시작 주소
// bp - 블럭의 payload 시작 부분을 가리킴. ex) allocated block의 payload 시작 부분, free block의 prev 포인터 부분

#define ALIGNMENT 8
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~0x7) // 8의 배수로 올림 정렬

#define MAX(x, y) ((x) > (y)? (x) : (y))

// 블록의 크기/할당 상태를 1 워드로 압축
#define MAKE_BLOCK_INFO(size, state)  ((size) | (state))

// 주소 p에 있는 워드 읽기/쓰기
#define GET_WORD(p)       (*(unsigned int *)(p))
#define PUT_WORD(p, val)  (*(unsigned int *)(p) = (val))

// 주소 p에서 블럭 크기/할당 상태 읽기
#define GET_SIZE(p)  (GET_WORD(p) & ~0x7)
#define GET_STATE(p) (GET_WORD(p) & 0x1)

// 블럭 포인터 bp를 이용해 해당 블록의 헤더/푸터 주소 계산
#define HEADER_P(bp)       ((char *)(bp) - WORDSIZE)
#define FOOTER_P(bp)       ((char *)(bp) + GET_SIZE(HEADER_P(bp)) - DOUBLE_WORDSIZE)

// 블럭 포인터 bp를 이용해 이전/다음 블럭 주소 계산
#define NEXT_BLOCK_P(bp)  ((char *)(bp) + GET_SIZE(((char *)(bp) - WORDSIZE)))
#define PREV_BLOCK_P(bp)  ((char *)(bp) - GET_SIZE(((char *)(bp) - DOUBLE_WORDSIZE)))

// 블럭 포인터 bp를 이용해 free list 에서 이전/다음 free 블럭 주소 찾기
#define NEXT_FREE_P(bp)  (*(void **)((char *)(bp) + DOUBLE_WORDSIZE))
#define PREV_FREE_P(bp)  (*(void **)(bp))

// 전역 변수 선언
static void **segregated_free_lists; // explicit free list 들을 담는 배열의 시작 포인터
static char *heap_listp = 0;         // 초기 힙 포인터. 힙 시작 주소 저장

// 추가 함수 선언
static void *extend_heap(size_t words);
static void split_allocate(void *bp, size_t target_block_size);
static void *coalesce(void *bp);
static void insert_node(void *bp, size_t size);
static void delete_node(void *bp);
static int get_class(size_t size);



// 힙 메모리가 부족할 때 운영체제에 추가 메모리를 요청하여 대형 free block 하나 생성. 워드 단위로 입력받아야함.
static void *extend_heap(size_t words) {
    char *bp;
    size_t size;

    // 정렬 규칙(8바이트 배수)을 유지해야함 -> 짝수 개의 워드 단위로 할당
    size = (words % 2) ? (words + 1) * WORDSIZE : words * WORDSIZE;

    // 최소 블럭 크기보다는 크게 맞춰주기 (헤더(4) + Prev(8) + Next(8) + 푸터(4) = 24바이트)
    if (size < 24)
        size = 24;
    
    // 추가적인 8바이트 정렬 보정
    if (size % DOUBLE_WORDSIZE != 0) {
        size = size + (DOUBLE_WORDSIZE - (size % DOUBLE_WORDSIZE));
    }

    // 메모리 요청 실패 시 NULL 반환
    if ((long)(bp = mem_sbrk(size)) == -1)
        return NULL;

    // 새롭게 얻어온 영역을 free block으로 초기화하고 에필로그 헤더를 뒤로 밀어냄
    PUT_WORD(HEADER_P(bp), MAKE_BLOCK_INFO(size, 0));             // 새로운 free block 헤더
    PUT_WORD(FOOTER_P(bp), MAKE_BLOCK_INFO(size, 0));             // 새로운 free block 푸터
    PUT_WORD(HEADER_P(NEXT_BLOCK_P(bp)), MAKE_BLOCK_INFO(0, 1)); // 새로운 에필로그 헤더 갱신

    // 만약 바로 앞 블록이 비어있다면 하나로 병합(Coalesce) 수행
    return coalesce(bp);
}

// 요청한 크기만큼 블록을 할당 상태로 만들고, 남은 공간이 충분히 크면 분할함
static void split_allocate(void *bp, size_t target_block_size) {
    size_t csize = GET_SIZE(HEADER_P(bp)); // 현재 블록의 총 크기
    delete_node(bp); // 할당될거니까 free list에서 제거

    // 남은 공간이 최소 블럭 크기(24바이트) 이상이면 블록을 쪼갬
    if ((csize - target_block_size) >= 24) { 
        PUT_WORD(HEADER_P(bp), MAKE_BLOCK_INFO(target_block_size, 1));
        PUT_WORD(FOOTER_P(bp), MAKE_BLOCK_INFO(target_block_size, 1));
        bp = NEXT_BLOCK_P(bp);
        PUT_WORD(HEADER_P(bp), MAKE_BLOCK_INFO(csize - target_block_size, 0));
        PUT_WORD(FOOTER_P(bp), MAKE_BLOCK_INFO(csize - target_block_size, 0));
        insert_node(bp, csize - target_block_size); // 쪼개고 남은 뒷부분은 다시 free list에 삽입
    } 
    
    // 남은 공간이 너무 작으면 Internal fragmentation 감수하고 전체 할당
    else {
        PUT_WORD(HEADER_P(bp), MAKE_BLOCK_INFO(csize, 1));
        PUT_WORD(FOOTER_P(bp), MAKE_BLOCK_INFO(csize, 1));
    }
}

// 인접한 free block들을 하나로 합치기
static void *coalesce(void *bp) {
    size_t prev_alloc = GET_STATE(FOOTER_P(PREV_BLOCK_P(bp))); // 이전 블록의 할당 상태
    size_t next_alloc = GET_STATE(HEADER_P(NEXT_BLOCK_P(bp))); // 다음 블록의 할당 상태
    size_t size = GET_SIZE(HEADER_P(bp));

    // 경우 1: 앞뒤 모두 할당되어 있을 때
    if (prev_alloc && next_alloc) {
        insert_node(bp, size);
        return bp;
    }

    // 경우 2: 이전 블록만 비어 있을 때
    else if (!prev_alloc && next_alloc) {
        delete_node(PREV_BLOCK_P(bp));
        size += GET_SIZE(HEADER_P(PREV_BLOCK_P(bp)));
        PUT_WORD(FOOTER_P(bp), MAKE_BLOCK_INFO(size, 0));
        PUT_WORD(HEADER_P(PREV_BLOCK_P(bp)), MAKE_BLOCK_INFO(size, 0));
        bp = PREV_BLOCK_P(bp);
        insert_node(bp, size);
    }

    // 경우 3: 다음 블록만 비어 있을 때
    else if (prev_alloc && !next_alloc) {
        delete_node(NEXT_BLOCK_P(bp));
        size += GET_SIZE(HEADER_P(NEXT_BLOCK_P(bp)));
        PUT_WORD(HEADER_P(bp), MAKE_BLOCK_INFO(size, 0));
        PUT_WORD(FOOTER_P(bp), MAKE_BLOCK_INFO(size, 0));
        insert_node(bp, size);
    }

    // 경우 4: 앞뒤 블럭 모두 비어 있을 때
    else {
        delete_node(PREV_BLOCK_P(bp));
        delete_node(NEXT_BLOCK_P(bp));
        size += GET_SIZE(HEADER_P(PREV_BLOCK_P(bp))) + GET_SIZE(FOOTER_P(NEXT_BLOCK_P(bp)));
        PUT_WORD(HEADER_P(PREV_BLOCK_P(bp)), MAKE_BLOCK_INFO(size, 0));
        PUT_WORD(FOOTER_P(NEXT_BLOCK_P(bp)), MAKE_BLOCK_INFO(size, 0));
        bp = PREV_BLOCK_P(bp);
        insert_node(bp, size);
    }
    return bp;
}

// 새로운 free block을 크기에 맞는 리스트에 삽입 (주소 오름차순 삽입)
static void insert_node(void *bp, size_t size) {
    int list_idx = get_class(size);
    void *search_ptr = segregated_free_lists[list_idx]; // 이 값을 넣을 free list
    void *insert_ptr = NULL;                            // 이 값이 해당 free list에서 들어갈 주소

    // Fragmentation를 줄이기 위해 메모리 주소 순으로 정렬하여 삽입 위치 탐색
    while (search_ptr != NULL && search_ptr < bp) {
        insert_ptr = search_ptr;
        search_ptr = NEXT_FREE_P(search_ptr);
    }

    if (search_ptr != NULL) {
        if (insert_ptr != NULL) {
            // 경우 1: 리스트 중간에 삽입할 때
            PREV_FREE_P(bp) = insert_ptr;
            NEXT_FREE_P(bp) = search_ptr;
            PREV_FREE_P(search_ptr) = bp;
            NEXT_FREE_P(insert_ptr) = bp;
        } 
        
        else {
            // 경우 2: 리스트 맨 앞에 삽입할 때
            PREV_FREE_P(bp) = NULL;
            NEXT_FREE_P(bp) = search_ptr;
            PREV_FREE_P(search_ptr) = bp;
            segregated_free_lists[list_idx] = bp;
        }
    } 
    
    else {
        if (insert_ptr != NULL) {
            // 경우 3: 리스트 맨 끝에 삽입할 때
            PREV_FREE_P(bp) = insert_ptr;
            NEXT_FREE_P(bp) = NULL;
            NEXT_FREE_P(insert_ptr) = bp;
        } 
        
        else {
            // 경우 4: 리스트가 완전히 비어있을 때
            PREV_FREE_P(bp) = NULL;
            NEXT_FREE_P(bp) = NULL;
            segregated_free_lists[list_idx] = bp;
        }
    }
}

// free block을 free block list에서 삭제
static void delete_node(void *bp) {
    size_t size = GET_SIZE(HEADER_P(bp));
    int list_idx = get_class(size);

    void *prev = PREV_FREE_P(bp);
    void *next = NEXT_FREE_P(bp);

    if (prev == NULL && next == NULL) {
        // 유일한 노드였던 경우
        segregated_free_lists[list_idx] = NULL;
    } else if (prev == NULL && next != NULL) {
        // 첫 번째 노드를 지우는 경우
        PREV_FREE_P(next) = NULL;
        segregated_free_lists[list_idx] = next;
    } else if (prev != NULL && next == NULL) {
        // 마지막 노드를 지우는 경우
        NEXT_FREE_P(prev) = NULL;
    } else {
        // 중간에 끼어있는 노드를 지우는 경우
        NEXT_FREE_P(prev) = next;
        PREV_FREE_P(next) = prev;
    }
}

// 주어진 블럭 크기에 알맞은 segregated free list의 인덱스를 반환
static int get_class(size_t size) {
    int index = 0;
    size_t min_size = 32;
    while (index < LIST_COUNT - 1 && size >= min_size) {
        min_size <<= 1; // min_size 2배로 늘리기
        index++;
    }
    return index;
}





// 메모리 할당자 시스템 초기화
int mm_init(void) {
    int i;
    
    // 리스트 포인터 배열(20개) 공간과 프롤로그/에필로그 블럭 공간을 한 번에 할당 요청
    size_t array_bytes = LIST_COUNT * sizeof(void *);
    size_t initial_bytes = array_bytes + 4 * WORDSIZE; // 패딩(1), 프롤로그(2), 에필로그(1) = 4워드
    
    if ((segregated_free_lists = mem_sbrk(initial_bytes)) == (void *)-1)
        return -1;

    // free list 배열 포인터들을 모두 NULL로 초기화
    for (i = 0; i < LIST_COUNT; i++) {
        segregated_free_lists[i] = NULL;
    }

    // 배열 공간 바로 다음부터 실제 힙 영역으로 설정
    heap_listp = (char *)segregated_free_lists + array_bytes;
    
    PUT_WORD(heap_listp, 0);                                                    // 정렬을 맞추기 위한 패딩 삽입
    PUT_WORD(heap_listp + (1 * WORDSIZE), MAKE_BLOCK_INFO(DOUBLE_WORDSIZE, 1)); // 프롤로그 헤더 (크기 8, 할당됨)
    PUT_WORD(heap_listp + (2 * WORDSIZE), MAKE_BLOCK_INFO(DOUBLE_WORDSIZE, 1)); // 프롤로그 푸터 (크기 8, 할당됨)
    PUT_WORD(heap_listp + (3 * WORDSIZE), MAKE_BLOCK_INFO(0, 1));               // 에필로그 헤더 (크기 0, 할당됨)
    heap_listp += (2 * WORDSIZE);

    // 힙 공간이 비어있으므로 CHUNKSIZE 만큼 힙 공간을 확장시켜 초기 free block 생성
    if (extend_heap(CHUNKSIZE / WORDSIZE) == NULL)
        return -1;

    return 0;
}

// 사용자의 요청 크기에 맞춰 메모리 블록을 할당
void *mm_malloc(size_t size) {
    size_t target_block_size;      // 오버헤드를 포함하여 조정한 실제 할당 크기
    size_t extendsize; // 적합한 블록이 없을 때 추가로 요청할 힙 크기
    void *bp = NULL;

    // 잘못된 요청(0바이트) 무시
    if (size == 0)
        return NULL;

    // 헤더와 푸터(8바이트)를 포함하여 8의 배수로 크기 정렬 연산
    target_block_size = ALIGN(size + 8);
    
    // 최소 블럭 크기 제약 (24바이트) 강제
    if (target_block_size < 24)
        target_block_size = 24;

    // 크기에 맞는 free list에서 적절히 들어갈 수 있는 블럭 First-fit 탐색
    int list_idx = get_class(target_block_size);
    while (list_idx < LIST_COUNT) {
        bp = segregated_free_lists[list_idx];
        while ((bp != NULL) && ((target_block_size > GET_SIZE(HEADER_P(bp))))) {
            bp = NEXT_FREE_P(bp);
        }
        if (bp != NULL)
            break;
        list_idx++; // 현재 클래스에 없으면 더 큰 클래스로 이동
    }

    // 적합한 free block을 찾지 못한 경우, 운영체제에 메모리 힙 확장을 요청
    if (bp == NULL) {
        extendsize = MAX(target_block_size, CHUNKSIZE);
        if ((bp = extend_heap(extendsize / WORDSIZE)) == NULL)
            return NULL; // 메모리 확장마저 실패 시 NULL 반환
    }

    // 찾은 블록에 할당 처리 (남는 공간이 크면 쪼개기(Splitting) 포함)
    split_allocate(bp, target_block_size);
    return bp;
}

// 할당된 블록을 다시 free 상태로 반환
void mm_free(void *bp) {
    if (bp == NULL)
        return;

    size_t size = GET_SIZE(HEADER_P(bp));

    // 헤더와 푸터의 할당 비트를 0으로 초기화
    PUT_WORD(HEADER_P(bp), MAKE_BLOCK_INFO(size, 0));
    PUT_WORD(FOOTER_P(bp), MAKE_BLOCK_INFO(size, 0));
    
    // 인접한 빈 블록이 있으면 하나로 합침
    coalesce(bp);
}

// 기존 블록의 크기를 변경. 가능하면 메모리 복사를 피하는 최적화 포함.
void *mm_realloc(void *ptr, size_t size) {
    if (size == 0) {
        mm_free(ptr);
        return NULL;
    }
    if (ptr == NULL) {
        return mm_malloc(size);
    }

    void *newptr;
    size_t copySize;
    size_t target_block_size;

    // 새롭게 요구하는 8바이트 정렬된 전체 블럭 크기 계산
    target_block_size = ALIGN(size + 8);
    if (target_block_size < 24)
        target_block_size = 24;

    size_t oldSize = GET_SIZE(HEADER_P(ptr));

    // 요청한 크기가 기존 할당된 블럭 크기보다 작거나 같으면 굳이 옮기지 않음
    if (oldSize >= target_block_size) {
        return ptr;
    }

    // 뒷 블록이 마침 비어있고, 병합했을 때 새로 요청한 크기를 감당할 수 있다면 그 자리에서 병합
    size_t next_alloc = GET_STATE(HEADER_P(NEXT_BLOCK_P(ptr)));
    size_t next_size = GET_SIZE(HEADER_P(NEXT_BLOCK_P(ptr)));

    if (!next_alloc && (oldSize + next_size >= target_block_size)) {
        delete_node(NEXT_BLOCK_P(ptr)); // 뒤에 있던 free block을 리스트에서 빼옴
        PUT_WORD(HEADER_P(ptr), MAKE_BLOCK_INFO(oldSize + next_size, 1));
        PUT_WORD(FOOTER_P(ptr), MAKE_BLOCK_INFO(oldSize + next_size, 1));
        return ptr; // 새로 할당/복사 없이 기존 포인터 그대로 반환!
    }

    // 새로운 공간을 찾아서 할당하고 기존 데이터 복사 (가장 느린 경로)
    newptr = mm_malloc(size);
    if (newptr == NULL)
        return NULL;
    
    copySize = GET_SIZE(HEADER_P(ptr)) - DOUBLE_WORDSIZE; // 실제 복사할 데이터 크기 (헤더/푸터 제외)
    if (size < copySize)
        copySize = size;
        
    memcpy(newptr, ptr, copySize);
    mm_free(ptr);
    
    return newptr;
}
