#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
    int *a = new int;
    *a = 10;
    printf("%d  ", a);
    printf("%d\n", *a);


    char *b = new char[10];
    char *c = (char*)malloc(10*sizeof(char));
    char *d = new char[10];
    int *e = new int[3] {1, 2, 3};

    strcpy(b, "HAPPY");
    strcpy(c, "ABCDEABCDEAAA");
    strcpy(d, "hahahahaha");
    
    printf("%s %s %s", b, c, d);




    return 0;

}