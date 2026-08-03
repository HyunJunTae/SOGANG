#include <stdio.h>

int sqr(int num);
void printOne(int num);

int main(void){
    int n, sqrn;
    printf("input : ");
    scanf("%d", &n);
    sqrn = sqr(n);
    printOne(sqrn);
    return 0;
}

int sqr(int num){
    return (num * num);
}

void printOne(int num){
    printf("Result : %d\n", num);
    return;
}