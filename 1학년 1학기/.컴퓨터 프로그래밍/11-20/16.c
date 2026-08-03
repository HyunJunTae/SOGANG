#include <stdio.h>

int main(void){
    int num1, num2, answer = 0, check = 1;
    printf("Enter 2 numbers : ");
    scanf("%d %d", &num1, &num2);
    if ((num1 % 10 + num2 % 10) >= 10){
        answer++;
    }else{
        check = 0;
    }
    if ((((num1 % 100 ) / 10) + ((num2 % 100) / 10 ) + check) >= 10){
        answer++;
        check = 1;
    }
    if (((num1  / 100) + (num2 / 100) + check) >= 10){
        answer++;
    }
    printf("%d carry operation!\n", answer);
    
    return 0;

}