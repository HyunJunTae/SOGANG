#include <stdio.h>
#include <stdlib.h>

int main(void) {
    char str[]="ab";
    char *strPt = str;
    int arr[2] = {1,2};
    int *intPt = arr;
    printf("strPt (prev): %u.\n", strPt);
    printf("intPt (prev): %u.\n", intPt);
    strPt += 1;
    intPt += 1;
    printf("strPt: %u.\n", strPt);
    printf("intPt: %u.\n", intPt);
    printf("strPt step: %u.\n", (unsigned int)strPt - (unsigned int)str);
    printf("intPt step: %u.\n", (unsigned int)intPt - (unsigned int)arr);
}