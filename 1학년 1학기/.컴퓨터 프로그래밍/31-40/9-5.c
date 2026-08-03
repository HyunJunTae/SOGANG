#include <stdio.h>
#include <stdlib.h>

void main()
{
    int* arr;
    int N;
    printf("Input N : ");
    scanf("%d", &N);
    arr = (int*)malloc(sizeof(int) * N); 

    for(int i=0;i<10;i++){

        if(N <= i){
            arr  = realloc(arr, sizeof(int)*(N*2));
            N *= 2;
            printf("Memory realloc N = %d\n", N);
        }
        printf("Input arr[%d] : ",i);
        scanf("%d", &arr[i]);
    }

    for (int i = 0; i < N; i++) {
        printf("arr[%d] : %d\n", i, arr[i]);
    }

    free(arr);
}