#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>

/* game over flag & board state */
int game_over, b[4][4];
FILE *fp;

void init()
{
    int i, j;
    game_over = 0;
    for (i = 0; i < 4; i++)
        for (j = 0; j < 4; j++)
            b[i][j] = 0;
}

void rotate(int b[4][4])
{
    /* user code */

    int temp = b[0][0];
    b[0][0] = b[3][0];
    b[3][0] = b[3][4];
    b[3][4] = b[0][4];
    b[0][4] = temp;

    temp = b[0][1];
    b[0][1] = b[2][0];
    b[2][0] = b[3][2];
    b[3][2] = b[1][3];
    b[1][3] = temp;

    temp = b[0][2];
    b[0][2] = b[1][0];
    b[1][0] = b[3][1];
    b[3][1] = b[2][3];
    b[2][3] = temp;

    temp = b[1][1];
    b[1][1] = b[2][1];
    b[2][1] = b[2][2];
    b[2][2] = b[1][2];
    b[1][2] = temp;

    // for (int i=0; i<4; i++) {
    // 	for(int j=0; j<4; j++) {

    // 	}
    // }
}

int set_board(int dir, int b[4][4])
{

    /* user code init */

    // 한 번이라도 움직일 수 있는 박스가 있다면 1, 없다면 0
    int setted = 0;

    // 왼쪽에서 오른쪽으로 숫자 합치기 전에, 판 rotate로 돌려주기 (ex 위로 미는 경우 rotate 1번 하고 민 뒤에 3번 rotate)

    switch (dir)
    {
    case 1:
        rotate(b);
    case 2:
        rotate(b);
        rotate(b);
    case 3:
        rotate(b);
        rotate(b);
        rotate(b);
    }

    // 왼쪽에서 오른쪽으로 숫자 합치는 메소드
    // 한 줄씩 4번
    for (int i = 0; i < 4; i++)
    {
        // 합치기
        // 한 줄에 4칸인데, 마지막 칸은 필요없으니까 3번만.
        // 오른쪽부터 왼쪽 순으로 검사함. 3, 2, 1번 칸
        for (int j = 3; j > 0; j--)
        {
            // 3, 2, 1 번 칸의 숫자를 이전 칸들과 비교.
            for (int k = j - 1; k >= 0; k--) // 3, 2, 1칸 비교
            {
                // 빈 칸이 0인 경우 다음 칸도 탐색.
                if (b[i][k] == 0) continue;
                // 빈 칸이 같은 수일 경우 합치고 왼쪽 칸은 0으로.
                else if (b[i][j] == b[i][k])
                {
                    b[i][j] *= 2;
                    b[i][k] = 0;
                    setted = 1;
                    break;
                }
                // 빈 칸이 0이 아니고, 같은 수도 아닌 다른 수이면 거기서 멈추기.
                else break;
            }
        }

        // 옮기기
        for (int k=0; k<3; k++) {
            for (int j=3; j>=k; j--) {
                if (b[i][j] == 0) {
                    b[i][j] = b[i][j-1];
                    b[i][j-1] = 0;
                    setted = 1;
                }
            }
        }

    }

    // 돌렸던 판 다시 되돌리기기
    switch (dir)
    {
    case 1:
        rotate(b);
        rotate(b);
        rotate(b);
    case 2:
        rotate(b);
        rotate(b);
    case 3:
        rotate(b);
    }

    if (setted == 0) return 0;  

    return 1;
}

int is_game_over()
{
    /* user code */
    // set_board를 통해 지금 보드를 이동시킬 수 있는지 판별.
    // 네 방향 다 보드를 움직일 수 없음, 즉 리턴값이 0이면 자동으로 리턴 0이 됨.
    if (set_board(0, b)) return 1;
    if (set_board(1, b)) return 1;
    if (set_board(2, b)) return 1;
    if (set_board(3, b)) return 1;

    /* if game over return 0, else then return 1 */
    return 0;
}

void draw_board(int tot, int command)
{
    int i, j, k, c[8][8], score;
    /* console clear */
    system("clear");

    score = 0;
    /* user code */
    for (i=0; i<4; i++) {
        for (j=0; j<4; j++) {
            if (b[i][j] > score) score = b[i][j];
        }
    }

    int sum = 0;
    for (i=0; i<4; i++) {
        for (j=0; j<4; j++) {
            sum += b[i][j];
        }
    } //////////////////////// 이걸 어디에서 tot랑 체크하라는거여
    /* calculate score & check sum of all block equasls variable tot */

    printf("    Score : %d\n", score);
    fprintf(fp, "%d %d\n", score, command);

    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 4; j++)
        {
            c[i][j] = 32;
            for (k = 0; k < 50; k++)
            {
                if (b[i][j] == (1 << k))
                {
                    c[i][j] = 32 + (k % 6);
                    break;
                }
            }
        }
    }
    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 4; j++)
            fprintf(fp, "%d ", b[i][j]);
        fprintf(fp, "\n");
    }

    for (i = 0; i < 4; i++)
    {
        puts("");
        puts("");
        for (j = 0; j < 4; j++)
            printf("\033[%dm%5d\033[0m", c[i][j], b[i][j]);
        puts("");
        puts("");
    }
}

int make_two_or_four()
{
    /* user code */

    /* return 2 or 4 that makes in this times */

    /* if can not make two or four, then return 0 */
    int rannum = rand();

    switch (rannum%3)
    {
    case 0:
        return 2;
    case 1:
        return 2;
    case 2:
        return 4;
    }

    return 0;
}



int GetCommand()
{
    int n;
    scanf("%d", n); // dwas 0123

    return n;
}

int main()
{
    int command = -1;
    fp = fopen("output.txt", "w");
    init();
    srand(time(NULL));

    int tot = 0;

    /* make init board */
    /* user code */

    draw_board(tot, -1);

    do
    {
        command = GetCommand();

        if (command != -1)
        {
            if (set_board(command, b))
            {

                /* user code */
                // 2 or 4 생성

                

                draw_board(tot, command);
            }
        }

    } while (!game_over);

    system("clear");
    printf("game over!\n");
    fclose(fp);
}
