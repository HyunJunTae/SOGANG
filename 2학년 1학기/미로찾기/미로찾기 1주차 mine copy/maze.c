#include <stdio.h>
#include <stdlib.h>
#include <time.h>


int count = 1;
int N;
int M;

int *set_id;           // 셀의 집합 ID
int *set_id_2; // 그 아래의 셀의 집합


void merge(int i) {
    // temp 는 더 작은 값, change_num은 더 큰 값
    int temp, change_num;
    // 더 작은 값을 저장
    if (set_id[i + 1] > set_id[i]) {
        temp = set_id[i];
        change_num = set_id[i + 1];
    }
    else {
        temp = set_id[i + 1];
        change_num = set_id[i];
    }

    // 현재 행에서 change_num을 가지는 모든 집합값을 temp로 바꾸기
    for (int j = 0; j < N; j++) {
        if (set_id[j] == change_num) {
            set_id[j] = temp;
        }
    }
}


// Eller 알고리즘
void generate_eller_maze() {

    FILE *file = fopen("20221627.maz", "w");


    srand(time(NULL));

    // 미로의 맨 윗줄 그려주기.
    for (int i=0; i<N; i++) {
        fprintf(file, "%s", "+-");
    }
    fprintf(file, "%s", "+\n"); 



    // M은 미로의 높이. 한 줄씩 실행하고 미로의 높이만큼 실행
    for (int row = 0; row < M; row++) {

        // 1. set_id 초기화.
        // 만약 이미 집합값이 들어있다면 그대로 두고, -1이 들어있다면 새로운 집합값을 넣어주기
        for (int i = 0; i < N; i++) {
            if (set_id[i] == -1)
                set_id[i] = count++;
        }






        // 2. 현재 행에서, 랜덤으로 빈 벽 출력하기
        // 맨 마지막 방은 나중에 따로 출력

        // 마지막 row의 경우에는, 다른 집합이면 무조건 합치기
        if (row == M-1) {
            fprintf(file, "%s", "|"); // 일단 맨 왼쪽 벽은 출력
            for (int i=0; i<N-1; i++){
                fprintf(file, "%s", " "); // 방 한 칸 출력

                if (set_id[i] != set_id[i+1]) { // 현재 칸과 다음 칸의 집합이 다르면, 무조건 벽 뚫기
                    fprintf(file, "%s", " ");
                    merge(i);
                }
                
                else if (set_id[i] == set_id[i+1]) { // 현재 칸과 다음 칸의 집합이 같으면, 안뚫기
                    fprintf(file, "%s", "|");
                }
            }
            fprintf(file, "%s", " |\n"); // 맨 마지막 방 출력
            break;
        }

        // 마지막 row가 아닌 경우
        fprintf(file, "%s", "|"); // 일단 맨 왼쪽 벽은 출력
        for (int i=0; i<N-1; i++){

            fprintf(file, "%s", " "); // 방 한 칸 출력

            if (set_id[i] == set_id[i+1]) {// 현재 칸과 다음 칸의 집합이 같으면, 벽 만들기
                fprintf(file, "%s", "|");
            }
            
            else if (rand() % 2 == 0) { // 현재 칸과 다음 칸 집합이 다르면, 랜덤으로 빈칸 만들기
                fprintf(file, "%s", " ");
                merge(i);

            }
            else { // 랜덤 실패했다면 벽 만들기
                fprintf(file, "%s","|");
            }
        }
        fprintf(file, "%s", " |\n"); // 마지막 방 출력





        // // 3. 현재 행과 그 아래 다음 행 사이의 벽 제거.
        int same_set_number_exist = 0;
        fprintf(file, "%s", "+");
        int wall_already_deleted = 0;
        for( int i=0; i<N; i++) {

            // 이번 칸의 집합값이 이후에 있는지 검사
            same_set_number_exist = 0;
            for (int j=i+1; j<N; j++) {
                if (set_id[i] == set_id[j]) {
                    same_set_number_exist = 1;
                    break;
                }
            }
            // 이번 칸의 집합값에 대해서, 이번 행에 뚫린 아래칸이 있는지 검사.
            wall_already_deleted = 0;
            for (int j=0; j<N; j++) {
                if (set_id_2[j] == set_id[i]) {
                    wall_already_deleted = 1;
                    break;
                }
            }

            // 이번 칸의 집합값이 이후에 없고, 아직 해당 집합값에 대해 안뚫렸다면, 무조건 뚫기
            if (same_set_number_exist == 0 && wall_already_deleted == 0) {
                fprintf(file, "%s", " +");
                set_id_2[i] = set_id[i];
            }
            // 이번 칸의 집합값이 이후에 또 있는 경우,
            // 이번 칸의 집합값이 이후에 없지면 해당 집합값에 대해 이미 뚫린 경우,
            // 무조건 뚫을 필요 없음
            else if (rand() % 2 == 0) { // 따라서 랜덤으로 아랫쪽 벽 제거
                fprintf(file, "%s", " +");
                set_id_2[i] = set_id[i];
            }
            // 랜덤으로 못 부쉈으면 벽 출력
            else {
                fprintf(file, "%s", "-+");
            }
            
        }
        fprintf(file, "%s", "\n");




        // 4. set_id_2의 값을 set_id에 복사.
        //    set_id_2 는 -1로 초기화
        for (int i=0; i<N; i++) {
            set_id[i] = set_id_2[i];
            set_id_2[i] = -1;
        }
    }

    // 미로의 맨 아랫줄 그려주기.
    for (int i=0; i<N; i++) {
        fprintf(file, "%s", "+-");
    }
    fprintf(file, "%s", "+\n");

}

int main() {
    
    printf("N input : ");
    scanf("%d", &N);
    printf("M input : ");
    scanf("%d", &M);


    // 메모리 할당 및 set의 초기값 설정
    set_id = (int*)malloc(N * sizeof(int));
    set_id_2 = (int*)malloc(N * sizeof(int));
    for (int i = 0; i < N; i++) {
        set_id[i] = -1;
        set_id_2[i] = -1;
    }

    generate_eller_maze();


    // 메모리 해제
    free(set_id);
    free(set_id_2);

    return 0;
}



// 왜 안됐는가
// 1. 가로로 벽 뚫을 때, 집합값을 작은 집합값으로 저장해야 함.
//    - 두 집합값을 비교해서, 더 작은 집합값이 저장되도록 설정.

// 2. 좌우 칸만 비교하는게 아니라, 행 단위로 봐야함
//    - 아래쪽 벽 뚫을 때, 현재칸과 다음칸 다르고, 아직 안뚫렸으면, 무조건 뚫기. 가 아님.
//      그렇게 하는 경우, 1 3 1 3 1 이면, 계속 무조건 뚫림.
//      (현재 집합값이 이번 행에서 마지막으로 보였고,) (현재 집합값에 대해 아직 뚫리지 않은 경우) 를 고려해야함
//      -> set_id 행을 돌면서 검사                -> set_id_2 행을 돌면서 검사