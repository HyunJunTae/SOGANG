#include <stdio.h>

int palindrome(int num, int digit);

int main(void){
    int N;
    scanf("%d", &N);
    if (palindrome(N,1)==0){
        printf("%d is not palindrome.\n",N);
    }else{
        printf("%d is a Palindrome number.\n", N);
    }
    return 0;
}

int palindrome(int num, int digit){
    if ((num / digit) < 10){
        return 1;
    }
    int check_num=1;
    while(check_num < num){
        check_num *= 10;
    }
    check_num /= 10;

    if ((num%(digit*10))/digit == (num / (check_num/digit)) % 10){
        palindrome(num, digit*10);
    }else{
        return 0;
    }

    
}