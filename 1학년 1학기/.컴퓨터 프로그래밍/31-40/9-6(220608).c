#include <stdio.h>
#include <stdlib.h>

int main(void) {

    int* arr;
    int N = 3;
    int num;
    int i = 0;

    arr = (int*)malloc(N*sizeof(int));

    printf("Input Integers : ");
    do {
        scanf("%d", &num); // 1 2 3 4 5 6 7 8 9

        if(i >= N) {
            printf("array is full %d/%d...... realloc ....\n", N, N);
            realloc(arr, N+2 * sizeof(int));
            N = N + 2;
        }

        arr[i] = num;

        i = i + 1;

    }while(num != -1);

    printf("Array : ");
    for(int i=0;i<N;i++) {
        if (arr[i] == -1) {
            break;
        }
        printf("%d ", arr[i]);
    }

    free(arr);
    return 0;

}