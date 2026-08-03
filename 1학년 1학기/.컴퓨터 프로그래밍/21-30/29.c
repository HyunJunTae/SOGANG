#include <stdio.h>

float calc_avg(int* mat,int col,int N);

int main(void){
    int N;
    scanf("%d", &N);
    int matrix[N][N];
    for (int i=0; i<N; i++){
        for(int j=0; j<N; j++){
            scanf("%d", &matrix[i][j]);
        }
    }
    for(int i=0;i<N;i++){
        printf("%.2f ",calc_avg(*matrix, i, N)); 
        // *matrix로 이차원배열 일차원으로 만들어서 쭉 가져가기. 일차원으로 만들어도 배열이니까 주소값임.
    }
    printf("\n");

    return 0;
}

float calc_avg(int* mat, int col, int N){
    float sum=0;
    for(int i=0; i < N; i++){
        sum += *(mat+N*(i)+col);
    }
    return sum / N;
}
// 일차원배열로 가져가서 열 단위로 가져오기
// 3x3배열이면 [0], [3], [6]번 가져오면 됨.

//int* 는 그냥 mat 가 포인터라는 뜻. mat가 주소값을 가지고있다는 뜻. 밑에서 * 붙여서 주소값을 실제값으로 바꿔서 쓸거라는 뜻
