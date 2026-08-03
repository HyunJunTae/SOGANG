#include <stdio.h>

int solution(int a, int b);

int main(void){
    int num1, num2;
    printf("Input first number : ");
    scanf("%d", &num1);
    printf("Input second number : ");
    scanf("%d", &num2);
    printf("%d + %d = %d\n",num1,num2,solution(num1,num2));
    return 0;
}

int solution(int a, int b){
    return a+b;
}