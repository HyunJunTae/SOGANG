#include <stdio.h>

void print_Array(int* matrix, int row, int col, int n) {

    printf("%d ", matrix[col]);

}

int main(void) {

    int n;
    printf("Input N : ");
    scanf("%d", &n);

    int matrix[n][n];
    int i,j;

    for (i=0;i<n;i++) {
        printf("Input Matrix row %d : ", i+1);

        for (j=0;j<n;j++) {
            scanf("%d", &matrix[i][j]);
        }
    }

    printf("\n\n");


    for (i=0;i<n;i++) {
        printf("Print Matrix row %d : ", i+1);
        for (j=0;j<n;j++) {
            print_Array(matrix[i], i, j, n);
        }
        printf("\n");
    }
}

// matrix[i] 하면 이차원배열 matrix의 i번 행이 나옴. 근데 그것도 주소값.
// 바로 넘겨줘도 됨.

// 역참조연산자가 붙어있다는건, 예를들면 print_array(int *matrix)라는건
// matrix가 주소값이라서 *써서 실제값으로 불러온다는 뜻.
// 주소값 자체가 넘어간다는 뜻.