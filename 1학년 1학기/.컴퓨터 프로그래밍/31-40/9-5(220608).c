        #include <stdio.h>
#include <stdlib.h>



int main(void) {

    int* arr;
    int N;
    printf("Input N : ");
    scanf("%d", &N);
    arr = (int*)malloc(N*sizeof(int));

    for(int i=0;i<10;i++) {

        if(i >= N) {
            realloc(arr, N*2 * sizeof(int));
            N = N * 2;
            printf("Memory realloc N = %d\n", N);
        }
        printf("Input arr[%d] : ", i);
        scanf("%d", &arr[i]);
    }

    for(int i=0;i<N;i++) {
        printf("arr[%d] : %d\n", i, arr[i]);
    }

    free(arr);
    return 0;

}

