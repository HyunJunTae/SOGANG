#include <stdio.h>



int *calc(char op, int* no1, int* no2) {

	if (op == "+") {
		*no1 = *no1 + *no2;
	}
	else if (op == "-") {
		*no1 = *no1 - *no2;
	}
	else if (op == "*") {
                *no1 = *no1 * *no2;
        }
	else if (op == "/") {
                *no1 = *no1 / *no2;
        }

	return *no1;

}


int main(void) {

	int no1 = 100;

	char op;
	printf("input operation : ");
	scanf("%c", &op);

	int no2;
	printf("input Integer : ");
	scanf("%d", &no2);

	int result;
	result = calc(op, &no1, &no2);

	printf("result : %d", result);

	return 0;
}
