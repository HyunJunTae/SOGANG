#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(void) {
    int i;

    char *s1 = (char*)malloc(sizeof(char) * 20);
    strcpy(s1,"World!");
    char *s2 = (char*)malloc(sizeof(char) * 20);
    strcpy(s2,"Hello");

    strcat(s2, s1);
    strcat(s1, s2);
    printf("%s\n", s1);
    printf("%s\n", s2);

    // 둘 다 동적배열이어도 됨. 뭐가 문제야 그럼.

    free(s1);
    free(s2);

    return 0;
}