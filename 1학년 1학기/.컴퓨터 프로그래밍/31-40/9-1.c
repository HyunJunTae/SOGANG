#include <stdio.h>

int main(void){
    int start=100;
    int *pointer = &start;
    int num;
    int no2;
    int no1;

    printf("no1 : ");
    scanf("%d", &no1);
    scanf("%d", &no2);

    printf("\n-------------print start---------------\n");
    printf("start_value, memory_address : %d %p\n", start, &start);
    printf("no1_value, memory_address : %d %p\n", no1, &no1);
    printf("no2_value, memory_address : %d %p\n", no2, &no2);
    printf("-------------print end---------------\n\n");

    printf("change start's address with pointer : ");

    // Start_code
    scanf("%d", &num);
    if(num%2==0){
        pointer = &no2;
    }else{
        pointer = &no1;
    }
    // End_code
    printf("\n-------------print start---------------\n");
    printf("start_value, memory_address : %d %p\n", *pointer, pointer);
    printf("-------------print end---------------\n\n");

    printf("change start value with pointer : ");
    // Start_code
    scanf("%d" , pointer);
    // End_code
    printf("\n-------------print start---------------\n");
    printf("start_value, memory_address : %d %p\n", *pointer, pointer);
    printf("-------------print end---------------\n\n");
    return 0;
}