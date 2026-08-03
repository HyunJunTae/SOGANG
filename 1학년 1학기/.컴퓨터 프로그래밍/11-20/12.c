#include <stdio.h>

int x = 1;

int Fibonacci(int *y);

int main(void){
    int x = 0;
    x = Fibonacci(&x);
    x = Fibonacci(&x);
    printf("Fibo (2) : %d\n", x);
    x = Fibonacci(&x);
    printf("Fibo (3) : %d\n", x);
    x = Fibonacci(&x);
    printf("Fibo (4) : %d\n", x);
    x = Fibonacci(&x);
    printf("Fibo (5) : %d\n", x);
    x = Fibonacci(&x);
    printf("Fibo (6) : %d\n", x);
    x = Fibonacci(&x);
    printf("Fibo (7) : %d\n", x);
    x = Fibonacci(&x);
    printf("Fibo (8) : %d\n", x);
    x = Fibonacci(&x);
    printf("Fibo (9) : %d\n", x);
    return 0;
}

int Fibonacci(int *y){
    x = x + *y;
    *y = x - *y;
    return *y;
}

