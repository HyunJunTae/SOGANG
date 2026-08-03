// #include <stdio.h>


// int main(void) {

// 	int n[10];
// 	int temp, i;
// 	int max = 0;
// 	int min = 999999999;


// 	printf("Input (10 numbers) : ");
// 	scanf("%d %d %d %d %d %d %d %d %d %d",&n[0],&n[1],&n[2],&n[3],&n[4],&n[5],&n[6],&n[7],&n[8],&n[9]);

// 	for (i=0;i<=4;i++) {
// 		temp = n[i];
// 		n[i] = n[9-i];
// 		n[9-i] = temp;
// 	}

// 	printf("%d %d %d %d %d %d %d %d %d %d\n",n[0],n[1],n[2],n[3],n[4],n[5],n[6],n[7],n[8],n[9]);

// 	for (i=0;i<=9;i++) {
// 		if (n[i] > max) {
// 			max = n[i];
// 		}
// 		else if (n[i] < min) {
// 			min = n[i];
// 		}
// 	}

// 	printf("Max : %d, Min : %d\n", max, min);

// 	return 0;

// }



#include <stdio.h>

int main(void) {

	int n[10];
	int i, num, temp;

	// 배열에 숫자 10개 입력
	printf("Input integers : ");
	for (i=0;i<10;i++) {
		scanf("%d", &num);
		n[i] = num;
	}


	// 배열 역순으로 바꿔주기
	for(i=0;i<5;i++) {
		temp = n[i];
		n[i] = n[9-i];
		n[9-i] = temp;
	}


	int max = n[0];
	int min = n[0]; // 와 이렇게하면 되네

	// 최댓값 최솟값 찾기
	for (i=0;i<10;i++) {
		if (max <= n[i]) {
			max = n[i];
		}
	}
	for (i=0;i<10;i++) {
		if (min >= n[i]) {
			min = n[i];
		}
	}

	// 출력
	for(i=0;i<10;i++) {
		printf("%d ", n[i]);
	}
	printf("\nMax : %d,  Min : %d\n", max, min);

	return 0;

}