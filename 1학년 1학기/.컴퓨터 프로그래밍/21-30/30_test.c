#include <stdio.h>


void onggibot(char *x) {

    printf("%c", *x);
}


int main (void) {

    // float a = 1234567.1234567;


    // printf("%5.2f\n", a);
    // printf("%10.4f\n", a);

    char a[10];
    int i;

    for (i=0;i<10;i++) {
        char c;
        scanf("%c", &c);
        a[i] = c;
    }
    
    onggibot(a);

    return 0;


}