#include <stdio.h>

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

int main(void)
{

    int c[4][4] = {{2, 0, 2, 2}, {2, 0, 4, 8}, {4, 0, 0, 0}, {2, 2, 2, 2}};

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            printf("%d ", c[i][j]);
        }
        printf("\n");
    }

    rotate(c);
    rotate(c);

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

    printf("--------------------\n");

    rotate(c);
    rotate(c);

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            printf("%d ", c[i][j]);
        }
        printf("\n");
    }

    return 0;
}