#include <stdio.h>
#include <string>
#include <iostream>
using namespace std;




int main(void) {
    
    enum Direction {East, West, South, North};
    enum icecream {mint=3, mintchoco, choco=7, chocomint};

    int direction1 = West;
    Direction direction2 = East;

    printf("%d %d\n\n", direction1, direction2);


    printf("icecream : %d %d %d %d\n", mint, mintchoco, choco, chocomint);

    icecream a = mint;
    printf("%d\n", a);

    if (a == mint)
        printf("a is mint\n");
    
    if (a == North)
        printf("a is North\n");

    return 0;
}