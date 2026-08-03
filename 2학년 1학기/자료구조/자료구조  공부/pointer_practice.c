//&a, &b의 값을 int x, int y 로 받으면 x와 y에는 그저 a와 b의 주소가 "정수형"으로 저장될 뿐이고,
// 거기서 주소값을 서로 변경해봤자 해당 함수 내에서 주소값이 바뀔 뿐 main 으로는 아무 영향을 미치지 못한다

// 그래서 특정 변수의 주소를 직접 가리키는 변수가 필요한거고, 그게 포인터.

// 그러한 포인터를 사용하기 위해서 *을 붙이기로 약속했다.

#include <stdio.h>

// swap 코드에서
// x와 y는 주소를 담는 포인터. *x, *y는 주소의 값을 나타냄.
// main 코드에서
// a 와 b는 주소의 값을 나타냄. &a와 &b 는 주소를 나타냄.
void swap(int *x, int *y)
{
    int temp = *x;

    *x = *y;

    *y = temp;

    return;
}

// 매크로는 텍스트 치환. C 컴파일러가 실제 코드를 처리하기 전에 텍스트를 치환하는 방식
// 즉, main에서 SWAP(a, b, t); 를 실행하면
// t = a;
// a = b;
// b = t;
// 가 실행된다.
// x, y, t 라는 매개변수로 코드가 따로 진행됐다가 main으로 돌아오는게 아니라, x, y, t에 a, b, t가 그대로 대입되어서 실행되는 것.
#define SWAP(x, y, t) ((t) = (x), (x) = (y), (y) = (t))

// 함수는 매개변수를 사용. main에서 a, b, t를 매개변수로 복사하여 swap 함수로 전달.
void swap(int x, int y)
{
    int t = x;
    x = y;
    y = t;
}

int main(void)
{

    // int list[5] = {1, 2, 3, 4, 5};
    // printf("%d\n", list);
    // printf("%d\n", list[0]);
    // printf("%d\n", list[1]);
    // printf("%d\n", list[2]);

    int a = 1;
    int b = 2;
    int t = 0;

    printf("%d\n", a);
    printf("%d\n", b);

    swap(&a, &b);
    printf("after swap \n");

    printf("%d\n", a);
    printf("%d\n", b);

    SWAP(a, b, t);
    printf("after SWAP \n");

    printf("%d\n", a);
    printf("%d\n", b);

    return 0;
}