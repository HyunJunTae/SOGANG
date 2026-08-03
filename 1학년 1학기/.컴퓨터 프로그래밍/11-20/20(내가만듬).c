#include <stdio.h>



int main(void) {

	int num, cut;
	int count = 0;
	int score = 0;
	int failures = 0;
	float scores = 0;

	printf("Number of class : ");
	scanf("%d", &num);
	printf("Cutline : ");
	scanf("%d", &cut);

	while ( count < num ) {
		printf("Input score #%d : ", count);
		scanf("%d", &score);
		scores += score;

		if (score < cut) {
			failures += 1;
		}

		count++;
	}

	printf("Average score : %.2f\n", scores / num);
	printf("Number of failures : %d\n", failures);

	return 0;
}
