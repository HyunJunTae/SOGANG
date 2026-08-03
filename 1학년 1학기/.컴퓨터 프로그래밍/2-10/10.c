#include <stdio.h>

void swap(float* x, float* y);

int main(void){
    float x, y;
    printf("Input two Number : ");
    scanf("%f %f", &x, &y);
    printf("Before : x [%f] y [%f]\n", x, y);
    swap(&x, &y);
    printf("After : x [%f] y [%f]\n", x, y);
    return 0;
}

void swap(float* x, float* y){
    float tmp = *x;
    *x = *y;
    *y = tmp;
}