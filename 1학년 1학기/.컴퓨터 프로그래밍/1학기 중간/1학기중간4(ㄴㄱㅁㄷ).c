#include <stdio.h>
#include <math.h>

float gun1(int a, int b, int c) {

	if (b*b - 4*a*c >= 0) {
		return (-b + sqrt(b*b-4*a*c)) / (2.0 * a);
	}
	else {
		return 0;
	}
}


float gun2(int a, int b, int c) {

	if (b*b - 4*a*c >= 0) {
		return (-b - sqrt(b*b-4*a*c)) / (2.0 * a);
	}
	else {
		return 0;
	}
}


int main(void) {

	int a, b, c, d, e, f;
	float n1, n2, n3, n4, N1, N2 = 0;

	scanf("%d %d %d %d %d %d", &a, &b, &c, &d, &e, &f);

	n1 = gun1(a, b, c);
	n2 = gun2(a, b, c);

	if (n1 != 0 || n2 != 0) {
		if (n1 >= n2) {
			N1 = n1;
		}
		else {
			N1 = n2;
		}
	}


	n3 = gun1(d, e, f);
	n4 = gun2(d, e, f);

	if (n3 != 0 || n3 != 0) {
			if (n3 >= n4) {
					N2 = n3;
			}
			else {
					N2 = n4;
			}
	}

	if (N1 != 0 && N2 != 0) {
		printf("%f\n", N1+N2);
	}
	else {
		printf("-1");
	}

}
