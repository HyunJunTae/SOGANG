#include <stdio.h>


int main(void) {

	int n[10];
	int temp, i;
	int max = 0;
	int min = 999999999;


	printf("Input (10 numbers) : ");

	for (i=0;i<=9;i++) {
		scanf("%d", &n[i]);
	}

	for (i=0;i<=4;i++) {
		temp = n[i];
		n[i] = n[9-i];
		n[9-i] = temp;
	}

	for (i=0;i<=9;i++) {
		printf("%d ", n[i]);
	}

	printf("\n");

	for (i=0;i<=9;i++) {
		if (n[i] > max) {
			max = n[i];
		}
		else if (n[i] < min) {
			min = n[i];
		}
	}

	printf("Max : %d, Min : %d\n", max, min);

	return 0;

}