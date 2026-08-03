#include <stdio.h>

float avgFunc(float a, float b, float c);
void gradeFunc(float avg);


int main(void){
    float score1, score2, score3;
    printf("input : ");
    scanf("%f %f %f",&score1, &score2, &score3);
    float avg = avgFunc(score1, score2, score3);
    gradeFunc(avg);
    return 0;
}

float avgFunc(float a, float b, float c){
    return (a+b+c)/3;
}

void gradeFunc(float avg){

    if (avg >= 90) {
        printf("A");
        return;
    }
    if (avg >= 80) {
        printf("B");
        return;
    }
    if (avg >= 70) {
        printf("C");
        return;
    }
    if (avg >= 50) {
        printf("D");
        return;
    }
    if (avg < 50) {
        printf("E");
        return;
    }
    return;
}