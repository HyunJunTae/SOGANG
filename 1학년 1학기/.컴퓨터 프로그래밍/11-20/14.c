#include <stdio.h>

int check_onetime = 0;

int checkLeapYear(int year){
    if (year % 4 == 0 && year % 100 != 0){
        printf("%d is a leap year\n", year);
        return 0;
    }
    else if (year % 400 == 0 ){
        printf("%d is a leap year\n", year);
        return 0;
    }
    else if (check_onetime == 0){
        printf("%d is not a leap year.\n", year);
        check_onetime = 1;
    }
    checkLeapYear(--year);
}

int main(void){
    int year;
    printf("Enter the year to be tested : ");
    scanf("%d", &year);
    checkLeapYear(year);
    return 0;
}