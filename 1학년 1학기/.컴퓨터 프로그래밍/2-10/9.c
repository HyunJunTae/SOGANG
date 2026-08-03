#include <stdio.h>

void divideRemainder(int *a, int* b);

int main(void){
    int a, b;
    printf("Input Two Number : ");
    scanf("%d %d", &a,&b);
    divideRemainder(&a, &b);
    printf("OUTPUT : divide [%d] remainder [%d]\n", a,b);
    return 0;
}

void divideRemainder(int *a, int* b){
    int remainder;
    remainder = *a % *b ;
    *a = *a / *b;
    *b = remainder;
}