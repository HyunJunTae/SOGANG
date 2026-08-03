#include <stdio.h>
#include <math.h>

int main(void){
    float real;
    printf("Input : ");
    scanf("%f", &real);
    printf("Use \">\" : %d\n", round(real) - ceil(real) > -0.1);
    printf("Use \">=\" : %d\n", round(real) >= ceil(real));
    printf("Use \"==\" : %d\n", ceil(real) == round(real));
    printf("Use \"!=\" : %d\n", floor(real) != round(real));
    return 0;
}