#include <stdio.h>

void dec2oct(int*,int*,int*,int*,int);
void dec2hex(char*,char*,char*,int);
char digit2hex(int);

int main(void){
    int num, oct1, oct2, oct3, oct4;
    char hex1, hex2, hex3;
    scanf("%d", &num);
    printf("%.4o\n", num);
    dec2oct(&oct1, &oct2, &oct3, &oct4, num);
    printf("%d%d%d%d\n", oct1, oct2, oct3, oct4);
    printf("%.3X\n", num);
    dec2hex(&hex1, &hex2, &hex3, num);
    printf("%c%c%c\n", hex1, hex2, hex3);
    return 0;
}

void dec2oct(int* a, int* b, int* c, int* d, int num){
    *a = num / (8*8*8);
    num = num - (*a * (8*8*8));
    *b = num / (8*8);
    num = num - (*b * (8*8));
    *c = num / (8);
    num = num - (*c * (8));
    *d = num % 8 ;
    return;
}

void dec2hex(char* a, char* b, char* c, int num){
    *a = digit2hex(num / (16*16));
    num -= ((num / (16*16)) * (16*16));
    *b = digit2hex(num / 16);
    num -= (num / 16) * 16;
    *c = digit2hex(num);
    return;
}

char digit2hex(int num){
    char tmp;
    if (num >= 10){
        tmp = num + 55;
    }else{
        tmp = num + 48;
    }
    return tmp;
}