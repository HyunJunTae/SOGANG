#include <stdio.h>

void checkWB(char col, int row, char *answer);

int main(void){
    char col, answer;
    int row;
    scanf("%c%d", &col, &row);
    checkWB(col, row, &answer);
    printf("answer : %c\n", answer);
    return  0;
}

void checkWB(char col, int row, char *answer){
    if (((int)col-97) % 2 == 0 && row % 2 == 0){
            *answer = 'W';
            return;
        }
    if (((int)col-97) % 2 == 1 && row % 2 == 1){
        *answer = 'W';
        return;
    }
    else{
         *answer = 'B';
         return;
    }
}