// #include <stdio.h>

// int Fibo(int a, int num) {

// 	int i;
// 	int b = 0;
// 	int c = 1;

// 	for (i=1; i<a; i++) {
// 		num = b + c;
// 		b = c;
// 		c = num;
// 	}
// 	return num;
// }

// int main (void) {

// 	int a, num;
// 	scanf("%d", &a);

// 	num = Fibo(a, num);
	
// 	printf("fibo(%d) : %d\n", a, num);

// }
// 0 1 1 2 3 5 8 13 21


#include <stdio.h>

int Fibo(int a) {

	int num;

	int i;
	int b = 0;
	int c = 1;

	for (i=1; i<a; i++) {
		num = b + c;
		b = c;
		c = num;
	}
	return num;
}

int main (void) {

	int a, num;
	scanf("%d", &a);

	num = Fibo(a);
	
	printf("fibo(%d) : %d\n", a, num);

}

// Fibo 함수에 num 가져갈 필요 없음. 어차피 return num 할거니까
