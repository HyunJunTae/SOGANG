#include <stdio.h>
#include <stdlib.h>
#include <time.h>


int count = 1;
int N;
int M;

char **maze; // 미로
int *set_id;           // 셀의 집합 ID
int *set_id_2; // 그 아래의 셀의 집합


// 초기 미로 생성.
void initialize_maze() {
    for (int i = 0; i < 2*M+1; i++) {
        for (int j = 0; j < 2*N+1; j++) {
            if (i % 2 == 0 && j % 2 == 0) maze[i][j] = '+';
            else if (i % 2 == 0) maze[i][j] = '-';
            else if (j % 2 == 0) maze[i][j] = '|';
            else maze[i][j] = ' ';
        }
    }
}

// Eller 알고리즘
void generate_eller_maze() {
    srand(time(NULL));
    initialize_maze();

    // M은 미로의 높이. 한 줄씩 실행하고 미로의 높이만큼 실행
    for (int row = 0; row < M; row++) {

        // 1. set_id 초기화.
        // 만약 이미 집합값이 들어있다면 그대로 두고, -1이 들어있다면 새로운 집합값을 넣어주기
        for (int i = 0; i < N; i++) {
            if (set_id[i] == -1)
                set_id[i] = count++;
        }

        // 2. 현재 행에서, 랜덤으로 벽 부시기
        for (int i=0; i<N-1; i++){
            // 현재 칸과 다음 칸의 집합이 다르면, 랜덤으로 벽 부수기.
            if (set_id[i] != set_id[i+1] && rand() % 2 == 0) {
                maze[2*row+1][2*i+2] = ' ';
                set_id[i+1] = set_id[i];
            }
        }

        // 만약 마지막 행이라면, 아래 줄 부시기는 생략
        if (row == M-1) break;
        
        // 3. 현재 행과 그 아래 다음 행 사이의 벽 제거.
        //    만약 제거되면, 해당 벽은 위의 벽과 같은 집합.
        int wall_deleted = 0;
        for( int i=0; i<N-1; i++) {

            
            // 만약 이번 집합에서 부숴진 아래쪽 벽이 없는데,
            // 다음 칸의 집합이 이번 칸의 집합과 다르다면, 무조건 부수기.
            if (set_id[i] != set_id[i+1] && wall_deleted == 0) {
                maze[2*row+2][2*i+1] = ' ';
                set_id_2[i] = set_id[i];
            }

            // 랜덤으로 아래쪽 벽 제거
            else if (rand() % 2 == 0) {
                maze[2*row+2][2*i+1] = ' ';
                set_id_2[i] = set_id[i];
                wall_deleted = 1;
            }

            // 새로운 집합으로 넘어가면, wall_deleted를 0으로 재설정
            if (set_id[i]!= set_id[i+1]) {
                wall_deleted = 0;
            }
            
        } 
        
        // 3.2 행의 마지막 칸의 경우는 따로 계산.
        // 만약 맨 마지막 칸이 이전 칸이랑 다른 집합인데, 부숴지지 않았다면, 무조건 부수기.
        if (set_id[N-2]!= set_id[N-1] && wall_deleted == 0) {
            maze[2*row+2][2*(N-1)+1] =' ';
            set_id_2[N-1] = set_id[N-1];
        }
        // 랜덤으로 벽 부시기.
        else if (rand() % 2 == 0) {
            maze[2*row+2][2*(N-1)+1] = ' ';
            set_id_2[N-1] = set_id[N-1];
            wall_deleted = 1;
        }

        // 4. set_id_2의 값을 set_id에 복사.
        //    set_id_2 는 -1로 초기화
        for (int i=0; i<N; i++) {
            set_id[i] = set_id_2[i];
            set_id_2[i] = -1;
        }
    }
}

// // 미로 출력
// void print_maze() {
//     for (int i = 0; i < 2*M+1; i++) {
//         for (int j = 0; j < 2*N+1; j++)
//             printf("%c", maze[i][j]);
//         printf("\n");
//     }
// }

// 미로 출력
void print_maze() {
    FILE *file = fopen("mymaze123.maz", "w");
    if (file == NULL) {
        perror("Failed to open file");
        return;
    }
    for (int i = 0; i < 2*M+1; i++) {
        for (int j = 0; j < 2*N+1; j++)
            fprintf(file, "%c", maze[i][j]);
        fprintf(file, "\n");
    }
    fclose(file);
}

int main() {
    
    printf("N input : ");
    scanf("%d", &N);
    printf("M input : ");
    scanf("%d", &M);

    // 메모리 할당 및 set의 초기값 설정
    maze = (char**)malloc((2*M+1) * sizeof(char*));
    for (int i = 0; i < 2*M+1; i++) {
        maze[i] = (char*)malloc((2*N+1) * sizeof(char));
    }
    set_id = (int*)malloc(N * sizeof(int));
    set_id_2 = (int*)malloc(N * sizeof(int));
    for (int i = 0; i < N; i++) {
        set_id[i] = -1;
        set_id_2[i] = -1;
    }

    generate_eller_maze();
    print_maze();


    // 메모리 해제
    for (int i = 0; i < 2*M+1; i++) {
        free(maze[i]);
    }
    free(maze);
    free(set_id);
    free(set_id_2);

    return 0;
}


// 개선점
// 필드를 배열에 저장해둔 다음에 출력하는게 아니라, 한 줄 만들고 set_id 배열 사용해서 바로 그리는 식으로. 메모리 줄이기.