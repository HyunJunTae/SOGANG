#include <stdio.h>
#include <string.h>

void strins(char *a, char *b, int i)
{
    char string[100];
    char *temp = string;

    strncpy(temp, a, i);
        // *(temp+i) = '\0';
    strcat(temp, b);
    strcat(temp, (a + i));
    strcpy(a, temp);
}

int main(void) {

    char a[100] = "abcdefg";
    char b[100] = "pqlm";


    strins(a, b, 2);

    for (int i=0; i<10; i++) {
        printf("%c", a[i]);
    }

    return 0;
}
