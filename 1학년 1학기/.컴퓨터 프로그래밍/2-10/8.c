#include <stdio.h>

float calc_mean(int a, int b, int c, int d);
float calc_deviation(float average, int num);

int main(void){
    int num1, num2, num3, num4;
    float average, deviation;
    printf("Input first integer : ");
    scanf("%d",&num1);
    printf("Input second integer : ");
    scanf("%d",&num2);
    printf("Input third integer : ");
    scanf("%d",&num3);
    printf("Input fourth integer : ");
    scanf("%d",&num4);
    average = calc_mean(num1, num2, num3, num4);
    printf("********* average is %.2f *********\n", average);
    printf("first integer : \t %2d -- deviation : \t %8.2f\n", num1, calc_deviation(average,num1));
    printf("second integer : \t %2d -- deviation : \t %8.2f\n", num2, calc_deviation(average,num2));
    printf("third integer : \t %2d -- deviation : \t %8.2f\n", num3, calc_deviation(average,num3));
    printf("fourth integer : \t %2d -- deviation : \t %8.2f\n", num4, calc_deviation(average,num4));
    return 0;
}


float calc_mean(int a, int b, int c, int d){
    return (a+b+c+d)/4.0;
}

float calc_deviation(float average, int num){
    return num - average;
}
