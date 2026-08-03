#include <stdio.h>

int isDivisor(int m, int n) {

	if ( n % m == 0 ) {
		return 1;
	// }
	// else {
	// 	return 0;
	}


	return 0;
	//어차피 나머지 0이면 위에서 return 1 되어서 끝나니까 그냥 바로 return 0 적어도 됨.
}

int isPrime(int m) {

	int j;

	if ( m == 2 ) {
		return 1;
	}

	else {
		for (j=2 ; j<m ; j++) {

			if ( m % j == 0 ){
				return 0;
			}
		}
	}

	return 1;
}





int main(void) {

	int i, num, a, b;

	printf("Input : ");
	scanf("%d", &num);

	for (i=2 ; i<=num ; i++) {

		a = isDivisor(i, num);
		b = isPrime(i);

		if (a == 1 && b == 1) {
			printf("%d     ", i);
		}
	}

	printf("\n");

}
