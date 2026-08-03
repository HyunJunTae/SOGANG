#include <stdio.h>

void matrix_inner_dot(int* new_matrix, int *left_matrix, int* right_matrix) {
    
    int i;
    for (i=0;i<5;i++) {
        new_matrix[i] = left_matrix[i] * right_matrix[i]; 
    }
}

int main(void) {
    int a[5][5], b[5][5], c[5][5];

    printf("Input First Matrix : \n");
    int i, j;
    for (i=0;i<5;i++) {
        for (j=0;j<5;j++) {
            scanf("%d", &a[i][j]);
        }
    }
    printf("Input Second Matrix : \n");
    for (i=0;i<5;i++) {
        for (j=0;j<5;j++) {
            scanf("%d", &b[i][j]);
        }
    }




    for (i=0;i<5;i++) {
        int *aa = a[i];
        int *bb = b[i];
        int *cc = c[i];
        for (j=0;j<5;j++) {
            matrix_inner_dot(cc, aa, bb);
        }
    }




    printf("New Matrix : \n");
    for (i=0;i<5;i++) {
        for (j=0;j<5;j++) {
            printf("%d ", c[i][j]);
        }
        printf("\n");
    }


}