#include <stdio.h>
#include <stdlib.h>

typedef struct polynomial* polyPointer;
typedef struct polynomial {
    int coef;
    int exp;
    polyPointer link;
} poly;


void pread (polyPointer *ptr, polyPointer node);
void pwrite(polyPointer ptr);
void padd(polyPointer *ptrA, polyPointer *ptrB);
void psub(polyPointer *ptrA, polyPointer *ptrB);
void pmult(polyPointer *ptrA, polyPointer *ptrB, int n1, int n2);
void eval(polyPointer *ptrA, polyPointer *ptrB, int n1, int n2);
void perase(polyPointer *ptr);



void pread (polyPointer *ptr, polyPointer node) {

    int coef, exp; // coef, exp 입력
    scanf("%d %d", &coef, &exp);

    polyPointer temp = (polyPointer)malloc(sizeof(poly)); // 임시 노드 선언

    temp->coef = coef; // 임시 노드에 데이터 설정.
    temp->exp = exp;

    temp->link = (*ptr)->link;
    (*ptr)->link = temp;

    (*ptr) = temp;

    return;
}

void pwrite(polyPointer ptr) {

    printf("result: ");

    polyPointer temp = ptr->link; // 임시 노드 선언
    int isfirst = 1;

    do {
        if (temp->coef == 0) continue; // coef 가 0 이면 출력하지 않고 넘어가기

        if (temp->coef > 0 && isfirst == 0) {
            printf("+ ");
        }

        if (temp->exp == 0) { // exp가 0이면  coef 만 출력. x 문자 출력 안함.
            printf("%d ", temp->coef); // 출력
        }
        else {
            printf("%dx^%d ", temp->coef, temp->exp); // 출력
        }

        isfirst = 0;
        temp = temp->link; // temp를 다음 노드로 설정.

    } while(temp!= ptr);

    printf("\n\n");

    return;
}

void padd(polyPointer *ptrA, polyPointer *ptrB) {

    int temp_coef, temp_exp;
    polyPointer tempA = *ptrA;
    tempA = tempA->link;
    polyPointer tempB = *ptrB;
    tempB = tempB->link;
    
    polyPointer headC = (polyPointer)malloc(sizeof(poly));; // headC 선언.
    headC->coef = -1;
    headC->exp = -1;
    headC->link = headC;
    polyPointer temp = headC;
    // 반복문을 이용해 A와 B의 coef 와 exp 비교해가며 한 칸씩 나아가며 계산.
    do {
        if (tempA->exp == tempB->exp) { // exp 가 같은 경우 둘 다 더해주기
            temp_coef = tempA->coef + tempB->coef;
            temp_exp = tempA->exp;
            tempA = tempA->link;
            tempB = tempB->link;
        }
        else if (tempA->exp > tempB->exp) { // tempA의 exp 가 더 큰 경우 tempA만
            temp_coef = tempA->coef;
            temp_exp = tempA->exp;
            tempA = tempA->link;
        }
        else { // tempB의 exp 가 더 큰 경우 tempB만
            temp_coef = tempB->coef;
            temp_exp = tempB->exp;
            tempB = tempB->link;
        }

        printf("%d %d\n", temp_coef, temp_exp);

        if (temp_coef != 0) { // coef 가 0 이면 continue

            polyPointer newNode = (polyPointer)malloc(sizeof(poly)); // 새 노드 선언
            newNode->coef = temp_coef;
            newNode->exp = temp_exp;

            newNode->link = temp->link;
            temp->link = newNode;

            temp = newNode;
        }

    } while(tempA != *ptrA && tempB != *ptrB);

    // A 와 B 중 한 쪽이 먼저 끝났다면 남은 쪽 출력
    while (tempA != *ptrA) {
        polyPointer newNode = (polyPointer)malloc(sizeof(poly)); // 새 노드 선언
        newNode->coef = tempA->coef;
        newNode->exp = tempA->exp;

        newNode->link = temp->link;
        temp->link = newNode;

        temp = newNode;

        tempA = tempA->link;
    }

    while (tempB != *ptrB) {
        polyPointer newNode = (polyPointer)malloc(sizeof(poly)); // 새 노드 선언
        newNode->coef = tempB->coef;
        newNode->exp = tempB->exp;

        newNode->link = temp->link;
        temp->link = newNode;

        temp = newNode;

        tempB = tempB->link;
    }

    pwrite(headC);

    perase(&headC);

    return;
}

void psub(polyPointer *ptrA, polyPointer *ptrB) {

    int temp_coef, temp_exp;
    polyPointer tempA = *ptrA;
    tempA = tempA->link;
    polyPointer tempB = *ptrB;
    tempB = tempB->link;
    
    polyPointer headC = (polyPointer)malloc(sizeof(poly));; // headC 선언.
    headC->coef = -1;
    headC->exp = -1;
    headC->link = headC;
    polyPointer temp = headC;
    // 반복문을 이용해 A와 B의 coef 와 exp 비교해가며 한 칸씩 나아가며 계산.
    do {
        if (tempA->exp == tempB->exp) { // exp 가 같은 경우 둘 다 더해주기
            temp_coef = tempA->coef - tempB->coef;
            temp_exp = tempA->exp;
            tempA = tempA->link;
            tempB = tempB->link;
        }
        else if (tempA->exp > tempB->exp) { // tempA의 exp 가 더 큰 경우 tempA만
            temp_coef = tempA->coef;
            temp_exp = tempA->exp;
            tempA = tempA->link;
        }
        else { // tempB의 exp 가 더 큰 경우 tempB만
            temp_coef = tempB->coef * (-1);
            temp_exp = tempB->exp;
            tempB = tempB->link;
        }

        printf("%d %d\n", temp_coef, temp_exp);

        if (temp_coef != 0) { // coef 가 0 이면 continue

            polyPointer newNode = (polyPointer)malloc(sizeof(poly)); // 새 노드 선언
            newNode->coef = temp_coef;
            newNode->exp = temp_exp;

            newNode->link = temp->link;
            temp->link = newNode;

            temp = newNode;
        }

    } while(tempA != *ptrA && tempB != *ptrB);

    // A 와 B 중 한 쪽이 먼저 끝났다면 남은 쪽 출력
    while (tempA != *ptrA) {
        polyPointer newNode = (polyPointer)malloc(sizeof(poly)); // 새 노드 선언
        newNode->coef = tempA->coef;
        newNode->exp = tempA->exp;

        newNode->link = temp->link;
        temp->link = newNode;

        temp = newNode;

        tempA = tempA->link;
    }

    while (tempB != *ptrB) {
        polyPointer newNode = (polyPointer)malloc(sizeof(poly)); // 새 노드 선언
        newNode->coef = tempB->coef * (-1);
        newNode->exp = tempB->exp;

        newNode->link = temp->link;
        temp->link = newNode;

        temp = newNode;

        tempB = tempB->link;
    }

    pwrite(headC);

    perase(&headC);

    return;
    
}

void pmult(polyPointer *ptrA, polyPointer *ptrB, int n1, int n2) {

    int temp_coef, temp_exp;

    polyPointer tempA = *ptrA;
    tempA = tempA->link;
    polyPointer tempB = *ptrB;
    tempB = tempB->link;
    polyPointer headC;
    headC->coef = -1;
    headC->exp = -1;
    headC->link = headC;

    // 계산
    for (int i=0; i<n1; i++) {
        for (int j=0; j<n2; j++) {

            // temp coef, temp exp 계산
            temp_coef = tempA->coef * tempB->coef;
            temp_exp = tempA->exp + tempB->exp;
            printf("temp_coef: %d, temp_exp: %d\n", temp_coef, temp_exp); // ("te")


            // 만약 temp exp가 이미 C에 있다면 tempC->coef 에 더해주기
            polyPointer temp = headC->link;
            int isfind = 0;
            while(temp != headC) {
                if (temp->exp == temp_exp) {
                    temp->coef += temp_coef;
                    isfind = 1;
                    break;
                }
                temp = temp->link;
            }

            // 만약 temp exp가 C에 없다면 C에 추가하기
            if (isfind == 0) {
                
                polyPointer newNode = (polyPointer)malloc(sizeof(poly)); // 새 노드 선언
                newNode->coef = temp_coef;
                newNode->exp = temp_exp;
                
                // 만약 headC에 -1 -1 만 있다면
                if (headC->link == headC) {
                    newNode->link = headC;
                    headC->link = newNode;

                    tempB = tempB->link;
                    continue;;
                }

                // headC에 들어있는게 2개 이상이라면
                temp = headC;
                while (temp->link != headC && temp->link->exp > temp_exp) temp = temp->link;

                // 링크드 리스트의 맨 마지막에 도달하거나 / temp->link 의 exp 가 temp_exp보다 작아졌다면, 노드 추가
                newNode->link = temp->link;
                temp->link = newNode;

            }
            tempB = tempB->link;
            continue;
        }
        tempA = tempA->link;
        tempB = tempB->link;
    }
    tempA = tempA->link;

    pwrite(headC);

    perase(&headC);

    return;
}

void eval(polyPointer *ptrA, polyPointer *ptrB, int n1, int n2) {

    polyPointer tempA = *ptrA;
    tempA = tempA->link;
    polyPointer tempB = *ptrB;
    tempB = tempB->link;

    // x값 입력
    int x;
    printf("Enter x value: ");
    scanf("%d", &x);

    // a 계산
    int sum = 0;
    int temp_x = 1;
    for (int i=0; i<n1; i++) {

        for (int i=0; i<tempA->exp; i++) temp_x *= x;

        sum += tempA->coef * temp_x;

        temp_x = 1;
        tempA = tempA->link;
    }
    printf("A(%d) = %d\n", x, sum);

    // b 계산
    sum = 0;
    for (int i=0; i<n2; i++) {

        for (int i=0; i<tempB->exp; i++) temp_x *= x;
        
        sum += tempB->coef * temp_x;

        temp_x = 1;
        tempB = tempB->link;
    }

    printf("B(%d) = %d\n", x, sum);

}

void perase(polyPointer *ptr) {

    polyPointer temp = *ptr;
    polyPointer next = *ptr;

    *ptr = (*ptr)->link;
    next->link = NULL;

    do {
        temp = *ptr;
        *ptr = (*ptr)->link;
        free(temp);

    } while(*ptr);

    return;
}

int main(void) {

    polyPointer head_A = (polyPointer)malloc(sizeof(poly));;
    head_A->coef = -1;
    head_A->exp = -1;
    head_A->link = head_A;
    polyPointer head_B = (polyPointer)malloc(sizeof(poly));;
    head_B->coef = -1;
    head_B->exp = -1;
    head_B->link = head_B;
    int n1, n2;

    // polyA polyB 입력
    printf("Enter number of terms in polynomiad A: ");
    scanf("%d", &n1);
    
    for (int i = 0; i < n1; i++) {
        polyPointer newNode; // 새 노드 선언
        printf("Enter terms for A (coef exp)");
        pread(&head_A, newNode); // pread 함수 호출      
    }

    head_A = head_A->link; // head_A 를 첫번째 노드로 설정.

    printf("Enter number of terms in polynomiad B: ");
    scanf("%d", &n2);
    
    for (int i = 0; i < n2; i++) {
        polyPointer newNode; // 새 노드 선언
        printf("Enter terms for B (coef exp)");
        pread(&head_B, newNode); // pread 함수 호출     
    }

    head_B = head_B->link; // head_B 를 첫번째 노드로 설정.


    printf("headA : %d %d\n", head_A->coef, head_A->exp);
    printf("headB : %d %d\n", head_B->coef, head_B->exp);

    
    
    int operation;
    do {

    printf("Choose operation:\n");
    printf("1. Addition\n");
    printf("2. Subtraction\n");
    printf("3. Multiplication\n");
    printf("4. Evaluation\n");
    printf("5. Quit\n");

    printf("Enter choice: ");
    scanf("%d", &operation);

    if (operation == 1) { // add
        padd(&head_A, &head_B);
    }
    else if (operation == 2) { // sub
        psub(&head_A, &head_B);
    }
    else if (operation == 3) { // mult
        pmult(&head_A, &head_B, n1, n2);
    }
    else if (operation == 4) { // eval
        eval(&head_A, &head_B, n1, n2);
    }
    else if (operation == 5) { // erase
        perase(&head_A);
        perase(&head_B);
        printf("Good Bye");
    }

    } while(operation != 5);

    return 0;
    
}