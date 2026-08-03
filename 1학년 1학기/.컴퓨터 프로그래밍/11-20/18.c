#include <stdio.h>

void basic_fee(int elect, float *pay);
void elect_fee(int elect, float *pay);

int main(void){
    int elect;
    float pay;
    printf("Input your elect : ");
    scanf("%d", &elect);
    basic_fee(elect, &pay);
    elect_fee(elect, &pay);
    printf("Pay : %d\n", (int)pay);
    return 0;
}

void basic_fee(int elect, float *pay){
    if (elect > 400){
        *pay = 7300;
    }else if(elect >= 201){
        *pay = 1600;
    }else{
        *pay = 910;
    }
}

void elect_fee(int elect, float *pay){
    if(elect > 400){
        *pay += (elect-400) * 280.6;
        *pay += 200 * 187.9;
        *pay += 200 * 93.3;
    }else if(elect >= 201){
        *pay += (elect-200) * 187.9;
        *pay += 200 * 93.3;
    }else{
        *pay += elect * 93.3;
        *pay -= 4000;
        if (*pay < 1000){
            *pay = 1000;
        }
    }
}