#define MAX_TERMS 101
#include <stdio.h>
#include <stdlib.h>
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

    return;
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

        return;
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
                for (; new_b[j].row == column; j++);
                column = new_b[j].row;
            }

            else if (new_b[j].row != column)
            {
                storesum(d, &total_d, row, column, &sum);
                i = row_begin;
                column = new_b[j].row;
            }

            else
                switch (COMPARE(a[i].col, new_b[j].col))
                {

                case -1:
                    i++;
                    break;
                case 0:
                    sum += (a[i++].value * new_b[j++].value);
                    break;
                case 1:
                    j++;
                }
        }
        for (; a[i].row == row; i++);
        row_begin = i;
        row = a[i].row;
    }
    d[0].row = rows_a;
    d[0].col = cols_b;
    d[0].value = total_d;

    return;
}

void readMatrix(term a[], char* input_txt) {
    FILE *inputFile;

     // input.txt 파일 열기
    inputFile = fopen(input_txt, "r");
    if (inputFile == NULL) {
        printf("No Input File.\n");
        return;
    }

    // input.txt파일에서 행과 열 개수 읽기
    int rows, cols, number;
    int count = 1;
    fscanf(inputFile, "%d %d", &rows, &cols);
    a[0].row = rows;
    a[0].col = cols;

    // 각 원소에 대해서, 0이면 스킵. 0이 아니면 i, j, number 값을 a에 삽입
    for (int i=0; i<rows; i++) {
        for (int j=0; j<cols; j++) {

            fscanf(inputFile, "%d", &number);

            if (number == 0) continue;
            else {
                a[count].row = i;
                a[count].col = j;
                a[count].value = number;
                count++;
            }
        }
    }

    a[0].value = count - 1;
    
    // 입력 파일 닫기
    fclose(inputFile);

    return;
}

void printMaxtix(term a[])
{
    FILE *outputFile;
    
    // 출력 파일 열기
    outputFile = fopen("output.txt", "w");
    if (outputFile == NULL) {
        printf("No Output File\n");
        return;
    }

    for (int i=0; i<=a[0].value; i++) {

        fprintf(outputFile, "%d %d %d\n", a[i].row, a[i].col, a[i].value);
    }

    return;
}


int main(void)
{
    char *input1 = "inputA.txt";
    char *input2 = "inputB.txt";
    term a[MAX_TERMS];
    term b[MAX_TERMS];
    term d[MAX_TERMS];

    readMatrix(a, input1);
    readMatrix(b, input2);

    mmult(a, b, d);

    printMaxtix(d);
    
    return 0;
}