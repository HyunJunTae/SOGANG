#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "list.h"
#include "hash.h"
#include "bitmap.h"

#include <stddef.h>

#define MAX_ITEMS 10  // 자료구조는 10개 이하로 생성
#define MAX_NAME_LEN 50


// hash entry 직접 작성 - elem이 들어오면 item을 반환.
#define hash_entry(HASH_ELEM, STRUCT, MEMBER) \
    ((STRUCT *)((uint8_t *)(HASH_ELEM) - offsetof(STRUCT, MEMBER)))



// 실제 데이터와 elem을 담을 struct list_item 과 hash_item.
struct list_item {
    int data;
    struct list_elem elem;
};

struct hash_item {
    int data;
    struct hash_elem elem;
};


// 선언한 list, hash, bitmap을 묶어서 관리할 배열 선언
struct { char name[MAX_NAME_LEN]; struct list *list; } lists[MAX_ITEMS];
int list_cnt = 0;

struct { char name[MAX_NAME_LEN]; struct hash *hash; } hashes[MAX_ITEMS];
int hash_cnt = 0;

struct { char name[MAX_NAME_LEN]; struct bitmap *bitmap; } bitmaps[MAX_ITEMS];
int bitmap_cnt = 0;



// list 크기 비교 함수
bool my_list_less_func(const struct list_elem *a, const struct list_elem *b, void *aux) {

    struct list_item *item_a = list_entry(a, struct list_item, elem);
    struct list_item *item_b = list_entry(b, struct list_item, elem);

    return item_a->data < item_b->data;
}



// 해시 함수 3개 구현
// hash_hash_func 구현. 데이터를 몇 번 버킷에 넣을지 계산
// hash_elem을 hash_entry에 넣어서 실제 데이터를 찾아오고, 실제 데이터로 해시값 계산.
unsigned my_hash_func(const struct hash_elem *e, void *aux) {

    struct hash_item *item = hash_entry(e, struct hash_item, elem);

    return hash_int(item->data); 
}

// hash_less_func 구현. 크기 비교 함수.
bool my_less_func(const struct hash_elem *a, const struct hash_elem *b, void *aux) {

    struct hash_item *item_a = hash_entry(a, struct hash_item, elem);
    struct hash_item *item_b = hash_entry(b, struct hash_item, elem);

    return item_a->data < item_b->data;
}

// 3. hash_action_func 구현. 해시 테이블을 없앨 때, 해시 테이블 안에 item들도 메모리 해제.
void my_action_func(struct hash_elem *e, void *aux) {

    struct hash_item *item = hash_entry(e, struct hash_item, elem);

    free(item); 
}


// hash_apply 보조 함수 - 제곱
void my_hash_square_func(struct hash_elem *e, void *aux) {
    struct hash_item *item = hash_entry(e, struct hash_item, elem);
    item->data = (item->data) * (item->data);
}

// hash_apply 보조 함수 - 세제곱
void my_hash_triple_func(struct hash_elem *e, void *aux) {
    struct hash_item *item = hash_entry(e, struct hash_item, elem);
    item->data = (item->data) * (item->data) * (item->data);
}












/* --- main 함수 --- */
int main() {
    char line[256];
    char cmd[50], arg1[50], arg2[50], arg3[50], arg4[50], arg5[50], arg6[50];

    // STDIN으로부터 한 줄씩 입력받기
    while (fgets(line, sizeof(line), stdin) != NULL) {
        line[strcspn(line, "\n")] = 0; // 개행문자 제거. quit과 quit\n 은 다른 문자니까 없애줘야함.
        if (strlen(line) == 0) continue; // 만약 입력이 없고 엔터키만 들어온거면 다시 입력

        // 변수 초기화
        cmd[0] = arg1[0] = arg2[0] = arg3[0] = arg4[0] = arg5[0] = arg6[0] = '\0';
        
        // 명령어, argument 분리
        sscanf(line, "%s %s %s %s %s %s %s", cmd, arg1, arg2, arg3, arg4, arg5, arg6);











        
        // 1. 공통 명령어 처리 (quit, create, dumpdata, delete)
        if (strcmp(cmd, "quit") == 0) {
            break;
        }

        else if (strcmp(cmd, "create") == 0) {

            // 리스트 생성
            if (strcmp(arg1, "list") == 0) {
                lists[list_cnt].list = (struct list *)malloc(sizeof(struct list));
                list_init(lists[list_cnt].list);
                strcpy(lists[list_cnt].name, arg2);
                list_cnt++;
            }
            
            // 해시테이블 생성
            else if (strcmp(arg1, "hashtable") == 0) {
                hashes[hash_cnt].hash = (struct hash *)malloc(sizeof(struct hash));
                hash_init(hashes[hash_cnt].hash, my_hash_func, my_less_func, NULL); 
                strcpy(hashes[hash_cnt].name, arg2);
                hash_cnt++;
            }

            // 비트맵 생성
            else if (strcmp(arg1, "bitmap") == 0) {
                int bit_cnt = atoi(arg3); 
                bitmaps[bitmap_cnt].bitmap = bitmap_create((size_t)bit_cnt);
                strcpy(bitmaps[bitmap_cnt].name, arg2);
                bitmap_cnt++;
            }
        }

        else if (strcmp(cmd, "dumpdata") == 0) {
            bool found = false;

            // 리스트 출력
            for (int i = 0; i < list_cnt; i++) {
                if (strcmp(lists[i].name, arg1) == 0) {
                    struct list *l = lists[i].list;
                    if (!list_empty(l)) {
                        for (struct list_elem *e = list_begin(l); e != list_end(l); e = list_next(e)) {
                            struct list_item *item = list_entry(e, struct list_item, elem);
                            printf("%d ", item->data);
                        }
                        printf("\n");
                    }
                    found = true; break;
                }
            }

            // 해시테이블 출력
            if (!found) {
                for (int i = 0; i < hash_cnt; i++) {
                    if (strcmp(hashes[i].name, arg1) == 0) {
                        struct hash *h = hashes[i].hash;
                        if (!hash_empty(h)) {
                            struct hash_iterator it;
                            hash_first(&it, h);
                            while (hash_next(&it)) {
                                struct hash_item *item = hash_entry(hash_cur(&it), struct hash_item, elem);
                                printf("%d ", item->data);
                            }
                            printf("\n");
                        }
                        found = true; break;
                    }
                }
            }
            
            // 비트맵 출력
            if (!found) {
                for (int i = 0; i < bitmap_cnt; i++) {
                    if (strcmp(bitmaps[i].name, arg1) == 0) {
                        struct bitmap *b = bitmaps[i].bitmap;
                        size_t size = bitmap_size(b);
                        for (size_t j = 0; j < size; j++) {
                            printf("%d", bitmap_test(b, j) ? 1 : 0);
                        }
                        printf("\n");
                        break;
                    }
                }
            }
        } 
        else if (strcmp(cmd, "delete") == 0) {
            bool found = false;

            // 리스트 삭제
            for (int i = 0; i < list_cnt; i++) {
                if (strcmp(lists[i].name, arg1) == 0) {

                    // list item 삭제
                    while (!list_empty(lists[i].list)) {
                        struct list_elem *e = list_pop_front(lists[i].list);
                        struct list_item *item = list_entry(e, struct list_item, elem);
                        free(item);
                    }

                    // list 삭제
                    free(lists[i].list);
                    lists[i].name[0] = '\0';
                    found = true; break;
                }
            }
            // 해시 삭제
            if (!found) {
                for (int i = 0; i < hash_cnt; i++) {
                    if (strcmp(hashes[i].name, arg1) == 0) {
                        // [수정된 부분] NULL 대신 my_action_func를 넘겨서 내부 데이터까지 싹 청소!
                        hash_destroy(hashes[i].hash, my_action_func);
                        free(hashes[i].hash);
                        hashes[i].name[0] = '\0';
                        found = true; break;
                    }
                }
            }
            // 비트맵 삭제
            if (!found) {
                for (int i = 0; i < bitmap_cnt; i++) {
                    if (strcmp(bitmaps[i].name, arg1) == 0) {
                        bitmap_destroy(bitmaps[i].bitmap);
                        bitmaps[i].name[0] = '\0';
                        break;
                    }
                }
            }
        }
















        // 2. 리스트(List) 명령어 처리
        else if (strncmp(cmd, "list_", 5) == 0) {
            if (strcmp(cmd, "list_push_front") == 0) { 
                for (int i = 0; i < list_cnt; i++) {
                    if (strcmp(lists[i].name, arg1) == 0) {
                        struct list_item *new_item = (struct list_item *)malloc(sizeof(struct list_item));
                        new_item->data = atoi(arg2);
                        list_push_front(lists[i].list, &new_item->elem);
                        break;
                    }
                }
            }
            else if (strcmp(cmd, "list_push_back") == 0) { 
                for (int i = 0; i < list_cnt; i++) {
                    if (strcmp(lists[i].name, arg1) == 0) {
                        struct list_item *new_item = (struct list_item *)malloc(sizeof(struct list_item));
                        new_item->data = atoi(arg2);
                        list_push_back(lists[i].list, &new_item->elem);
                        break;
                    }
                }
            }
            else if (strcmp(cmd, "list_pop_front") == 0) {
                for (int i = 0; i < list_cnt; i++) {
                    if (strcmp(lists[i].name, arg1) == 0) {
                        // 리스트가 빈 리스트인지 먼저 확인.
                        if (!list_empty(lists[i].list)) {
                            struct list_elem *e = list_pop_front(lists[i].list);
                            struct list_item *item = list_entry(e, struct list_item, elem);
                           free(item); 
                        }
                        break;
                    }
                }
            }
            else if (strcmp(cmd, "list_pop_back") == 0) {
                for (int i = 0; i < list_cnt; i++) {
                    if (strcmp(lists[i].name, arg1) == 0) {
                        // 리스트가 빈 리스트인지 먼저 확인.
                        if (!list_empty(lists[i].list)) {
                            struct list_elem *e = list_pop_back(lists[i].list);
                            struct list_item *item = list_entry(e, struct list_item, elem);
                            free(item);
                        }
                        break;
                    }
                }
            }
            else if (strcmp(cmd, "list_insert") == 0) {
                for (int i = 0; i < list_cnt; i++) {
                    if (strcmp(lists[i].name, arg1) == 0) {
                        int index = atoi(arg2); // arg2 : 인덱스
                        int value = atoi(arg3); // arg3 : 넣을 데이터
                        
                        // 인덱스 / 리스트 사이즈 검사.
                        size_t size = list_size(lists[i].list);
                        if (index < 0 || index > size) {
                            break; // 인덱스가 0보다 작거나, 사이즈보다 크면 오류 -> 실행 X
                        }

                        // arg2 인덱스까지 포인터 한 칸씩 옮기기.
                        struct list_elem *e = list_begin(lists[i].list);    
                        for (int j = 0; j < index; j++) {
                            e = list_next(e);
                        }

                        struct list_item *new_item = (struct list_item *)malloc(sizeof(struct list_item));
                        new_item->data = value;

                        list_insert(e, &new_item->elem);
                        // new_item 자체가 포인터라서 주소를 가리키고 있음.
                        // 거기에->를 이용해서 elem에 접근한 후에, 
                        // &를 이용해서 주소 형태로 매개변수로 넘겨주기
                        
                        break;
                    }
                }
            }
            else if (strcmp(cmd, "list_insert_ordered") == 0) {
                for (int i = 0; i < list_cnt; i++) {
                    if (strcmp(lists[i].name, arg1) == 0) {
                        int value = atoi(arg2); // arg2 : 넣을 데이터
                        
                        struct list_item *new_item = (struct list_item *)malloc(sizeof(struct list_item));
                        new_item->data = value;

                        list_insert_ordered(lists[i].list, &new_item->elem, my_list_less_func, NULL);
                        break;
                    }
                }
            }
            else if (strcmp(cmd, "list_front") == 0) {
                for (int i = 0; i < list_cnt; i++) {
                    if (strcmp(lists[i].name, arg1) == 0) {
                        if (!list_empty(lists[i].list)) {
                            struct list_item *item = list_entry(list_front(lists[i].list), struct list_item, elem);
                            printf("%d\n", item->data);
                        }
                        break;
                    }
                }
            }
            else if (strcmp(cmd, "list_back") == 0) {
                for (int i = 0; i < list_cnt; i++) {
                    if (strcmp(lists[i].name, arg1) == 0) {
                        if (!list_empty(lists[i].list)) {
                            struct list_item *item = list_entry(list_back(lists[i].list), struct list_item, elem);
                            printf("%d\n", item->data);
                        }
                        break;
                    }
                }
            }
            else if (strcmp(cmd, "list_max") == 0) {
                for (int i = 0; i < list_cnt; i++) {
                    if (strcmp(lists[i].name, arg1) == 0) {
                        if (!list_empty(lists[i].list)) {
                            struct list_elem *e = list_max(lists[i].list, my_list_less_func, NULL);
                            struct list_item *item = list_entry(e, struct list_item, elem);
                            printf("%d\n", item->data);
                        }
                        break;
                    }
                }
            }
            else if (strcmp(cmd, "list_min") == 0) {
                for (int i = 0; i < list_cnt; i++) {
                    if (strcmp(lists[i].name, arg1) == 0) {
                        if (!list_empty(lists[i].list)) {
                            struct list_elem *e = list_min(lists[i].list, my_list_less_func, NULL);
                            struct list_item *item = list_entry(e, struct list_item, elem);
                            printf("%d\n", item->data);
                        }
                        break;
                    }
                }
            }
            else if (strcmp(cmd, "list_empty") == 0) {
                for (int i = 0; i < list_cnt; i++) {
                    if (strcmp(lists[i].name, arg1) == 0) {
                        printf("%s\n", list_empty(lists[i].list) ? "true" : "false");
                        break;
                    }
                }
            }
            else if (strcmp(cmd, "list_size") == 0) {
                for (int i = 0; i < list_cnt; i++) {
                    if (strcmp(lists[i].name, arg1) == 0) {
                        printf("%zu\n", list_size(lists[i].list));
                        break;
                    }
                }
            }

            else if (strcmp(cmd, "list_remove") == 0) {
                for (int i = 0; i < list_cnt; i++) {
                    if (strcmp(lists[i].name, arg1) == 0) {
                        int index = atoi(arg2); // arg2 : 인덱스
                        if (index >= 0 && index < list_size(lists[i].list)) {
                            struct list_elem *e = list_begin(lists[i].list);
                            for (int j = 0; j < index; j++) e = list_next(e);
                            
                            list_remove(e);
                            struct list_item *item = list_entry(e, struct list_item, elem);
                            free(item);
                        }
                        break;
                    }
                }
            }
            else if (strcmp(cmd, "list_reverse") == 0) {
                for (int i = 0; i < list_cnt; i++) {
                    if (strcmp(lists[i].name, arg1) == 0) {
                        list_reverse(lists[i].list);
                        break;
                    }
                }
            }
            else if (strcmp(cmd, "list_sort") == 0) {
                for (int i = 0; i < list_cnt; i++) {
                    if (strcmp(lists[i].name, arg1) == 0) {
                        list_sort(lists[i].list, my_list_less_func, NULL);
                        break;
                    }
                }
            }
            else if (strcmp(cmd, "list_shuffle") == 0) {
                for (int i = 0; i < list_cnt; i++) {
                    if (strcmp(lists[i].name, arg1) == 0) {
                        list_shuffle(lists[i].list);
                        break;
                    }
                }
            }

            else if (strcmp(cmd, "list_splice") == 0) {
                int dest_idx = -1, src_idx = -1;
                for (int i = 0; i < list_cnt; i++) {
                    if (strcmp(lists[i].name, arg1) == 0) dest_idx = i; // destination 리스트
                    if (strcmp(lists[i].name, arg3) == 0) src_idx = i;  // source 리스트
                }
                if (dest_idx != -1 && src_idx != -1) {
                    struct list_elem *before = list_begin(lists[dest_idx].list);
                    for (int j = 0; j < atoi(arg2); j++) before = list_next(before);
                    // arg2 : destination 리스트에서 어느 인덱스에 넣을지
                    
                    struct list_elem *first = list_begin(lists[src_idx].list);
                    for (int j = 0; j < atoi(arg4); j++) first = list_next(first);
                    // arg4 : source 리스트에서 어디부터 넣을지
                    
                    struct list_elem *last = list_begin(lists[src_idx].list);
                    for (int j = 0; j < atoi(arg5); j++) last = list_next(last);
                    // arg5 : source 리스트에서 어디까지 넣을지
                    
                    list_splice(before, first, last);
                }
            }
            else if (strcmp(cmd, "list_swap") == 0) {
                for (int i = 0; i < list_cnt; i++) {
                    if (strcmp(lists[i].name, arg1) == 0) {
                        struct list_elem *a = list_begin(lists[i].list);
                        for (int j = 0; j < atoi(arg2); j++) a = list_next(a);
                        // arg2 : swap하는 인덱스 1
                        
                        struct list_elem *b = list_begin(lists[i].list);
                        for (int j = 0; j < atoi(arg3); j++) b = list_next(b);
                        // arg3 : swap하는 인덱스 2

                        list_swap(a, b);
                        break;
                    }
                }
            }
            else if (strcmp(cmd, "list_unique") == 0) {
                // list_unique list1        - 중복 삭제
                // list_unique list1 list2  - 중복을 떼어내서 list2로 옮김)
                int dest_idx = -1, src_idx = -1;
                for (int i = 0; i < list_cnt; i++) {
                    if (strcmp(lists[i].name, arg1) == 0) dest_idx = i;
                    if (strlen(arg2) > 0 && strcmp(lists[i].name, arg2) == 0) src_idx = i;
                }
                if (dest_idx != -1) {
                    if (src_idx != -1) {
                        list_unique(lists[dest_idx].list, lists[src_idx].list, my_list_less_func, NULL);
                    } else {
                        // 가상의 쓰레기통(temp_list)을 만들어서 거기에 담은 뒤 한 번에 free.
                        struct list temp_list;
                        list_init(&temp_list);
                        list_unique(lists[dest_idx].list, &temp_list, my_list_less_func, NULL);
                        
                        while (!list_empty(&temp_list)) {
                            struct list_elem *e = list_pop_front(&temp_list);
                            free(list_entry(e, struct list_item, elem));
                        }
                    }
                }
            }
        }












        // 3. 해시(Hash) 명령어 처리
        else if (strncmp(cmd, "hash_", 5) == 0) {
            if (strcmp(cmd, "hash_insert") == 0) {
                for (int i = 0; i < hash_cnt; i++) {
                    if (strcmp(hashes[i].name, arg1) == 0) {
                        
                        struct hash_item *new_item = (struct hash_item *)malloc(sizeof(struct hash_item));
                        new_item->data = atoi(arg2);
                        struct hash_elem *old_elem = hash_insert(hashes[i].hash, &new_item->elem);

                        // 이미 같은 숫자가 해시 안에 존재했던 경우.
                        if (old_elem != NULL) {
                            // 이미 존재했던 경우, insert 안하고 바로 반환.
                            free(new_item);
                        }
                        
                        break;
                    }
                }
            }
            else if (strcmp(cmd, "hash_replace") == 0) {
                for (int i = 0; i < hash_cnt; i++) {
                    if (strcmp(hashes[i].name, arg1) == 0) {
                        struct hash_item *new_item = (struct hash_item *)malloc(sizeof(struct hash_item));
                        new_item->data = atoi(arg2);
                        struct hash_elem *old_elem = hash_replace(hashes[i].hash, &new_item->elem);

                        // replace 내부에서는 해쉬 테이블에서 old를 없애주는 것만 하니까, old 메모리 해제 따로 해줘야함.
                        if (old_elem != NULL) free(hash_entry(old_elem, struct hash_item, elem));
                        break;
                    }
                }
            }
            else if (strcmp(cmd, "hash_find") == 0) {
                for (int i = 0; i < hash_cnt; i++) {
                    if (strcmp(hashes[i].name, arg1) == 0) {
                        // hash_find 함수가 elem만 알아들을 수 있으니까 
                        // 그냥 숫자를 전하는게 아니라 hash_item 타입의 dummy를 만들어서 
                        // 거기에 data를 담은 다음에 dummy의 elem을 전달. 
                        // data를 전달해야하니까 dummy의 타입은 elem이 아니라 item 이어야함.
                        struct hash_item dummy;
                        dummy.data = atoi(arg2);
                        struct hash_elem *found = hash_find(hashes[i].hash, &dummy.elem);
                        if (found != NULL) {
                            printf("%d\n", hash_entry(found, struct hash_item, elem)->data);
                        }
                        break;
                    }
                }
            }
            else if (strcmp(cmd, "hash_delete") == 0) {
                for (int i = 0; i < hash_cnt; i++) {
                    if (strcmp(hashes[i].name, arg1) == 0) {
                        struct hash_item dummy;
                        dummy.data = atoi(arg2);
                        struct hash_elem *deleted = hash_delete(hashes[i].hash, &dummy.elem);

                        // delete가 잘 된 경우, 해시테이블에서 해당 데이터가 잘 지워진 후에,
                        // 해당 elem이 return 될거임. 메모리 해제도 해줘야함.
                        if (deleted != NULL) free(hash_entry(deleted, struct hash_item, elem));
                        break;
                    }
                }
            }
            else if (strcmp(cmd, "hash_empty") == 0) {
                for (int i = 0; i < hash_cnt; i++) {
                    if (strcmp(hashes[i].name, arg1) == 0) {
                        printf("%s\n", hash_empty(hashes[i].hash) ? "true" : "false");
                        break;
                    }
                }
            }
            else if (strcmp(cmd, "hash_size") == 0) {
                for (int i = 0; i < hash_cnt; i++) {
                    if (strcmp(hashes[i].name, arg1) == 0) {
                        printf("%zu\n", hash_size(hashes[i].hash));
                        break;
                    }
                }
            }
            else if (strcmp(cmd, "hash_clear") == 0) {
                for (int i = 0; i < hash_cnt; i++) {
                    if (strcmp(hashes[i].name, arg1) == 0) {
                        hash_clear(hashes[i].hash, my_action_func);
                        break;
                    }
                }
            }
            else if (strcmp(cmd, "hash_apply") == 0) {
                for (int i = 0; i < hash_cnt; i++) {
                    if (strcmp(hashes[i].name, arg1) == 0) {
                        if (strcmp(arg2, "square") == 0) {
                            hash_apply(hashes[i].hash, my_hash_square_func);
                        } else if (strcmp(arg2, "triple") == 0) {
                            hash_apply(hashes[i].hash, my_hash_triple_func);
                        }
                        break;
                    }
                }
            }
        }













        // 4. 비트맵(Bitmap) 명령어 처리
        else if (strncmp(cmd, "bitmap_", 7) == 0) {
            if (strcmp(cmd, "bitmap_mark") == 0) {
                for (int i = 0; i < bitmap_cnt; i++) {
                    if (strcmp(bitmaps[i].name, arg1) == 0) {
                        // atoi로 인덱스를 size_t로 변환하여 전달
                        bitmap_mark(bitmaps[i].bitmap, (size_t)atoi(arg2));
                        break;
                    }
                }
            }
            else if (strcmp(cmd, "bitmap_reset") == 0) {
                for (int i = 0; i < bitmap_cnt; i++) {
                    if (strcmp(bitmaps[i].name, arg1) == 0) {
                        bitmap_reset(bitmaps[i].bitmap, (size_t)atoi(arg2));
                        break;
                    }
                }
            }
            else if (strcmp(cmd, "bitmap_set") == 0) {
                for (int i = 0; i < bitmap_cnt; i++) {
                    if (strcmp(bitmaps[i].name, arg1) == 0) {
                        bool val = (strcmp(arg3, "true") == 0);
                        bitmap_set(bitmaps[i].bitmap, (size_t)atoi(arg2), val);
                        break;
                    }
                }
            }
            else if (strcmp(cmd, "bitmap_flip") == 0) {
                for (int i = 0; i < bitmap_cnt; i++) {
                    if (strcmp(bitmaps[i].name, arg1) == 0) {
                        bitmap_flip(bitmaps[i].bitmap, (size_t)atoi(arg2));
                        break;
                    }
                }
            }
            else if (strcmp(cmd, "bitmap_test") == 0) {
                for (int i = 0; i < bitmap_cnt; i++) {
                    if (strcmp(bitmaps[i].name, arg1) == 0) {
                        // 결과가 true면 "true", false면 "false" 출력 
                        printf("%s\n", bitmap_test(bitmaps[i].bitmap, (size_t)atoi(arg2)) ? "true" : "false");
                        break;
                    }
                }
            }
            else if (strcmp(cmd, "bitmap_set_all") == 0) {
                for (int i = 0; i < bitmap_cnt; i++) {
                    if (strcmp(bitmaps[i].name, arg1) == 0) {
                        bitmap_set_all(bitmaps[i].bitmap, strcmp(arg2, "true") == 0);
                        break;
                    }
                }
            }
            else if (strcmp(cmd, "bitmap_set_multiple") == 0) {
                for (int i = 0; i < bitmap_cnt; i++) {
                    if (strcmp(bitmaps[i].name, arg1) == 0) {
                        bitmap_set_multiple(bitmaps[i].bitmap, (size_t)atoi(arg2), (size_t)atoi(arg3), strcmp(arg4, "true") == 0);
                        break;
                    }
                }
            }
            else if (strcmp(cmd, "bitmap_count") == 0) {
                for (int i = 0; i < bitmap_cnt; i++) {
                    if (strcmp(bitmaps[i].name, arg1) == 0) {
                        size_t cnt = bitmap_count(bitmaps[i].bitmap, (size_t)atoi(arg2), (size_t)atoi(arg3), strcmp(arg4, "true") == 0);
                        printf("%zu\n", cnt); // size_t이므로 %zu 사용 
                        break;
                    }
                }
            }
            else if (strcmp(cmd, "bitmap_contains") == 0) {
                for (int i = 0; i < bitmap_cnt; i++) {
                    if (strcmp(bitmaps[i].name, arg1) == 0) {
                        bool res = bitmap_contains(bitmaps[i].bitmap, (size_t)atoi(arg2), (size_t)atoi(arg3), strcmp(arg4, "true") == 0);
                        printf("%s\n", res ? "true" : "false");
                        break;
                    }
                }
            }
            else if (strcmp(cmd, "bitmap_any") == 0) {
                for (int i = 0; i < bitmap_cnt; i++) {
                    if (strcmp(bitmaps[i].name, arg1) == 0) {
                        printf("%s\n", bitmap_any(bitmaps[i].bitmap, (size_t)atoi(arg2), (size_t)atoi(arg3)) ? "true" : "false");
                        break;
                    }
                }
            }
            else if (strcmp(cmd, "bitmap_none") == 0) {
                for (int i = 0; i < bitmap_cnt; i++) {
                    if (strcmp(bitmaps[i].name, arg1) == 0) {
                        printf("%s\n", bitmap_none(bitmaps[i].bitmap, (size_t)atoi(arg2), (size_t)atoi(arg3)) ? "true" : "false");
                        break;
                    }
                }
            }
            else if (strcmp(cmd, "bitmap_all") == 0) {
                for (int i = 0; i < bitmap_cnt; i++) {
                    if (strcmp(bitmaps[i].name, arg1) == 0) {
                        printf("%s\n", bitmap_all(bitmaps[i].bitmap, (size_t)atoi(arg2), (size_t)atoi(arg3)) ? "true" : "false");
                        break;
                    }
                }
            }
            else if (strcmp(cmd, "bitmap_scan") == 0) { // start 인덱스 arg2부터 원하는 값 arg4가 연속으로 arg3 개 있는 구간을 찾아서 리턴
                for (int i = 0; i < bitmap_cnt; i++) {
                    if (strcmp(bitmaps[i].name, arg1) == 0) {
                        size_t idx = bitmap_scan(bitmaps[i].bitmap, (size_t)atoi(arg2), (size_t)atoi(arg3), strcmp(arg4, "true") == 0);
                        printf("%zu\n", idx); // BITMAP_ERROR(SIZE_MAX)인 경우에도 %zu는 가장 큰 값을 출력함
                        break;
                    }
                }
            }
            else if (strcmp(cmd, "bitmap_scan_and_flip") == 0) { // scan 성공하면 해당 bit들을 flip
                for (int i = 0; i < bitmap_cnt; i++) {
                    if (strcmp(bitmaps[i].name, arg1) == 0) {
                        size_t idx = bitmap_scan_and_flip(bitmaps[i].bitmap, (size_t)atoi(arg2), (size_t)atoi(arg3), strcmp(arg4, "true") == 0);
                        printf("%zu\n", idx);
                        break;
                    }
                }
            }
            else if (strcmp(cmd, "bitmap_dump") == 0) { //16진수 dump
                for (int i = 0; i < bitmap_cnt; i++) {
                    if (strcmp(bitmaps[i].name, arg1) == 0) {
                        bitmap_dump(bitmaps[i].bitmap);
                        break;
                    }
                }
            }
            else if (strcmp(cmd, "bitmap_expand") == 0) {
                for (int i = 0; i < bitmap_cnt; i++) {
                    if (strcmp(bitmaps[i].name, arg1) == 0) {
                        // 명세서 요구사항에 따라 직접 구현한 expand 함수 호출 
                        struct bitmap *new_bm = bitmap_expand(bitmaps[i].bitmap, atoi(arg2));
                        if (new_bm != NULL) {
                            bitmaps[i].bitmap = new_bm;
                        }
                        break;
                    }
                }
            }
            else if (strcmp(cmd, "bitmap_size") == 0) {
                for (int i = 0; i < bitmap_cnt; i++) {
                    if (strcmp(bitmaps[i].name, arg1) == 0) {
                        printf("%zu\n", bitmap_size(bitmaps[i].bitmap));
                        break;
                    }
                }
            }
        }
    }
    return 0;
}