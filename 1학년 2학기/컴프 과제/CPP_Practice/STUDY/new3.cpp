#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(void) {

    char a[5] = "ABC";

    for (int i=0; i<5; i++) {
        if (a[i] == '\0')
            printf("NULL! ");
        else
            printf("%c ", a[i]);
    }
    printf("\n");

    

    char *b = (char*)malloc(5 * sizeof(char));
    strcpy(b, "DCBA");

    for (int i=0; i<5; i++) {
        if (b[i] == '\0')
            printf("NULL! ");
        else
            printf("%c ", b[i]);
    }
    printf("\n");

    return 0;
}