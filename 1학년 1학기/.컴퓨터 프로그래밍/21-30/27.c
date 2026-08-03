#include <stdio.h>

int main(void){
    int arr[10];
    for (int i=0; i<10; i++){
        scanf("%d", &arr[i]);
    }
    for (int i=0;i<5;i++){
        int tmp = arr[i];
        arr[i] = arr[9-i];
        arr[9-i] = tmp;
    }
    for(int i=0; i<10; i++){
        printf("%d ", arr[i]);
    }
    printf("\n");
    int min = arr[0];
    int max = arr[0];
    for (int i=0; i<10;i++){
        if (min > arr[i]){
            min = arr[i];
        }
        if(max < arr[i]){
            max = arr[i];
        }
    }
    printf("MAX : %d, MIN : %d\n",max,min);
    return 0;
}
