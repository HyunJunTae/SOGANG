#include <cstdio>
#include <string.h>
#include <stdlib.h>



int main(void) {

    char* a;
    a = (char*)malloc(5);
    strcpy(a, "Hello");
    char* b;
    b = (char*)malloc(5);
    strcpy(b, "World!");

    int i = strlen(a);


    
    printf("%s  %s  %d\n", a, b, i);

    strcat(a, b);
    printf("%s  %s  %d\n", a, b, i);

    // i=strlen(a) 로 한 번 저장해두면, a의 길이가 변해도 i의 값은 변하지 않음. 

    return 0;
}