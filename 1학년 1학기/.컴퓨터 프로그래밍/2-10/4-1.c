#include <stdio.h>

int main(){
    int birth,curr_year;
    printf("Birthday : ");
    scanf("%d", &birth);
    printf("Your birthday is %d / %d / %d\n",birth/10000,birth/100-(birth/10000)*100, birth%100);
    printf("Year : ");
    scanf("%d", &curr_year);
    printf("In %d, your age is %d\n",curr_year,1 + curr_year - birth/10000);
    return 0;
}