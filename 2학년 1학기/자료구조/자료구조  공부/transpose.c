#define MAX_TERMS 101
#include <stdio.h>

typedef struct
{
    int row;
    int col;
    int value;
} term;



void transpose(term a[], term b[]) {

    int n, i, j, currentb;
    n = a[0].value; // total number o elements
    b[0].row = a[0].col;
    b[0].col = a[0].row;
    b[0].value = n;


    if (n > 0) {
        currentb = 1;

        for (i=0; i<=a[0].col; i++) {
            for (j=1; j<=n; j++) {
                if (a[j].col == i) {
                    b[currentb].row = a[j].col;
                    b[currentb].col = a[j].row;
                    b[currentb].value = a[j].value;
                    currentb++;
                }
            }
        }
    }

}


void print_terms(term a[]) {
    int k = 1;
    for (int i=0; i<a[0].row; i++) {
        for (int j=0; j<a[0].col; j++) {
            if (k <= a[0].value && a[k].row == i && a[k].col == j) {
                // 저장된 비제로 항이 있는 위치이면 해당 값을 출력
                printf("%d ", a[k].value);
                k++;
            } else {
                // 비저장(즉, 0인) 항이면 0 출력
                printf("0 ");
            }
        }
        printf("\n");
    }
}


int main(void) {

    term a[MAX_TERMS];
    term b[MAX_TERMS];

    a[0].row = 2;
    a[0].col = 3;
    a[0].value = 2;
    a[1].row = 0;
    a[1].col = 0;
    a[1].value = 2;
    a[2].row = 1;
    a[2].col = 1;
    a[2].value = 1;

    print_terms(a);

    transpose(a, b);

    print_terms(b);

    return 0;
}