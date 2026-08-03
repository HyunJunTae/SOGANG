#include <stdio.h>

void dec2oct(int* a, int* b, int* c, int* d, int e) {

	*a = e / 8 / 8 / 8 % 8;
	*b = e / 8 / 8 % 8;
	*c = e / 8 % 8;
	*d = e % 8;
}

char digit2hex(int a) {
	char ascii;

	if (a > 10) {
		ascii = a + 55;
	}
	else {
		ascii = a + 48;
	}
	return ascii;
}

void dec2hex(char* a, char* b, char* c, int d) {

	*a = digit2hex(d / 16 / 16 % 16);
	*b = digit2hex(d / 16 % 16);
	*c = digit2hex(d % 16);
}


int main (void) {

	int num;
	int a, b, c, d;
	char e, f, g;

	printf("Input : ");
	scanf("%d", &num);

	dec2oct(&a, &b, &c, &d, num);
	dec2hex(&e, &f, &g, num);


	printf("%.4o\n", num);
	printf("%d%d%d%d\n", a, b, c, d);
	printf("%.3X\n", num);
	printf("%c%c%c\n", e, f, g);

}
