/*
 * absVal(x): return the absolute value of x.
 * - Assume that INT_MIN (-2^31) is not provided as an input.
 * - Ex) absVal(2) = 2, absVal(-1) = 1
 */
#include <stdio.h>

int absVal(int x)
{

    int sign = x >> 31; // 부호
    printf("sign : %d\n", sign);
    int zero_or_one = ~sign + 1;
    printf("zero_or_one : %d\n", zero_or_one);
    int x_after_change = (x ^ sign) + zero_or_one;

    return x_after_change;
}

int main(void)
{

    int a = 30000;

    printf("x_after_change : %d", absVal(a));

    return 0;
}