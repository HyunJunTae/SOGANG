#include <stdio.h>

int btoten(int num, int b) {

	int count = 1;
	int a = num;
	int c = 0;
	int d = 0;

	while (num != 0 ) {
		c = num % 10;
		d = d + c*count;
		count = count*b;
		num = num / 10;
	}

	return d;
}

int tentob(int num, int b) {

	int count = 1;
	int a = num;
	int c;
	int result = 0;

	while ( a != 0 ) {

		a = a / b;
		count = count*10;
	}
	// while (a > count) {
	// 	count = count * 10;
	// }
	// 둘 은 다름. 그냥 count에 10씩 곱해가며 a랑 count 비교하는게 아니라, a를 b로 몇 번 나눠지는지 보는 것.
	// 예시 : 12의 경우 12 6 3 1 0 4번. 4번 하면 count는 10000이 됨. 12는 이진수로 1100
	count = count / 10;            // 그래서 10으로 한 번 나눠줌. 그래야 수가 맞음.
	
	int digit = 1;

	while ( count >= digit ) {
		c = num % b;
		result = result + c*digit;
		digit = digit*10;

		num = num / b;
	}

	return result;
}


int main(void) {

	int x, y, b, result;

	scanf("%d %d %d", &x, &y, &b);

	int a1, b1;

	a1 = btoten(x,b);
	b1 = btoten(y,b);

	printf("%d + %d = %d\n", x, y, tentob(a1+b1, b));

	return 0;
}
