#include <stdio.h>
#include <stdlib.h>

int  main(void) {
    int a;
    int *p;

    p = new int;
    *p = 10;
    printf("%d\n", *p);
    delete p;

    p = new int[20];
    delete[] p;

    return 0;
}