#include <stdio.h>

double char_to_double(char *x);

int main(void){
    char x[10]; // 10칸인데 1234.123456 입력하면 11칸 아닌가 왜 되는거지
    char c;
    int i = 0;
    do{
        scanf("%c", &c);
        x[i++] = c;
    }while(c!='\n');
    printf("%4.6f\n",char_to_double(x));
    return 0;
}

double char_to_double(char *x){
    double sign = 1, data = 0, point = 0, decimal_position = 1;
    if (*x == '-'){
        sign = -1;
    }
    while(*x != '\n'){
        if(*x >= '0' && *x <= '9' && point == 0){
            data = data*10 + *x - '0'; // *x의 데이터는 문자형. 연산하면 아스키값으로 되기때문에 0의 아스키값 빼주면 원래 숫자 나옴.
        }
        if(*x=='.'){
            point = 1;
            *x++;
            continue;
        }
        if(point != 0){
            decimal_position *= 0.1;
            data = data + (*x - '0') * decimal_position;
        }
        *x++;
    }
    return sign * data;
}

// x는 일차원배열. 그냥 x 하면 x의 0번항 의미?
// x하면 일차원배열이라 주소값이니까 *q붙여서 데이터값 사용가능.
// x[0] x[1] x[2] 하듯이 *x *x+1 *x+2 or *(x) *(x+1) *(x+2)
// *x는 x의 0번째 항 의미. *x++ 해주면서 하나하나씩 봐주기.









// #include <stdio.h>

// double char_to_double(char *x);

// int main(void){
//     char x[10];
//     char c;
//     int i = 0;
//     do{
//         scanf("%c", &c);
//         x[i++] = c;
//     }while(c!='\n');
//     printf("%4.6f\n",char_to_double(x));
//     return 0;
// }


// double char_to_double(char *x){
//     int i=0;
//     double sign = 1, data = 0, point = 0, decimal_position = 1;
//     if (*x+i == '-'){
//         sign = -1;
//     }
//     while(*x+i != '\n'){
//         if(*x+i >= '0' && *x+i <= '9' && point == 0){
//             data = data*10 + *x+i - '0';
//         }
//         if(*x+i=='.'){
//             point = 1;
//             i = i + 1;
//             continue;
//         }
//         if(point != 0){
//             decimal_position *= 0.1;
//             data = data + (*x+i - '0') * decimal_position;
//         }
//         i = i + 1;
//     }
//     return sign * data;
// }