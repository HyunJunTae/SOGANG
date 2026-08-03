#include <stdio.h>

int main(){
    int num1, num2;
    printf("Input two integer : ");
    scanf("%d %d",&num1, &num2);
    printf("%d / %d is %d with a remainder of %d\n",num1,num2,num1/num2,num1%num2);
    return 0;
}
