#include <stdio.h>
void matrix_inner_dot(int* new_matrix ,int *left_matrix,int *right_matrix);

int main(void){
    int left_matrix[5][5];
    int right_matrix[5][5];
    int new_matrix[5][5]; 

    printf("Input First Matrix : \n");
    for (int i=0;i<5;i++){
        for(int j=0;j<5;j++){
            scanf("%d", &left_matrix[i][j]);
        }
    }

    printf("\nInput Second Matrix : \n");
    for (int i=0;i<5;i++){
        for(int j=0;j<5;j++){
            scanf("%d", &right_matrix[i][j]);
        }
    }
    
    matrix_inner_dot((int*)new_matrix, (int*)left_matrix, (int*)right_matrix);
    
    printf("\nNew Matrix : \n");
    for(int i=0; i<5; i++){
        for (int j=0; j<5; j++){
            printf("%d ", new_matrix[i][j]);
        }
        printf("\n");
    }
    return 0;
}

void matrix_inner_dot(int* new_matrix ,int *left_matrix,int *right_matrix){
    
    for(int i=0;i<5; i++){
        for(int j=0;j<5;j++){
            *((new_matrix+j)+i*5) = *((left_matrix+j)+i*5) * *((right_matrix+j)+i*5);
        }
    }
    return;
}
// 여기서도 (int*)matrix 이런 식으로 한 줄로 만들어서 계산?