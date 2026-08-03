#include <stdio.h>
#include <string.h>

#define MAX_STACK_SIZE 100 /*maximum stack size*/
#define MAX_EXPR_SIZE 100  /*max size of expression*/
typedef enum
{
    lparen, // 0
    rparen, // 1
    plus,
    minus,
    times,
    divide,
    mod,
    eos,
    operand // 8
} precedence;
int stack[MAX_STACK_SIZE]; /* global stack */
char expr[MAX_EXPR_SIZE];  /* input string */
int top = -1;

int isp[] = {0, 19, 14, 12, 12, 13, 13, 13, 0};
int icp[] = {20, 19, 14, 12, 12, 13, 13, 13, 0};

void stackFull()
{
    printf("STACK IS FULL");
}

void stackEmpty()
{
    printf("STACK IS EMPTY");
}

void push(precedence item)
{
    /* add an item to the global stack */
    if (top >= MAX_STACK_SIZE - 1)
        stackFull();
    else
        stack[++top] = item;
}

precedence pop()
{
    /* delete and return the top element from the stack */
    if (top == -1)
        stackEmpty(); /* returns an error key */
    else
        return stack[top--];
}

void printToken(precedence token)
{
    switch (token)
    {
    case lparen:
        printf("(");
        break;
    case rparen:
        printf(")");
        break;
    case plus:
        printf("+");
        break;
    case minus:
        printf("-");
        break;
    case divide:
        printf("/");
        break;
    case times:
        printf("*");
        break;
    case mod:
        printf("%");
        break;
    case eos:
        printf("Null");
        break;
    default:
        break;
    }
}

precedence getToken(char *symbol, int *n)
{
    /* get the next token, symbol is the character representation, which is returned, the
    token is represented by its enumerated value, which is returned in the function name */
    *symbol = expr[(*n)++];
    switch (*symbol)
    {
    case '(':
        return lparen;
    case ')':
        return rparen;
    case '+':
        return plus;
    case '-':
        return minus;
    case '/':
        return divide;
    case '*':
        return times;
    case '%':
        return mod;
    case ' ':
        return eos;
    default:
        return operand; /* no error checking, default is operand */
    }
}

// 1. infix를 postfix로 변환하기
// 숫자는 그대로 출력하고, operator를 스택에 넣음.
// operator를 스택에 넣을 때, enum값으로 넣음. 즉 정수값으로 넣음
void postfix(void)
{
    /* output the postfix of the expression. The expression string, stack, and the top are global */
    char symbol;
    int n = 0;
    int top = 0;
    precedence token;
    stack[0] = eos;
    for (token = getToken(&symbol, &n); token != eos; token = getToken(&symbol, &n))
    {
        if (token == operand)
            printf("% c", symbol);

        else if (token == rparen)
        {
            /* unstack tokens until left parenthesis */
            while (stack[top] != lparen)
                printToken(pop());
            pop(); /* discard the left parenthesis */
        }

        // else if (token == minus && stack[top] == lparen) {
        //     push()
        // }

        else
        {
            /* remove and print symbols whose isp is greater
            than or equal to the current token’s icp */
            while (isp[stack[top]] >= icp[token])
                printToken(pop());
            push(token);
        }
    }
    while ((token = pop()) != eos)
        printToken(token);
    printf("\n");
}

// 2. postfix 식을 계산하기
// 숫자를 스택에 넣어주고, operator가 나올 때마다 스택의 숫자 빼내서 계산한 다음 다시 스택으로.
int eval(void)
{
    /* evaluate a postfix expression, expr, maintained as a global variable. ‘\0’ is the end of the
    expression. The stack and top of the stack are global variables. getToken is used to
    return the tokentype and the character symbol. Operands are assumed to be single
    character digits */
    precedence token;
    char symbol;
    int op1, op2;
    int n = 0; /* counter for the expression string */
    token = getToken(&symbol, &n);

    while (token != eos)
    {
        if (token == operand)
            push(symbol - '0'); /* stack insert */
        else
        {
            /* remove two operands, perform operation, and return result to the stack */
            op2 = pop(); /* stack delete */
            op1 = pop();
            switch (token)
            {
            case plus:
                push(op1 + op2);
                break;
            case minus:
                push(op1 - op2);
                break;
            case times:
                push(op1 * op2);
                break;
            case divide:
                push(op1 / op2);
                break;
            case mod:
                push(op1 % op2);
            }
        }
        token = getToken(&symbol, &n);
    }
    return pop(); /* return result */
}

int main(void)
{

    // printf("%d", lparen);

    printf("Input: ");
    scanf("%s", expr);

    // for (int i=0; i<strlen(my_input); i++) {
    //     printf("%c", my_input[i]);
    // }

    postfix();

    return 0;
}