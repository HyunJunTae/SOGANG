#include <stdio.h>


void func_gcd(int a, int b, int* gcd) {

	int c = 1;

	// while (a % b != 0) {
	
	// 	a = a % b;

	// 	c = a;
	// 	a = b;
	// 	b = c;
	// }
	// *gcd = b;

	while ( c != 0 ) {

		c = a % b;
		a = b;
		b = c;
	}

	*gcd = a;

	// 두 가지 다 가능.
}

int main(void) {

	int a, b, gcd;

	printf("Input fisrt number : ");
	scanf("%d", &a);

	printf("Input second number : ");
        scanf("%d", &b);

	func_gcd(a, b, &gcd);

	printf("GCD : %d\n", gcd);

	return 0;
}
