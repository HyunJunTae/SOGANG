#include <stdio.h>




int main(void) {

	int a;
	int b = 1;
	int c = 1;
	int count = 1;

	printf("Input : ");
	scanf("%d", &a);


	while (count <= a) {
		while (count <= a) {
			printf(" ");
			count += 1;
		}
		count = 1;
		
		while ( b > 0 ) {
			printf("*");
			b -= 1;
		}
		c += 2;
		b = c;
		// 왜 이거 그냥 c 없이 b = b + 2 하면 안되지 머지
		// 아 위에 while문제서 b를 계속 빼니까. 별 개수를 저장해둘 c가 필요함.
		a -= 1;

		printf("\n");
	}

}
