#include <stdio.h>
#define MAX_QUEUE_SIZE 100
#define FALSE 0
#define TRUE 1

typedef struct
{
    short int row;
    short int col;
    short int dir;
} element;

typedef struct
{
    short int vert;
    short int horiz;
} offsets;

void init_move(offsets *move)
{ /* initial the table for the next row and column moves */
    move[1].vert = -1;
    move[1].horiz = 0; /* N */
    move[2].vert = -1;
    move[2].horiz = 1; /* NE */
    move[3].vert = 0;
    move[3].horiz = 1; /* E */
    move[4].vert = 1;
    move[4].horiz = 1; /* SE */
    move[5].vert = 1;
    move[5].horiz = 1; /* S */
    move[6].vert = 1;
    move[6].horiz = 0; /* SW */
    move[7].vert = 0;
    move[7].horiz = -1; /* W */
    move[8].vert = -1;
    move[8].horiz = -1; /* NW */
}

void queue_full()
{
    printf("QUEUE IS FULL \n");
}

void queue_empty()
{
    printf("QUEUE IS EMPTY \n");
}

void add(int *front, int *rear, element item, element *queue)
{
    *rear = (*rear + 1) % MAX_QUEUE_SIZE;
    if (*front == *rear)
        queue_full();
    else
        queue[*rear] = item;
}

element delete(int *front, int *rear, element *queue)
{
    if (*front == *rear)
        queue_empty();
    else
    {
        *front = (*front + 1) % MAX_QUEUE_SIZE;
        return queue[*front];
    }
}



int main(void)
{

    // maze array 생성
    FILE *inputFile, *outputFile;

    // input.txt 파일 열기
    inputFile = fopen("input.txt", "r");
    if (inputFile == NULL)
    {
        printf("No Input File.\n");
        return 1;
    }

    // input.txt파일에서 행과 열 개수 읽기
    int rows, cols;
    fscanf(inputFile, "%d %d", &rows, &cols);

    // 테두리 1로 해야하니까 2씩 더 크게.
    int maze[rows + 2][cols + 2];

    // 위아래쪽 테두리 1로 설정
    for (int i = 0; i < cols + 2; i++)
    {
        maze[0][i] = 1;
        maze[rows + 1][i] = 1;
    }

    // 왼오른쪽 테두리 1로 설정
    for (int i = 0; i < rows + 2; i++)
    {
        maze[i][0] = 1;
        maze[i][cols + 1] = 1;
    }

    // maze에 maze 데이터 읽어서 저장
    for (int i = 1; i <= rows; i++)
    {
        for (int j = 1; j <= cols; j++)
        {
            fscanf(inputFile, "%d", &maze[i][j]);
        }
    }

    // 입력 파일 닫기
    fclose(inputFile);





    // mark array 생성
    int mark[rows + 2][cols + 2];

    // 다 0으로 채워주기
    for (int i = 0; i < rows + 2; i++)
    {
        for (int j = 1; j < cols + 2; j++)
        {
            mark[i][j] = 0;
        }
    }




    // element를 담는 queue 생성
    element queue[MAX_QUEUE_SIZE];
    // offsets를 담는 move 배열 생성
    offsets move[9]; /* array of moves for each direction */
    init_move(move);




    // path 함수 따로 안만들고 main에서 바로 실행
    /* output a path through the maze if such a path exists */
    int i, row, col, nextRow, nextCol, dir, found = FALSE;
    element position;
    mark[1][1] = 1;
    int front = 0, rear = -1;
    queue[0].row = 1;
    queue[0].col = 1;
    queue[0].dir = 1;
    int exit_row = rows+1;
    int exit_col = cols+1;
    while (front != rear && !found)
    {
        position = delete(&front, &rear, queue);
        row = position.row;
        col = position.col;
        dir = position.dir;
        while (dir < 8 && !found)
        {
            /* move in direction dir */
            nextRow = row + move[dir].vert;
            nextCol = col + move[dir].horiz;

            if (nextRow == exit_row && nextCol == exit_col)
                found = TRUE;

            else if (!maze[nextRow][nextCol] && !mark[nextRow][nextCol])
            {
                mark[nextRow][nextCol] = 1;

                position.row = row;
                position.col = col;
                position.dir = ++dir;

                add(&front, &rear, position, queue);

                row = nextRow;
                col = nextCol;
                dir = 0;
            }

            else
                ++dir;
        }
    }

    if (found)
    {
        printf("The path is : \n");
        printf("row col \n");
        for (i = 0; i <= top; i++)
            printf("% 2d % 5d", queue[i].row, queue[i].col);
        printf("% 2d % 5d\n", row, col);
        printf("% 2d % 5d\n", EXIT_ROW, EXIT_COL);
    }
    else
        printf("The maze does not have a path \n");

    return 0;
}