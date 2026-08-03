#include <stdio.h>
#include <stdlib.h>
#define MAX_TERMS 1000  

typedef struct {
    float coef;
    int expon;
} polynomial;

polynomial terms[MAX_TERMS];
int avail;

void readPoly(int *start_a, int *finish_a, int *start_b, int *finish_b)
{

    // a 배열 입력
    int poly_size;
    float temp_coef;
    int temp_exp;
    printf("Input polynomial : \n");
    scanf("%d", &poly_size);

    *start_a = 0;
    *finish_a = poly_size-1;

    for (int i=*start_a; i<=*finish_a; i++) {
        scanf("%f %d", &temp_coef, &temp_exp);
        terms[i].coef = temp_coef;
        terms[i].expon = temp_exp;
    }



    // b 배열 입력
    printf("Input polynomial : \n");
    scanf("%d", &poly_size);
    
    *start_b = *finish_a+1;
    *finish_b = *start_b + poly_size - 1;

    for (int i=*start_b; i<=*finish_b; i++) {
        scanf("%f %d", &temp_coef, &temp_exp);
        terms[i].coef = temp_coef;
        terms[i].expon = temp_exp;
    }

    return;
}

void printPoly(int start_c, int finish_c) {

    for (int i=start_c; i<=finish_c; i++) {

        // 만약 coef가 0이면 건너뛰기
        if (terms[i].coef == 0) continue;

        // 만약 x^0이면, 즉 상수항이면 숫자만 출력
        if (terms[i].expon == 0) printf("%.0f", terms[i].coef);
        else printf("%.0fx^%d", terms[i].coef, terms[i].expon);

        if (terms[i+1].coef > 0) printf("+");
    }
    
    printf("\n");
    return;
}

void pMult(int start_a, int finish_a, int start_b, int finish_b, int *start_c, int *finish_c) {

    // coef끼리는 곱하고, exp끼리는 더해서 뒤에 추가.

    *start_c = finish_b + 1;
    avail = *start_c;

    float new_coef;
    int new_exp;
    for (int i=start_a; i<=finish_a; i++) {
        for (int j=start_b; j<=finish_b; j++) {

            if (avail >= MAX_TERMS) {
                printf("Too many in the polynomial");
                return;
            }

            // new_exp가 term안에 있는지 없는지를 판별하는 변수 0으로 초기화
            int exp_is_already_in_term = 0;

            // 각 항을 계산
            new_coef = terms[i].coef * terms[j].coef;
            new_exp = terms[i].expon + terms[j].expon;

            // 계산한 항을 새로운 배열에 추가.
            // 같은 expon이 이미 있으면, coef를 덧셈연산.
            for (int k=*start_c; k<avail; k++) {
                if (terms[k].expon == new_exp) {
                    terms[k].coef += new_coef;
                    exp_is_already_in_term = 1;
                    break;
                }
            }

            // 해당 expon이 term_3에 없으면, 새로 추가.
            if (exp_is_already_in_term == 0) {
                terms[avail].coef = new_coef;
                terms[avail].expon = new_exp;
                avail++;
            }
        }
    }

    *finish_c = avail-1;

    // 내림차순으로 정렬
    for (int i = *start_c; i < *finish_c; i++) {
        for (int j = i + 1; j <= *finish_c; j++) {
            if (terms[i].expon < terms[j].expon) {
                polynomial temp = terms[i];
                terms[i] = terms[j];
                terms[j] = temp;
            }
        }
    }

    return;
}

int main(void) {
    // coef expon 순서로 입력.

    int start_a, finish_a, start_b, finish_b, start_c, finish_c;

    readPoly(&start_a, &finish_a, &start_b, &finish_b);

    pMult(start_a, finish_a, start_b, finish_b, &start_c, &finish_c);

    printPoly(start_c, finish_c);

    return 0;
}