#include <stdio.h>

int main(void) {
	int x1, x2, y1, y2, square1, square2, add;

	printf("First : ");
	scanf("%d %d %d %d", &x1, &y1, &x2, &y2);

	square1 = (x2-x1) * (y2-y1);

	printf("Second : ");
        scanf("%d %d %d %d", &x1, &y1, &x2, &y2);

        square2 = (x2-x1) * (y2-y1);

	printf("answer : %d\n", square1 + square2);

	return 0;
}

