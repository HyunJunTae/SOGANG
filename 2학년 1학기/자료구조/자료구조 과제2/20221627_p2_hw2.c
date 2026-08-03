#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    short int row;
    short int col;
    short int prev_row;
    short int prev_col;
} element;

typedef struct
{
    short int vert;
    short int horiz;
} offsets;

void init_move(offsets *move)
{
    move[1].vert = -1;
    move[1].horiz = 0; // N
    move[2].vert = -1;
    move[2].horiz = 1; // NE
    move[3].vert = 0;
    move[3].horiz = 1; // E
    move[4].vert = 1;
    move[4].horiz = 1; // SE
    move[5].vert = 1;
    move[5].horiz = 0; // S
    move[6].vert = 1;
    move[6].horiz = -1; // SW
    move[7].vert = 0;
    move[7].horiz = -1; // W
    move[8].vert = -1;
    move[8].horiz = -1; // NW
}

void queue_full()
{
    printf("QUEUE IS FULL \n");
}

void queue_empty()
{
    printf("QUEUE IS EMPTY \n");
}

int IsFull(int front, int rear)
{
    if (front == rear)
        return 1;
    return 0;
}

int IsEmpty(int front, int rear)
{
    if (front == rear)
        return 1;
    return 0;
}

void add(int *front, int *rear, element item, element *queue, int MAX_QUEUE_SIZE)
{
    if (IsFull(*front, *rear+1))
    queue_full();
    else {
        *rear = (*rear + 1) % MAX_QUEUE_SIZE;
        queue[*rear] = item;
    }
}

element delete(int *front, int *rear, element *queue, int MAX_QUEUE_SIZE)
{
    if (IsEmpty(*front, *rear))
    {
        queue_empty();
        element empty = {-1, -1, -1, -1};
        return empty;
    }
    *front = (*front + 1) % MAX_QUEUE_SIZE;
    return queue[*front];
}

int main(void)
{

    // input.txt 파일 읽기
    FILE *inputFile = fopen("input.txt", "r");
    if (inputFile == NULL)
    {
        printf("No Input File.\n");
        return 1;
    }

    int MAX_QUEUE_SIZE = 100;

    int rows, cols;
    fscanf(inputFile, "%d %d", &rows, &cols);

    // int maze[rows + 2][cols + 2];
    // 1. maze 동적 할당
    int **maze = (int **)malloc((rows + 2) * sizeof(int *));
    for (int i = 0; i < rows + 2; i++)
    {
        maze[i] = (int *)malloc((cols + 2) * sizeof(int));
    }

    // 테두리 1로 설정
    for (int i = 0; i < cols + 2; i++)
    {
        maze[0][i] = 1;
        maze[rows + 1][i] = 1;
    }

    for (int i = 0; i < rows + 2; i++)
    {
        maze[i][0] = 1;
        maze[i][cols + 1] = 1;
    }

    // 미로 데이터 읽기
    for (int i = 1; i <= rows; i++)
    {
        for (int j = 1; j <= cols; j++)
        {
            fscanf(inputFile, "%d", &maze[i][j]);
        }
    }
    fclose(inputFile);


    // 이미 갔던 길 표시할 mark 배열
    // int mark[rows + 2][cols + 2];
    // 2. mark 동적 할당
    int **mark = (int **)malloc((rows + 2) * sizeof(int *));
    for (int i = 0; i < rows + 2; i++)
    {
        mark[i] = (int *)malloc((cols + 2) * sizeof(int));
    }

    for (int i = 0; i < rows + 2; i++)
    {
        for (int j = 0; j < cols + 2; j++)
        {
            mark[i][j] = 0;
        }
    }


    // element queue[MAX_QUEUE_SIZE];
    // queue 동적 할당
    element *queue = (element*)malloc(MAX_QUEUE_SIZE * sizeof(element));
    
    // offsets move[9];
    offsets *move = (offsets*)malloc(9 * sizeof(offsets));
    init_move(move);

    // BFS 초기화
    int front = 0, rear = 0;
    element start = {1, 1, -1, -1}; // 출발점 (1,1), 이전 노드 없음
    mark[1][1] = 1;
    add(&front, &rear, start, queue, MAX_QUEUE_SIZE);

    int exit_row = rows, exit_col = cols;
    int found = 0;
    element position;


    // BFS
    while (front != rear && !found)
    {
        position = delete(&front, &rear, queue, MAX_QUEUE_SIZE);
        int row = position.row;
        int col = position.col;


        // 모든 방향을 탐색
        for (int dir = 1; dir < 9 && !found; dir++)
        {
            int nextRow = row + move[dir].vert;
            int nextCol = col + move[dir].horiz;


            // 도착점 발견
            if (nextRow == exit_row && nextCol == exit_col)
            {
                found = 1;
                position.row = nextRow;
                position.col = nextCol;
                position.prev_row = row;
                position.prev_col = col;
                add(&front, &rear, position, queue, MAX_QUEUE_SIZE);
                break;
            }

            // 유효한 경로 (벽이 아니고, 방문하지 않은 곳)
            else if (!maze[nextRow][nextCol] && !mark[nextRow][nextCol])
            {
                mark[nextRow][nextCol] = 1;

                element new_position = {nextRow, nextCol, row, col};
                add(&front, &rear, new_position, queue, MAX_QUEUE_SIZE);
            }

        }
    }


    if (found)
    {
        printf("The path is : \n");
        printf("row col\n");
        // element path[MAX_QUEUE_SIZE];
        // path 동적 할당
        element *path = (element *)malloc(MAX_QUEUE_SIZE * sizeof(element));
        int path_index = 0;

        // 경로 추적

        // 탈출 지점부터 시작
        int row = position.prev_row;
        int col = position.prev_col;

        // 큐를 역순으로 돌며 이전 위치 추적
        while (row != -1 && col != -1)
        {
            for (int i = rear; i >= 0; i--)
            {
                if (queue[i].row == row && queue[i].col == col)
                {
                    path[path_index].row = queue[i].row;
                    path[path_index++].col = queue[i].col;
                    row = queue[i].prev_row;
                    col = queue[i].prev_col;
                    break;
                }
            }
        }

        for (int i = path_index - 1; i >= 0; i--)
        {
            printf(" %d  %d\n", path[i].row, path[i].col);
        }

        printf(" %d  %d\n", position.row, position.col);

        free(path);
    }
    else
    {
        printf("The maze does not have a path \n");
    }


    // 메모리 해제
    for (int i = 0; i < rows + 2; i++)
    {
        free(maze[i]);
        free(mark[i]);
    }
    free(maze);
    free(mark);
    free(move);
    free(queue);

    return 0;
}
