#include <stdio.h>




int main(void) {

    int i, j;
    int matrix[3][3] = {{1,2,3},{4,5,6},{7,8,9}};
    int matrix1[3] = {1,2,3};

    printf("%d\n", *(*(matrix+1)+1));
    printf("%d\n", *((matrix[1])+1));
    printf("%d\n", *(*matrix+4));
    printf("%d\n", *(*(matrix+4)));
    printf("%d\n", **(matrix+4));
    printf("%d\n", **(matrix+0));

    printf("%d\n", matrix1[1]);
    printf("%d\n", *matrix1+1);
    printf("%d\n", *(matrix1+1));

}


// C언어 상에서 2차원배열은 존재하지 않는다. 그냥 우리가 보기좋게 일차원배열을 끊어서 이차원으로 만든거.
// 그래서 이차원배열도 일차원배열로 생각하고, 3X3 배열을 그냥 뭐 몇번칸. 이렇게 바로 할수있음 몇번행몇번열 하지말고.

