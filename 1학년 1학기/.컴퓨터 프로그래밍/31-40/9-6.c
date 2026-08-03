#include <stdio.h>
#include <stdlib.h>

int main(void){
    int maxlen, len = 3, num, index = 0;

    int *ptr = (int *)malloc(sizeof(int) * len);

    while(1){
        scanf("%d", &num);
        if(num == -1){
            break;
        }
        if (index==len){
            printf("array is full %d/%d.....realloc ....\n", index, len);
            ptr = (int *)realloc(ptr,sizeof(int)*(len+2));
            len = len+2;
        }
        ptr[index] = num;
        index++;
    }
    printf("Array : ");
    for(int i=0; i<len;i++){
        printf("%d ",ptr[i]);
    }
    printf("\n");
    free(ptr);
    return 0;
    
}