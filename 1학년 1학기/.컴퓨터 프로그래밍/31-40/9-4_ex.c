#include <stdio.h>



int main(void) {

    int i, a;

    printf("Input Integers : ");
    

    for (i=0;i<10;i++) {
        scanf("%d", &a);
        printf("%d", a);
    }



    scanf("%d", &a);
    for (i=0;i<10;i++) {
        printf("%d", a);
    }

    return 0;

}
