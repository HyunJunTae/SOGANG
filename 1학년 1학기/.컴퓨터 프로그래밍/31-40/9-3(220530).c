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
        int *p = matrix[i];
        printf("Print Matrix row %d : ", i+1);
        for (j=0;j<n;j++) {
            print_Array(p, i, j, n);
        }
        printf("\n");
    }
}