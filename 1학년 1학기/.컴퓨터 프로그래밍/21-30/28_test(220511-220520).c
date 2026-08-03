#include <stdio.h>

// int main(void) {

//     int i;

//     for (i=0;i<5;i++) {
//         int a;
//         scanf("%d", &a);
//         printf("%d", a);
//     }
// // C언어에서는 변수가 어떤 형태인지 정해짐. 그거 하나 딱 입력받으면 끝. 
// // 그래서 하나 입력하고 스페이스바 누르고 또 입력하면 다음 입력 바로 가능.










// // c언어는 문자열 계산할 때 아스키코드로 변환해서 계산 가능.
//     // char a;
//     // scanf("%c", &a);
//     // printf("%d", (int)a);

//     return 0;
// }











// void alpha(char c[ ]) {

// 	int i;

// 	for (i=0;i<=49;i++) {
// 		if (97 <= c[i] && c[i] <= 122) {
// 			c[i] = c[i] - 32;
// 		}
// 	}
// }

// int main(void) {

// 	int i;
// 	char c[50] = {0};

// 	// 바뀐 부분
//     char a;
//     scanf("%c", &a);
// 	// 이 부분이 for 반복문 밖으로 나옴

//     for (int i=0; i<50; i++){
//         if (a =='\n'){
//             break;
//             }
//         c[i]=a;
//     }

// 	alpha(c);

// 	for (i=0;i<=49;i++) {
// 		if (c[i] != 0) {
// 			printf("%c", c[i]);
// 		}
// 	}

// 	return 0;
// }












// int main (void) {

// 	char c;
	
// 	scanf("%c", &c);
// 	printf("c = %c\n", c);

// 	return 0;
// }
//이거 Hello, World! 입력하면 H 한 글자만 출력됨.








// int main (void) {

// 	char c;
	
// 	scanf("%c", &c);
// 	printf("c = %c\n", c);

// 	int n;

// 	scanf("%d", &n);
// 	printf("n = %d\n", n);

// 	return 0;
// }
// 위의 예시에서 int형 입력 출력을 추가.
// 12345 입력하면 c = 1     n = 2345 출력됨.
// Hello 입력하면 c = H,    n = 0 출력됨.
// scanf에서 char형은 한 글자밖에 입력을 못받나보다. 한 글자 입력받아서 출력한 후, 아직 이미 입력받아놓은 것들이 남아있으니까 그걸로 n 할당.









// int main (void) {

// 	char c2;

// 	scanf("%c", &c2);
// 	printf("c = %c\n", c2);

// 	int n1;

// 	scanf("%d", &n1);
// 	printf("n = %d\n", n1);

// 	int n2;

// 	scanf("%d", &n2);
// 	printf("n = %d\n", n2);

// 	return 0;
// }
// 위의 예시에서 int형 입력 출력을 하나 더 추가
// 위처럼 12345 입력하면 c2 = 1  n1 = 2345 나오는건 똑같음.
// 근데 그 뒤에 입력하는 칸이 하나 더 나옴. 12345 입력하면 n2 = 12345 나옴.
// 문자형은 한 개밖에 입력 못한다는걸 이제서야 알았다. 그래서 c에 1 입력하고 남은 2345가 n1에 들어간거였어








int main(void) {

	char c1 = 'a';
	printf("%c\n", c1);

	char *c2 = "Hello, World!";
	printf("%c\n", c2);

	char *c3 = "Hello, World!";
	printf("%s\n", c3);

	return 0;
}
// https://dojang.io/mod/page/view.php?id=328
// char은 1바이트 크기민 문자 하나만 저장 가능. 문자열은 변수에 직접 저장하지 않고 "포인터" 라는 것을 이용한다.