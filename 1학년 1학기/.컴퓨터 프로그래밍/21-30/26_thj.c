// #include <stdio.h>

// int num_c = 0;
// int b = 1;

// int palindrome (int num, int digit) {

// 	if ( (num / digit) != 0 ) {
// 		int n;

// 		n = (num / digit) % 10;
// 		n = n * b;
// 		b = b / 10;
// 		num_c = num_c + n;

// 		digit = digit * 10;
		
// 		palindrome(num, digit);
// 	}

// 	else if ( num_c == num) {
// 		return 1;
// 	}
// 	else if ( num_c != num) {
// 		return 0;
// 	}
// }

// int main (void) {

// 	int num, a;
// 	int digit = 1;

// 	printf("Input : ");
// 	scanf("%d", &num);

// 	while ( num > b) {
// 		b = b * 10;
// 	}
// 	b = b / 10;

// 	a = palindrome(num, digit);

// 	if (a==1) {
// 		printf("%d is palindrome number.\n", num);
// 	}
// 	else {
// 		printf("%d is not a palindrome number.\n", num);
// 	}

// 	return 0;
// }




//12321 / 10000 		1








// 220608 추가
#include <stdio.h>

int palindrome (int num, int digit) {

	int num_2 = num;
	int num_1 = 0;
	int result = 0;
	int count = 1;

	while( digit >= 1) {

		num_1 = num / digit; 		// 12321 / 10000 = 1 앞자리 빼오기

		num = num - num_1*digit; // 12321 - 10000 = 2321  원래 숫자에서 빼주기
		result = result + num_1*count; // 0 + 1 = 1	    앞자리에서 빼온 값 뒷자리로.

		digit = digit / 10;
		count = count * 10;

		printf("%d\n", result);
	}

	if (num_2 == result) {
		return 1;
	}

	return 0;
}

int main(void) {

	int i, num, result;
	int digit = 1;

	// 회문수인지 확인할 숫자 입력
	printf("Input : ");
	scanf("%d", &num);

	// 입력 숫자 음수이면 오류라고 출력하기.
	if (num < 0) {
		printf("Input number canno be negative");
	}


	// 회문수 몇자리인지 확인할 변수 생성. 12321이면 5자리. 5자리로 가정하면 digit = 10000.
	while(digit < num) {
		digit = digit * 10;
	}
	digit = digit / 10;

	// palindrome 함수로 회문수인지 검사. 회문수이면 return 1.
	result = palindrome(num, digit);

	// result = 1이면 회문수. 0이면 회문수 아님.
	if (result == 1) {
		printf("%d is a palindrome number", num);
	}
	else {
		printf("%d is not a palindrome number", num);
	}

	return 0;

}