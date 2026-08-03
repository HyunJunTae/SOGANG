#include <stdio.h>

int num1 = 0;

void baesu1(int a, int b) {

	int i;

	for (i = a; i <= b; i++) {
		if (i % 2 == 0 || i % 3 == 0) {
			num1 += i;
		}
	}
}

int main(void) {

	int a, b, c;

	printf("Input first number : ");
	scanf("%d", &a);
	if (a < 1) {
		printf("Input is less than 1\n");
		return 0;
	}

	printf("Input second number : ");
	scanf("%d", &b);
	if (b < 1) {
			printf("Input is less than 1\n");
			return 0;
        }

	if (a > b) {
		c = a;
		a = b;
		b = c;
	}

	baesu1(a, b);

	printf("Result : %d\n", num1);

	return 0;
}
