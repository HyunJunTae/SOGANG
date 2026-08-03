#include <stdio.h>
#include <string.h>

void strins(char *a, char *b, int i)
{
    char string[100];

    strncpy(string, a, i);
    // *(string+i) = '\0';
    strcat(string, b);
    strcat(string, (a + i));
    strcpy(a, string);
}

int main(void) {

    char a[100] = "abcd";
    char b[100] = "pqlm";


    strins(a, b, 2);

    for (int i=0; i<10; i++) {
        printf("%c", a[i]);
    }

    return 0;
}
