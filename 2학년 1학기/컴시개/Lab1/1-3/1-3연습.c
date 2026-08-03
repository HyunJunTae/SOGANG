/*
 * conditional(x, y, z): Return z if x is 0, return y otherwise.
 * - Ex) conditional(2, 4, 5) = 4, conditional(0, 1, 2) = 2
 */

#include <stdio.h>

int conditional(int x, int y, int z)
{
    int is_zero = !x; // x=0이면 1, o.w. 0.
    printf("is zero : %d\n", is_zero);
    int mask = ~(!is_zero) + 1;
    printf("mask : %d\n", mask);

    return (y & mask) | (z & ~mask);
}

// 이렇게 하면 - 쓰는거라 안됨
// int conditional(int x, int y, int z) {
//   int is_zero = !x; // x=0이면 1, o.w. 0.
//   printf("is zero : %d\n", is_zero);
//   int mask = is_zero - 1;
//   printf("mask : %d\n", mask);

//   return (y&mask) | (z&~mask);
// }

int main(void)
{

    int x = 0;
    int y = 1;
    int z = 2;

    printf("%d\n", conditional(x, y, z));

    return 0;
}