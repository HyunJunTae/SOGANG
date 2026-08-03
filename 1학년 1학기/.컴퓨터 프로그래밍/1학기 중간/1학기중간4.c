#include <stdio.h>
#include <math.h>

int check(int a, int b, int c);

int main(void){
    int a, b, c, d, e, f;
    scanf("%d %d %d %d %d %d", &a, &b, &c, &d, &e, &f);
    float result1 = (-b + sqrt(b*b - 4*a*c)) / (2 * a);
    float result2 = (-e + sqrt(e*e - 4*d*f)) / (2 * d);
    if (check(a,b,c) == -1 || check(d,e,f) == -1){
        printf("-1\n");
    }else{
        printf("%.6f\n", result1 + result2);
    }
    return 0;
}

int check(int a, int b, int c){
    int D;
    D = (b*b) - (4*a*c);
    if (D >= 0){
        return 1;
    }else{
        return -1;
    }
}
