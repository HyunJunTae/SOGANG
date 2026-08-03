#include <stdio.h>
#include <string.h>

template <typename S>

void swap(S *x, S *y) {
    S tmp = *x;
    *x = *y;
    *y = tmp;
}

int main(void) {
    int i1 = 1, i2 = 2;
    double f1 = 1.5, f2 = 3.5;
    
    swap (&i1, &i2);
    swap <double> (&f1, &f2);

    printf("%d %d\n", i1, i2);
    printf("%f %f\n", f1, f2);

    return 0;
}