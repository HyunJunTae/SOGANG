#include <stdio.h>

void stackFull() {
    printf("STACK IS FULL\n");
}

void stackEmpty() {
    printf("STACK IS EMPTY\n");
}

int IsFull(int top, int MAX_STACK_SIZE) {
    if(top >= MAX_STACK_SIZE-1) return 1;
    return 0;
}

int IsEmpty(int top) {
    if(top < 0) return 1;
    return 0;
}

void push(int *top, char *stack, char item, int MAX_STACK_SIZE)
{
    if (IsFull(*top, MAX_STACK_SIZE)) stackFull();
    else stack[++(*top)] = item;
}

char pop(int *top, char* stack)
{
    if (IsEmpty(*top)) stackEmpty();
    else return stack[(*top)--];
}



int main(void) {

    int MAX_STACK_SIZE = 21;

    char correct = 'T';

    // 검사를 괄호 위해 담아둘 스택과 top 변수
    char stack[MAX_STACK_SIZE];
    int top = -1;

    // input 저장할 배열 선언 및 입력
    char input[MAX_STACK_SIZE];
    scanf("%s", input);

    // 입력 배열의 길이 재기
    int input_len = 0;
    for (int i=0; i<MAX_STACK_SIZE; i++) {
        if (input[i] == '\0') break;
        input_len++;
    }

    for (int i=0; i<input_len; i++) {
        // printf("%c", input[i]);

        switch (input[i])
        {
        case '(' :
            push(&top, stack, '(', MAX_STACK_SIZE);
            continue;
        case '{':
            push(&top, stack, '{', MAX_STACK_SIZE);
            continue;
        case '[':
            push(&top, stack, '[', MAX_STACK_SIZE);
            continue;
        case ')':
            if (pop(&top, stack) == '(') continue;
            correct = 'F';
            break;
        case '}':
            if (pop(&top, stack) == '{') continue;
            correct = 'F';
            break;
        case ']':
            if (pop(&top, stack) == '[') continue;
            correct = 'F';
            break;
        }
    }

    printf("%c\n", correct);

    return 0;
}