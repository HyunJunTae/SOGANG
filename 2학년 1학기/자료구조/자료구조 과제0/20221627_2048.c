#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <termios.h>
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
    b[3][0] = b[3][3];
    b[3][3] = b[0][3];
    b[0][3] = temp;

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
}

int set_board(int dir, int b[4][4])
{

    /* user code init */

    // 전후 차이가 있는지 비교를 위해 보드 c 생성.
    int c[4][4];

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            c[i][j] = b[i][j];
        }
    }

    // 왼쪽에서 오른쪽으로 숫자 합치기 전에, 판 rotate로 돌려주기 (ex 위로 미는 경우 rotate 1번 하고 민 뒤에 3번 rotate)
    switch (dir)
    {
    case 1:
        rotate(c);
        break;
    case 2:
        rotate(c);
        rotate(c);
        break;
    case 3:
        rotate(c);
        rotate(c);
        rotate(c);
        break;
    default:
        break;
    }

    // row 하나씩 4번 실행
    for (int i = 0; i < 4; i++)
    {
        // 오른쪽으로 숫자 쭉 밀고
        for (int j = 2; j >= 0; j--)
        {
            for (int k = j; k >= 0; k--)
            {
                if (c[i][k] != 0 && c[i][k + 1] == 0)
                {
                    c[i][k + 1] = c[i][k];
                    c[i][k] = 0;
                }
            }
        }

        // 오른쪽부터 숫자 합치고
        for (int j = 2; j >= 0; j--)
        {
            if (c[i][j] == c[i][j + 1])
            {
                c[i][j + 1] *= 2;
                c[i][j] = 0;
            }
        }

        // 다시 오른쪽으로 쭉 밀기
        for (int j = 2; j >= 0; j--)
        {
            for (int k = j; k >= 0; k--)
            {
                if (c[i][k] != 0 && c[i][k + 1] == 0)
                {
                    c[i][k + 1] = c[i][k];
                    c[i][k] = 0;
                }
            }
        }
    }

    // 돌렸던 판 다시 되돌리고
    switch (dir)
    {
    case 1:
        rotate(c);
        rotate(c);
        rotate(c);
        break;
    case 2:
        rotate(c);
        rotate(c);
        break;
    case 3:
        rotate(c);
        break;
    default:
        break;
    }

    int iscorrect = 0;
    // b와 c가 다른지 검사. 다른 부분이 하나라도 있다면 iscorrect = 1 -> 이 방향으로 이동 가능
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (c[i][j] != b[i][j])
            {
                iscorrect = 1;
                break;
            }
        }
        if (iscorrect)
            break;
    }

    // 만약 변경점이 있다면,
    if (iscorrect == 1)
    {
        // 이동 반영된 c를 기존 배열 b에 복사
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                b[i][j] = c[i][j];
            }
        }
    }
    // 변경점이 없다면, 즉 잘못된 이동이라면 return 0;
    else
        return 0;

    return 1;
}

int is_game_over()
{
    /* user code */
    // 4방향으로 이동이 가능한지 검사.
    // 4방향 검사를 위해 보드를 a c d e 4개로 복사하고, 각 보드를 wasd 각 방향으로 set_board 시켜셔 검사.
    // set_board의 리턴값이 1이면 해당 방향으로 이동 가능하다는 것.
    // 4개 리턴값 모두 0이면 더 이상 이동 불가하다는 것.
    int a[4][4];

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            a[i][j] = b[i][j];
        }
    }
    int c[4][4];

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            c[i][j] = b[i][j];
        }
    }
    int d[4][4];

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            d[i][j] = b[i][j];
        }
    }
    int e[4][4];

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            e[i][j] = b[i][j];
        }
    }

    // 각각의 방향으로 검사하고, 만약 하나의 방향으로라도 이동 가능하다면,
    // set_board의 리턴값이 1일 것이고, 따라서 1이 리턴될 것이다 -> 게임 오버가 아님을 의미
    // 게임 오버가 아니면 0을 리턴
    if (set_board(0, a))
        return 0;
    else if (set_board(1, c))
        return 0;
    else if (set_board(2, d))
        return 0;
    else if (set_board(3, e))
        return 0;

    // 위의 4가지 경우 다 이동 불가인 경우 return 0 -> 게임 오버
    /* if game over return 0, else then return 1 */
    return 1;
}

void draw_board(int tot, int command)
{
    int i, j, k, c[8][8], score;
    /* console clear */
    system("clear");

    score = 0;
    /* user code */
    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 4; j++)
        {
            if (b[i][j] > score)
                score = b[i][j];
        }
    }

    int sum = 0;
    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 4; j++)
        {
            sum += b[i][j];
        }
    }

    // 만약 sum과 tot이 다르다면 새로 그리지 않고 return
    if (sum != tot)
        return;
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

    // 새로운 숫자 생성할 수 있는지 검사. 못만들면 0 만들수있으면 1
    int can_make_new_num = 0;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (b[i][j] == 0)
            {
                can_make_new_num = 1;
                break;
            }
        }
        if (can_make_new_num == 1)
            break;
    }

    // 새로운 2또는4 못만들면 return 0
    if (can_make_new_num == 0)
        return 0;

    srand(time(NULL));
    int rannum = rand();
    // thisnum 변수에 2 또는 4 생성.
    int thisnum;

    // 2 또는 4를 랜덤으로 생성성
    // 랜덤 변수를 3으로 나눠서 나머지가 0 1 이면 2 / 2 면 4
    switch (rannum % 3)
    {
    case 0:
        thisnum = 2;
        break;
    case 1:
        thisnum = 2;
        break;
    case 2:
        thisnum = 4;
        break;
    default:
        break;
    }

    // // 랜덤 변수 만들어서 어디에 놓을지 위치 정하기
    while (1)
    {
        rannum = rand();
        rannum %= 16; // 0~15까지
        int ran1 = rannum / 4;
        int ran2 = rannum % 4;
        if (b[ran1][ran2] == 0)
        {
            b[ran1][ran2] = thisnum;
            break;
        }
    }

    return thisnum;
}

int getch(void)
{
    char ch;
    int error;
    static struct termios Otty, Ntty;

    fflush(stdout);
    tcgetattr(0, &Otty);
    Ntty = Otty;
    Ntty.c_iflag = 0;
    Ntty.c_oflag = 0;
    Ntty.c_lflag &= ~ICANON;
#if 1
    Ntty.c_lflag &= ~ECHO;
#else
    Ntty.c_lflag |= ECHO;
#endif
    Ntty.c_cc[VMIN] = 0;
    Ntty.c_cc[VTIME] = 1;

#if 1
#define FLAG TCSAFLUSH
#else
#define FLAG TCSANOW
#endif

    if (0 == (error = tcsetattr(0, FLAG, &Ntty)))
    {
        error = read(0, &ch, 1);
        error += tcsetattr(0, FLAG, &Otty);
    }

    return (error == 1 ? (int)ch : -1);
}

int GetCommand()
{
    int ch = getch();

    switch (ch)
    {
    case 'd':
    case 'D':
        return 0;
    case 'w':
    case 'W':
        return 1;
    case 'a':
    case 'A':
        return 2;
    case 's':
    case 'S':
        return 3;
    default:
        return -1;
    }

    return -1;
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
    tot += make_two_or_four();
    tot += make_two_or_four();

    draw_board(tot, -1);

    do
    {
        command = GetCommand();

        if (command != -1)
        {
            if (set_board(command, b))
            {

                /* user code */
                // 2 or 4 생성, 새로 생긴 2 or 4 를 tot에 더해주기
                tot += make_two_or_four();

                draw_board(tot, command);

                game_over = is_game_over();
            }
        }

    } while (!game_over);

    system("clear");
    printf("game over!\n");
    fclose(fp);
}

// is game over 가 제대로 game over를 감지하지 못함
// make two or four 이 1칸만 남았을 때 숫자를 생성하지 못함.