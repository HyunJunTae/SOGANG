#define MAX_TERMS 101
#include <stdio.h>
// #define COMPARE (x,y) ((x) < (y)) ? -1: ((x) == (y)) ? 0: 1
#define COMPARE(x, y) (x < y) ? -1 : (x == y) ? 0 : 1

typedef struct
{
    int row;
    int col;
    int value;
} term;

void fast_transpose(term a[], term b[])
{

    int i, j;
    int num_terms = a[0].value; // total number o elements
    int num_cols = a[0].col;
    int row_terms[100];
    int starting_pos[100];

    b[0].row = a[0].col;
    b[0].col = a[0].row;
    b[0].value = num_terms;

    if (num_terms > 0)
    {
        // row_temrs의 값을 a의 컬럼 개수만큼 0으로
        for (i = 0; i < num_cols; i++)
        {
            row_terms[i] = 0;
        }
        // 각 컬럼에 값 몇 개 있는지 계산
        for (i = 1; i < num_terms; i++)
        {
            row_terms[a[i].col]++;
        }

        // starting_pos 계산 (각 컬럼별 값이 시작되는 곳)
        starting_pos[0] = 1;
        for (i = 1; i < num_cols; i++)
        {
            starting_pos[i] = starting_pos[i - 1] + row_terms[i - 1];
        }

        // starting_pos는 b에 넣어야하는 위치를 컬럼별로 나눈거.
        // 0번 칸에 값은 a에서 col이 0인 값들을 넣기 시작하는 위치.
        // 1번 칸에 값은 a에서 col이 1인 값들을 넣기 시작하는 위치.
        for (i = 1; i <= num_terms; i++)
        {
            j = starting_pos[a[i].col]++; // col 값을 알면, starting_pos 를 통해 해당 원소를 b의 어느 위치에 넣어야하는지 알 수 있다.
            b[j].row = a[i].col;
            b[j].col = a[i].row;
            b[j].value = a[i].value;
        }
    }
}

void storesum(term d[], int *totald, int row, int column, int *sum)
{
    if (*sum)
        if (*totald < MAX_TERMS)
        {
            d[++*totald].row = row;
            d[*totald].col = column;
            d[*totald].value = *sum;
            *sum = 0;
        }
        else
        {
            fprintf(stderr, "Numbers of terms exceeds %d", MAX_TERMS);
            exit(1);
        }
}

void print_terms(term a[])
{
    int k = 1;
    for (int i = 0; i < a[0].row; i++)
    {
        for (int j = 0; j < a[0].col; j++)
        {
            if (k <= a[0].value && a[k].row == i && a[k].col == j)
            {
                // 저장된 비제로 항이 있는 위치이면 해당 값을 출력
                printf("%d ", a[k].value);
                k++;
            }
            else
            {
                // 비저장(즉, 0인) 항이면 0 출력
                printf("0 ");
            }
        }
        printf("\n");
    }
}

void mmult(term a[], term b[], term d[])
{

    int i, j;

    int rows_a = a[0].row;
    int cols_a = a[0].col;
    int total_a = a[0].value;

    int cols_b = b[0].col;
    int total_b = b[0].value;

    int total_d = 0;

    int row = a[1].row;
    int column;
    int row_begin = 1;
    int sum = 0;

    term new_b[MAX_TERMS];

    if (cols_a != b[0].row)
    {
        printf("stderr, Incompatible matrices\n");
        return;
    }

    fast_transpose(b, new_b);

    a[total_a + 1].row = rows_a;
    new_b[total_b + 1].row = cols_b;
    new_b[total_b + 1].col = -1;

    for (i = 1; i <= total_a;)
    {
        column = new_b[1].row;
        for (j = 1; j <= total_b + 1;)

        {
            if (a[i].row != row)
            {
                storesum(d, &total_d, row, column, &sum);
                i = row_begin;
                for ( ; new_b[j].row == column; j++);
                column = new_b[j].row;
            }

            else if (new_b[j].row != column) {
                storesum(d, &total_d, row, column, &sum);
                i = row_begin;
                column = new_b[j].row;
            }

            else switch (COMPARE(a[i].col, new_b[j].col)) {

                case -1 :
                    i++;
                    break;
                case 0:
                    sum += (a[i++].value * new_b[j++].value);
                    break;
                case 1:
                    j++;
            }
        }
        for ( ;a[i].row == row; i++);
        row_begin = i;
        row = a[i].row;
    }
    d[0].row = rows_a;
    d[0].col = cols_b;
    d[0].value = total_d;
}

int main(void)
{

    term a[MAX_TERMS];
    term b[MAX_TERMS];
    term d[MAX_TERMS];

    a[0].row = 2;
    a[0].col = 3;
    a[0].value = 2;
    a[1].row = 0;
    a[1].col = 0;
    a[1].value = 2;
    a[2].row = 1;
    a[2].col = 1;
    a[2].value = 1;

    print_terms(a);

    fast_transpose(a, b);

    print_terms(b);

    return 0;
}