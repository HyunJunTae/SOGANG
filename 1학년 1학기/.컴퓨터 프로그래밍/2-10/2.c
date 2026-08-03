#include <stdio.h>

int main(void){
    int a;
    printf("input a : ");
    scanf("%d", &a);
    printf("++a = %d\n",++a); // prefix a는 계산이후 출력이된다.
    printf("a++ = %d\n",a++); // postfix a는 출력이후 계산이된다.
    printf("a-- = %d\n",a--); // postfix a는 출력이후 계산이된다.
    return 0;
}
