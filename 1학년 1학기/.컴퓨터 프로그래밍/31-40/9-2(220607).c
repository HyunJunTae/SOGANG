#include <stdio.h>



int *calc(char op, int* no1, int* no2) {

	if (op == '+') {
		*no1 = (*no1 + *no2);
	}
	else if (op == '-') {
		*no1 = (*no1 - *no2);
	}
	else if (op == '*') {
        *no1 = (*no1 * *no2);
        }
	else if (op == '/') {
        *no1 = (*no1 / *no2);
        }
	return no1;

}
// 연산자 비교할 때 "" 쓰면 안되고 ''쓰면 됨. 왜지..?

int main(void) {

	int no1 = 100;

	char op;
	printf("input operation : ");
	scanf("%c", &op);

	int no2;
	printf("input Integer : ");
	scanf("%d", &no2);

	int *result;
	result = calc(op, &no1, &no2);

	printf("result : %d", *result);

	return 0;
}




// int add(int *x, int *y) {
//     *x = *x + *y;
//     return *x;
// }

// int multi(int x, int y) {
//     x = x + y;
//     return x;
// }

// int main(void) {

//     int a = 100;
//     int b = 200;
//     int c = 300;
//     int d = add(&a, &b);
//     int e = multi(b, c);

//     printf("%d %d %d %d %d", a, b, c, d, e);

//     return 0;
// }

// add에서는 a와 b의 주소를 가져감. 주소가 그대로 남아있음 함수 안에서도.
// 주소 가져가서 역참조연산자 *로 실제값으로 되돌려서 계산.
// 그렇게 계산한다 해도 주소가 그대로 있으니까 *a의 값이 바뀌면 주소안의 값 자체가 바뀜.
// return 해줄때도 실제값을 return해야함 왜냐하면 함수가 int형이니까. 그래서 *붙여서 실제값으로 return.
// multi에서는 b와 c의 값만을 가져감. 그냥 실제 값 자체를 가져가니까 역참조연산자 안써야함.
// 그냥 b랑 c의 숫자만 가져간거니까 위에서 계산해도 당연히 실제값은 안바뀜.
// 여기서 원래 문제의 int *calc와 비교. 이거는 포인터 함수임. return값 자체가 주소값이 되어야함.
// 맨처음에 값 가져올때부터 result = calc(op, &no1, &no2) 로 넘버1이랑 넘버2는 주소값을 가져옴.
// 주소값을 가져갔으니까 int *no1 이런식으로 실제값으로 바꿔줘야하고 계산할때도 *no1 이런식으로 실제값 가져와야함.
// 근데 중요한부분. 이거는 함수 자체가 포인터함수임. return시킬 때 return시키는 것이 주소값이어야함. 그냥 숫자값이 아니라.
// 가져올때부터 no1 no2 주소값으로 가져왔다했지 no1 no2는 주소값이야. 그래서 return no1으로 해야함.