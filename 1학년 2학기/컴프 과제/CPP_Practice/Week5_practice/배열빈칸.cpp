#include <cstdio>


int main(void) {

    int arr[3] = {0, 1};
    char crr[3] = {'a', 'b'};

    printf("%d\n", arr[0]);
    printf("%c\n", crr[1]);
    printf("----------------------------\n");
    printf("%d\n", arr[2]);
    printf("%c\n", crr[2]);
    printf("----------------------------\n");
    printf("%d\n", arr[3]);
    printf("%c\n", crr[3]);

    if (crr[1] == '\0')
        printf("so");
    if (crr[2] == '\0')
        printf("gang");
    if (crr[3] == '\0')
        printf("uni");
    if (crr[3] == '\0')
        printf("versity");


    return 0;
}