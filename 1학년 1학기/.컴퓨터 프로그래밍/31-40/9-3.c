    #include <stdio.h>

void print_Array(int *matrix, int row, int col, int n);

int main(void){
    int N;
    printf("Input N : ");
    scanf("%d", &N);
    int arr[N][N];
    for(int i=0;i<N;i++){
        printf("Input Matrix row %d: ", i+1);
        for (int j=0;j<N;j++){
            scanf("%d",&arr[i][j]);
        }
    }

    
    for(int i=0;i<N;i++){
        printf("\nPrint Matrix row %d : ", i+1);
        for(int j=0;j<N;j++){
            print_Array((int*)arr,i,j,N);
        }
    }
    printf("\n");
    return 0;
}
// (int*)arr 하면 배열 arr 안에 있던 모든 값들이 주소값으로 바뀐다?

void print_Array(int *matrix, int row, int col, int n){
    printf("%d ", *(matrix + col + row * n));
    return;
}
