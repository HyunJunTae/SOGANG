#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 3 // 미로의 너비
#define M 5 // 미로의 높이

char maze[2*M+1][2*N+1]; // 미로
int set_id[N];           // 셀의 집합 ID
int rank[N];             // Union-Find 랭크
int set_to_cells[N][N];  // 집합별 셀 리스트
int set_count[N];        // 집합별 셀 수

// Union-Find: Find 연산
int find(int cell) {

    // 만약 세트가 1번 세트인데, 집합이 0에 속해있다면, 0을 리턴해아함.
    if (set_id[cell] != cell)
        set_id[cell] = find(set_id[cell]);  
    return set_id[cell];
}

// Union-Find: Union 연산
void union_sets(int cell1, int cell2) {
    // 두 셀의 집합 번호 찾기.
    int root1 = find(cell1), root2 = find(cell2);
    if (root1 != root2) {
        if (rank[root1] < rank[root2]) {
            int temp = root1; root1 = root2; root2 = temp;
        }
        set_id[root2] = root1;
        if (rank[root1] == rank[root2]) rank[root1]++;
        // 집합별 셀 리스트 갱신
        for (int i = 0; i < set_count[root2]; i++)
            set_to_cells[root1][set_count[root1]++] = set_to_cells[root2][i];
        set_count[root2] = 0;
    }
}

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

// 집합 초기화
void initialize_sets() {
    for (int i = 0; i < N; i++) {
        set_id[i] = i;
        rank[i] = 0;
        set_count[i] = 1;
        set_to_cells[i][0] = i;
    }
}

// Eller 알고리즘
void generate_eller_maze() {
    srand(time(NULL));
    initialize_maze();

    // M은 미로의 높이. 한 줄씩 실행하고 미로의 높이만큼 실행
    for (int row = 0; row < M; row++) {
        // 1. 첫 행 초기화 또는 이전 행의 집합 상속
        initialize_sets();
        
        // 2. 인접 셀 간 벽 제거
        for (int col = 0; col < N-1; col++) {
            // 만약 set_id[col]과 set_id[col+1]이 다르면, 랜덤하게 벽 제거.
            if (find(col) != find(col+1) && (rand() % 2)) {
                maze[2*row+1][2*col+2] = ' ';
                union_sets(col, col+1);
            }
        }
        
        // 3. 다음 행으로 수직 연결
        if (row < M-1) {
            int connected[N] = {0}; // 연결된 셀 추적
            for (int set = 0; set < N; set++) {
                if (set_count[set] > 0) {
                    // 최소 하나 이상 연결
                    int idx = rand() % set_count[set];
                    int cell = set_to_cells[set][idx];
                    maze[2*row+2][2*cell+1] = ' ';
                    connected[cell] = 1;
                }
            }
            // 4. 다음 행의 나머지 셀 초기화
            for (int col = 0; col < N; col++) {
                if (!connected[col]) {
                    set_id[col] = col;
                    set_count[col] = 1;
                    set_to_cells[col][0] = col;
                }
            }
        }
        
        // 5. 마지막 행: 모든 다른 집합 연결
        if (row == M-1) {
            for (int col = 0; col < N-1; col++) {
                if (find(col) != find(col+1)) {
                    maze[2*row+1][2*col+2] = ' ';
                    union_sets(col, col+1);
                }
            }
        }
    }
}

// 미로 출력
void print_maze() {
    for (int i = 0; i < 2*M+1; i++) {
        for (int j = 0; j < 2*N+1; j++)
            printf("%c", maze[i][j]);
        printf("\n");
    }
}

int main() {
    generate_eller_maze();
    print_maze();
    return 0;
}