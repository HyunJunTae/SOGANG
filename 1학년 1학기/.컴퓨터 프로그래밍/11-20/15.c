#include <stdio.h>

int main(void){
    int Num;
    printf("Input : ");
    scanf("%d", &Num);
    Num % 2 == 0 ? printf("%d is even number\n", Num) : printf("%d is odd number\n", Num);
    return 0;
}