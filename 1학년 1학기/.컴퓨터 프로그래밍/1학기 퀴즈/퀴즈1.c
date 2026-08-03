#include <stdio.h>

void innerproduct(int* x1, int* y1, int* z1, int* x2, int* y2, int* z2, int* answer);

int main(void) {
	int x1, x2, y1, y2, z1, z2, answer;

	printf("Input : ");
	scanf("%d %d %d %d %d %d", &x1, &y1, &z1, &x2, &y2, &z2);

	innerproduct(&x1, &y1, &z1, &x2, &y2, &z2, &answer);

	printf("Output : %d\n", answer);

	return 0;
}




void innerproduct(int* x1, int* y1, int* z1, int* x2, int* y2, int* z2, int* answer) {

	*answer = *x1**x2 + *y1**y2 + *z1**z2;

}

