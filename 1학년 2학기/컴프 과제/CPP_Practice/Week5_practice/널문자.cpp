#include <cstdio>
#include <stdlib.h>
#include <string.h>


int main(void) {

    char a[4] = "ABC";
    // char b[4] = "ABCD"; // 너무 길다고 컴파일 자체가 안됨. 널문자 들어갈 자리가 있어야함.

    char* c;
    c = (char*)malloc(5);
    memcpy(c, a, 4);

    printf("%s\n", a);
    printf("%s\n", c);

    // ????????????????????


    return 0;
}