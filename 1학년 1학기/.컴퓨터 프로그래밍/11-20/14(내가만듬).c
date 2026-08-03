#include <stdio.h>
#include <math.h>

int main(void) {
    int a, b;

    printf("Enter the year to be tested : ");
    scanf("%d", &a);

    if (a % 4 == 0){ // 4로 나누어떨어지는 경우 2000 2004 2096 2104 2100 2200 2300 2400
        if (a % 100 == 0 && a % 400 != 0) { //100의 배수이며 400의 배수가 아닌 윤년X경우 2100 2200 2300
            printf("%d is not a leap year\n", a); 
            printf("%d is a leap year\n", a-4);

        }
        else { // 위 경우 외의 모든 경우 2000 2004 2096 2104 2400
            printf("%d is a leap year\n", a);
        }
    }


    else { // 4로 나누어떨어지지 않는 경우 2099 2101 2102 2103 
        printf("%d is not a leap year\n", a);
        b = a - a % 4;  //4로 나누어떨어지도록 조정

        if (b % 100 == 0 && b % 400 != 0) { //조정된 년도 b가 100의 배수이며 
            b = b - 4;
            printf("%d is a leap year\n", b);
        }

        else {
            printf("%d is a leap year\n", b);
        }  
    }




    return 0;

}

// 윤년인 경우 2000 2004 2096 2104 2400
// 윤년X 경우  2099 2100 2101 2102 2103 2200 2300