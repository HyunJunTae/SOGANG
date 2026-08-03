#include <stdio.h>
#include <stdlib.h>
#include<time.h>



int main(void) {

	int i, j, k, num, num1, num2, num1n, num2n;
	int check = 0;
	int b[4][4] = {0};
	
	b[0][3] = 2;
	b[1][2] = 4;

	// 배열출력
    for (i=0;i<4;i++) {
        for(j=0;j<4;j++) {
            printf("%d ", b[i][j]);
        }
        printf("\n");
    }

	// 칸이 차있으면 check = check + 1, 모든 칸이 차있으면 return 0
	for (i=0;i<4;i++){
		for (j=0;j<4;j++){
			if (b[i][j] != 0){
				check = check + 1;
			}
		}
	}

	if (check == 16) {
		return 0;
	}






	srand(time(NULL));

	// 랜덤숫자 하나 받아서 3으로 나눠서 나머지 0이면 4, 나머지 1,2면 2.
	num1 = rand();

	if (num1%3 == 0) {
                num = 4;
        }
        else {
                num = 2;
        }


	// 랜덤숫자 두 개 받은다음 4로 나눠서 나머지 얻기.그 나머지 두 개가 배열의 좌표. 그 좌표에 값이 0이면 위의 2 또는 4 넣고 
	// check2 = check2 + 1해서 반복문 끝내기.
	int check2 = 0;

	while (check2 == 0) {

		num1 = rand();
	    num2 = rand();

		num1n = num1 % 4;
		num2n = num2 % 4;

		if ( b[num1n][num2n] == 0 ) {
			b[num1n][num2n] = num;
			check2 = 1;
		}
	}


	// 배열출력
    for (i=0;i<4;i++) {
        for(j=0;j<4;j++) {
            printf("%d ", b[i][j]);
        }
        printf("\n");
    }
	printf("number is %d\n", num);
	return num;


}
