#include <stdio.h>

void stackFull()
{
    printf("STACK IS FULL\n");
}

void stackEmpty()
{
    printf("STACK IS EMPTY\n");
}

int IsFull(int top, int MAX_STACK_SIZE)
{
    if (top >= MAX_STACK_SIZE - 1)
        return 1;
    return 0;
}

int IsEmpty(int top)
{
    if (top < 0)
        return 1;
    return 0;
}

void push(int *top, char *stack, char item, int MAX_STACK_SIZE)
{
    if (IsFull(*top, MAX_STACK_SIZE))
        stackFull();
    else
        stack[++(*top)] = item;
}

char pop(int *top, char *stack)
{
    if (IsEmpty(*top))
        stackEmpty();
    else
        return stack[(*top)--];
}

int main(void) {

    int MAX_STACK_SIZE = 30;

    // 스택과 top 변수 선언
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

    

    
    // 만약 표준화가 되지 않았다면, 입력한 그대로를 출력
    if (input[0] != '/') {
        printf("%s", input);
        return 0;
    }



    // 맨 처음꺼는 / 이니까 이거 넣고 시작하기
    push(&top, stack, '/', MAX_STACK_SIZE);

    // ///// 이렇게 있으면 / 로 줄이기
    // . 하나만 있으면 무시
    // .. 두 개 있으면 이전으로
    char temp;
    int count;
    for (int i=1; i<input_len; i++) {
        temp = input[i];

        // 이번에 들어온게 / 인데, 이전에 이미 /가 들어왔으면
        // 또 출력할 필요 없이 스킵
        if (temp == '/' && stack[top] == '/') continue;


        // 스택이 /. 인데 / 이 들어오면
        // 입력받은 / 스킵하고 .을 pop
        else if (temp == '/' && stack[top] == '.') {
            pop(&top, stack);
        }

        // 만약에 .이 들어왔는데, 이전에 들어온게 .이면
        // ..인거니까 디렉토리 하나 뺄 때까지 or 스택 빌 때까지 빼내기.
        else if (temp == '.' && stack[top] == '.') {
            // 스택이 /. 인 경우
            if (top == 1) {
                pop(&top, stack); // / 로 만들기
            }

            // 스택이 /home/. 처럼 먼가 들어있는 경우
            else {
                pop(&top, stack);
                pop(&top, stack);
                while(stack[top] != '/') {
                    pop(&top, stack);
                }
            }
        }

        else {
            push(&top, stack, temp, MAX_STACK_SIZE);
        }

    }

    if (stack[top] == '.') pop(&top, stack);
    if (stack[top] == '/') pop(&top, stack);

    if (IsEmpty(top)) {
        printf("%c", '/');
    }

    for (int i=0; i<=top; i++) {
        printf("%c", stack[i]);
    }

    printf("\n");

    return 0;
}