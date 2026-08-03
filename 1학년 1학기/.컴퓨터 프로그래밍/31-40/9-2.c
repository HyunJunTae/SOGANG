#include <stdio.h>

int* calc(char op, int* no1, int* no2);

int main(void){
    int no = 100;   
    int no2;
    int *p = &no;
    char op;

    printf("input operation : ");
    scanf("%c", &op);

    printf("input Integer : ");
    scanf("%d", &no2);

    p = calc(op, &no, &no2);

    printf("result : %d\n",  *p);
    return 0;
}

int* calc(char op, int *no1, int* no2){
    
    if (op == '+'){
        *no1 = (*no1 + *no2);
        return no1;
    }
    if(op == '-'){
        *no1 = (*no1 - *no2);
        return no1;
    }
    if(op == '*'){
        *no1 = (*no1 * *no2);
        return no1;
    }
    if (op == '/'){
        *no1 = (*no1 / *no2);
        return no1;
    }
}