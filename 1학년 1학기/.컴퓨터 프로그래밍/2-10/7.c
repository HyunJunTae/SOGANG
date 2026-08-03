#include <stdio.h>

float calc_x(int a, int b, int c, int d);
float calc_y(float x, int c, int d);


int main(void){
    int a, b, c, d;
    float x, y;
    printf("y = ax - b, Input 'a' and 'b' : ");
    scanf("%d %d",&a,&b);
    printf("y = cx - d, Input 'c' and 'd' : ");
    scanf("%d %d",&c,&d);
    x = calc_x(a,b,c,d);
    y = calc_y(x,c,d);
    printf("x is %f\n", x);
    printf("y is %f\n", y);
    return 0;
}

float calc_x(int a, int b, int c, int d){
    float answer = ((b - d) / (float)(a-c));
    return answer;
}

float calc_y(float x, int c, int d){
    float answer = (float)(c * x - d);
    return answer;
}