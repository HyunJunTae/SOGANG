#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(void) {
    int i;

    char s1[10] = "world";
    char *s2 = (char*)malloc(sizeof(char) * 20);
    strcpy(s2,"Hello");

    // strcat(s2, s1);
    // printf("%s", s2);

    strcat(s1, s2);
    printf("%s", s1);

    // 둘 다 되는디.
    // 동적배열에 정적배열을 cat 할수도, 정적배열에 동적배열을 cat 할수도 있다.

    free(s2);

    return 0;
}