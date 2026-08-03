#include <stdio.h>

int main(void) {
    char k[10100];

    scanf("%s", k);
    
    for (int i=0; i<10100; i++) {
        if (k[i] == '\n'){
            printf("\n");
            continue;
        }
        printf("%c\n", k[i]);
    }
    
    return 0;
}