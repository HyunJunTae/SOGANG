#include <stdio.h>
#include <stdlib.h>

void transpose(int **a, int **b, int rows, int cols) {


    // transpose 실행
    for (int i = 0; i < cols; i++) {
        for (int j = 0; j < rows; j++) {
            b[i][j] = a[j][i];
        }
    }

    return;
}


int main(void) {

    FILE *inputFile, *outputFile;

    // input.txt 파일 열기
    inputFile = fopen("input.txt", "r");
    if (inputFile == NULL) {
        printf("No Input File.\n");
        return 1;
    }

    

    // input.txt파일에서 행과 열 개수 읽기
    int rows, cols;
    fscanf(inputFile, "%d %d", &rows, &cols);

    // a배열 선언.
    int **a;
    a = (int**)malloc(rows*sizeof(int*));
    for (int i = 0; i<rows; i++) { 
        a[i] = (int*)malloc(cols*sizeof(int));
    }

    // a 배열에 행렬 데이터 읽어서 저장
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            fscanf(inputFile, "%d", &a[i][j]);
        }
    }

    // 입력 파일 닫기
    fclose(inputFile);




    // b배열 선언
    int **b;
    b = (int**)malloc(cols*sizeof(int*));
    for (int i = 0; i<cols; i++) { 
        b[i] = (int*)malloc(rows*sizeof(int));
    }

    // b배열에 a배열의 transpose를 저장
    transpose(a, b, rows, cols);




    // 출력 파일 열기
    outputFile = fopen("output.txt", "w");

    // b행렬 데이터를 output.txt에 쓰기
    fprintf(outputFile, "%d %d\n", cols, rows);
    for (int i = 0; i < cols; i++) {
        for (int j = 0; j < rows; j++) {
            fprintf(outputFile, "%d ", b[i][j]);
        }
        fprintf(outputFile, "\n");
    }

    // 출력 파일 닫기
    fclose(outputFile);

    for (int i = 0; i < rows; i++) {
        free(a[i]);
    }
    free(a);
    for (int i = 0; i < cols; i++) {
        free(b[i]);
    }
    free(b);

    return 0;
}