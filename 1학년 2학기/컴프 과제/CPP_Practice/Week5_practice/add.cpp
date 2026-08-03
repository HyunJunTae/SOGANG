#include <stdio.h>

int add(int *a, int*b) {
    *a = *a + *b;

    return *a+*b;
}

int main(void) {

    int a = 3, b = 4;

    printf("%d %d %d", a, b, add(&a, &b));
}