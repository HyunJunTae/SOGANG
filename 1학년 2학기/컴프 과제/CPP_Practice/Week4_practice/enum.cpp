#include <cstdio>


enum icecream {mint, mintchoco, choco, chocomint};

int main(void) {
    printf("icecream : %d %d %d %d\n", mint, mintchoco, choco, chocomint);

    icecream a;
    printf("%d\n", a);

    // mint += 10;
    // enum은 연속적인 열거형 값. 암시적으로 바뀌지 않음.

    return 0;
}