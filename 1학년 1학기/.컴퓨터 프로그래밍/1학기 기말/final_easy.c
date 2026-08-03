#include <stdio.h>
#include <stdlib.h> 
void Merge(int* arr1, int* arr2, int* newArr);

int main(void){
    int* arr1 = (int*)malloc(sizeof(int) * 1);
    int* arr2 = (int*)malloc(sizeof(int) * 1);

    int arr1_len=0;
    int arr2_len=0;
    while(1){
        int num;
        scanf("%d", &num);
        if (num == -1){
            arr1[arr1_len++] = num;
            break;
        }
        arr1[arr1_len++] = num;
        if (arr1_len%2==0){
            arr1 = realloc(arr1, sizeof(int)*(arr1_len+2));
        }

    }

    while(1){
        int num;
        scanf("%d", &num);
        if (num == -1){
            arr2[arr2_len++] = num;
            break;
        }
        arr2[arr2_len++] = num;
        if (arr2_len%2==0){
            arr2 = realloc(arr2, sizeof(int)*(arr2_len+2));
        }
    }
    int* newArr = (int*)malloc(sizeof(int) * (arr2_len + arr1_len));
    
    Merge(arr1, arr2, newArr);
    
    for(int i=0; i < (arr1_len + arr2_len); i++){
        if (newArr[i]==-1){
            break;
        }
        printf("%d ", newArr[i]);
    }
    printf("\n");
    return 0;
}

void Merge(int* arr1, int* arr2, int* newArr){
    int left_arr=0;
    int right_arr=0;
    int i = 0;
    while(arr1[left_arr]!=-1 && arr2[right_arr]!=-1){
        
        if (arr1[left_arr] >= arr2[right_arr]){
            newArr[i++] = arr2[right_arr++];
        }else{
            newArr[i++] = arr1[left_arr++];
        }
    }
    while(1){
        if(arr1[left_arr]==-1){
            newArr[i++] = arr2[right_arr++];
            if (arr2[right_arr]==-1){
                newArr[i] = -1;
                break;
            }
        }else{
            newArr[i++] = arr1[left_arr++];
            if (arr1[left_arr]==-1){
                newArr[i] = -1;
                break;
            }
        }
    }
}