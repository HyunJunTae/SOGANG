#include <stdio.h>

int print_array(int *matrix, int row, int col, int n) {
    printf("%d\n", *(matrix + col + row * n));
}

int add(int matrix[][3], int i, int j) {
    return matrix[i][j];

}

int add1(int matrix[], int i) {
    return matrix[i];
}

int main(void) {

    int i, j;
    int matrix[3][3] = {{1,2,3},{4,5,6},{7,8,9}};
    int matrix1[3] = {1,2,3};

    printf("%d\n", matrix[0]);
    printf("%d\n", *matrix[0]);
    printf("%d %d %d\n", *matrix[0]);
    printf("%d\n", *((int*)matrix + 2 + 1 * 3));
    printf("%d\n", *(*(matrix+1)+2));

    // (int*)하면 이차원배열이 일차원으로 바뀐다?
    // 일차원배열로 바꿔서 (열번호)*차수 + 행번호 해서 출력
    // 이차원배열에서 *(matrix+1) 행 번호 먼저 입력해서 1번 행 보고, *(*(matrix+1)+2) 뒤에 +2해서 2번열 보는거.

    print_array((int*)matrix,1,2,3);
    // matrix는 3x3 배열. 그냥 matrix라고 쓰면 0번행    주소값임. 그래서 (int*)로 실제값으로 바꾸기.
    // 근데 바꾸면 일차원 배열이 됨 -> 일차원배열도 주소값. 함수에 가져가려면 int *matrix처럼 하는게 당연.
    // 배열을 가져갈 때 print_array(matrix) / print_array(int mat[]) 이런식으로 배열 자체를 가져갈 수도 있지만,
    // print_array((int*)matrix) / print_array(int * matrix) 이런식으로 배열을 일차원배열로 바꿔서 가져갈수도 있다
    // 근데 그냥 print_array(matrix)처럼 하면 이차원 배열은 다 가져가지 못하잖아. 한줄밖에 못가져가잖아.
    // 그래서 내가 9-3, 9-4실습 코드 잘 때는 이차원배열은 한 행씩 가져가서 출력했잖아.
    
    // 일차원 배열의 경우 배열 가져가도록 add1(matrix1) / add(int matrix[])하면 잘됨
    for (i=0;i<3;i++) {
        printf("%d", add1(matrix1, i));
    }
    printf("\n");

    // 근데 이차원배열에서는 그렇게하면 생각대로 안됨. 이차원배열 다 가져가려면?
    for (i=0;i<3;i++) {
        for (j=0;j<3;j++) {
            printf("%d", add(matrix, i, j));
        }
    }
    printf("\n");
    // 이차원배열 다가져가려면 함수에서 int(matrix[][3]) 처럼 높이도 설정해줘야함. 근데 이건 안배웠잖아 학교에서.

    

    

    return 0;

}
