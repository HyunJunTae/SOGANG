#include <stdio.h>

void solution(int *result, int *matrix,int curr_x,int curr_y, int row, int first_row, int first_col, int last_row, int last_col);

int main(void){
    int row, col;
    int result = 0;
    printf("Input row, col : ");
    scanf("%d %d",&row, &col);

    int matrix[row][col];

    for(int i=0; i<row;i++){
        for(int j=0;j<col;j++){
            scanf("%d", &matrix[i][j]);
        }
    }

    int first_row,first_col,last_row,last_col;
    scanf("%d %d %d %d", &first_row, &first_col, &last_row, &last_col);
    solution(&result, (int*)matrix, first_row-1, first_col-1, col, first_row-1, first_col-1, last_row, last_col-1);
    printf("%d\n", result);
    return 0;
}


void solution(int *result, int *matrix, int curr_x, int curr_y, int col, int first_row, int first_col, int last_row, int last_col){
    printf("%d %d %d\n", *(matrix + curr_y + curr_x*col), curr_x, curr_y);
    if (curr_y > last_col){
        return;
    }
    if(curr_x == last_col){
        solution(result, matrix, first_row, curr_y+1, col, first_row, first_col, last_row, last_col);
    }else{
        solution(result, matrix, curr_x+1, curr_y, col, first_row, first_col, last_row, last_col);
    }
    *result += *(matrix + curr_y + curr_x*col);
}