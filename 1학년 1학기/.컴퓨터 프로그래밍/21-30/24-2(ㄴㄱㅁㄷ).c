#include <stdio.h>

int Fibo(int a, int b, int num) {

	int result;

	do {
		result = a + b;
		b = a;
		a = result;

		num = num - 1;
	}while(num != 0);

	return result;
}





int main(void) {

	int num, result;
	int a = 0;
	int b = 1;

	scanf("%d", &num);

	result = Fibo(a, b, num);

	printf("fibo(%d) : %d\n", num, result);

	return 0;

}
