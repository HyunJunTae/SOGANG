#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int b[4][4] = {0};
int game_over = 0;

void rotate(int b[4][4]){
	int a[4][4];
	int i, j;

	for (i=0;i<4;i++) {
		for (j=0;j<4;j++) {
			a[i][j] = b[i][j];
		}
	}

	b[0][0] = a[3][0];
	b[0][1] = a[2][0];
	b[0][2] = a[1][0];
	b[0][3] = a[0][0];
	b[1][0] = a[3][1];
	b[1][1] = a[2][1];
	b[1][2] = a[1][1];
	b[1][3] = a[0][1];
	b[2][0] = a[3][2];
	b[2][1] = a[2][2];
	b[2][2] = a[1][2];
	b[2][3] = a[0][2];
	b[3][0] = a[3][3];
	b[3][1] = a[2][3];
	b[3][2] = a[1][3];
	b[3][3] = a[0][3];

}

int set_board(int dir, int b[4][4]){

	int i, j, k;

    // b와 비교하기 위한 a 만들기.
    int a[4][4];

	for (i=0;i<4;i++) {
		for (j=0;j<4;j++) {
			a[i][j] = b[i][j];
		}
	}

    // 미는게 왼쪽으로만 밀도록 정해져있으니까 돌려서 방향 맞추기.
	if (dir == 2) {
		rotate(b);
	}
	else if (dir == 3) {
		rotate(b);
		rotate(b);
	}
	else if (dir == 0) {
		rotate(b);
		rotate(b);
		rotate(b);
	}

    // 배열 왼쪽으로 밀기
	for (i=0;i<4;i++){
		for (j=0;j<3;j++){
			for(k=0;k<3;k++){
				if ( b[i][k] == 0 && b[i][k+1] != 0 ) {
					b[i][k] = b[i][k+1];
					b[i][k+1] = 0;
				}
			}
		}
	}

    // 배열 왼쪽 오른쪽 같으면 합치고 왼쪽으로 넘겨주기
	for (i=0;i<4;i++) {
        	for (j=0;j<3;j++) {
            		if (b[i][j] == b[i][j+1]) {
                		b[i][j] = b[i][j] + b[i][j+1];
                		b[i][j+1] = 0;
            		}
        	}
    	}

    // 배열 왼쪽으로 밀기
	for (i=0;i<4;i++){
                for (j=0;j<3;j++){
                        for(k=0;k<3;k++){
                                if ( b[i][k] == 0 && b[i][k+1] != 0 ) {
                                        b[i][k] = b[i][k+1];
                                        b[i][k+1] = 0;
                                }
                        }
                }
        }

    // 다시 돌려서 원래 모습으로.
	if (dir == 2) {
		rotate(b);
		rotate(b);
		rotate(b);

        }
	else if (dir == 3) {
		rotate(b);
		rotate(b);
        }
	else if (dir == 0) {
		rotate(b);
        }

    // 여기서 b는 밀고 돌리고 다시 밀어서 이동 다 끝난 상태
    // 이동 다 했는데 만약 이동 전인 a와 이동 후인 b가 같다면 잘못된 이동.
    // a와 b가 같냐 다르냐는 한 숫자 같을떄마다 count = count + 1 하고 count가 16이면 모든 자리 각각 다 같은것.
    int count = 0;
    for (i=0;i<4;i++) {
        for (j=0;j<4;j++) {
            if (a[i][j] == b[i][j])
            count = count + 1;
        }
    }
    if (count == 16) {
        return 0;
    }
    

	return 1;
}

int is_game_over(){

	int i, j;

	int a[4][4];
	for (i=0;i<4;i++) {
		for (j=0;j<4;j++) {
			a[i][j] = b[i][j];
		}
	}

	int c[4][4];
	for (i=0;i<4;i++) {
		for (j=0;j<4;j++) {
			c[i][j] = b[i][j];
		}
	}

	int d[4][4];
	for (i=0;i<4;i++) {
		for (j=0;j<4;j++) {
			d[i][j] = b[i][j];
		}
	}

	int e[4][4];
	for (i=0;i<4;i++) {
		for (j=0;j<4;j++) {
			e[i][j] = b[i][j];
		}
	}


	if (set_board(0,a) == 0) {
		if (set_board(1,c) == 0) {
			if (set_board(2,d) == 0) {
				if (set_board(3,e) == 0) {
					return 1;
				}
			}
		}
	}
	return 0;
}

int make_two_or_four(){

	int i, j, num, num1, num2, num1n, num2n;
	int check = 0;

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

	num1 = rand();


	if (num1%3 == 0) {
                num = 4;
        }
        else {
                num = 2;
        }

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
	return num;
}







int main(void) {

	int i, j;
    int dir;
	
	b[0][0] = 2;
    b[0][1] = 2;
    b[0][2] = 2;
    b[0][3] = 2;
    b[1][0] = 8;
    b[1][1] = 4;
	b[1][2] = 4;
    b[2][0] = 8;


	// 배열출력
    for (i=0;i<4;i++) {
        for(j=0;j<4;j++) {
            printf("%d ", b[i][j]);
        }
        printf("\n");
    }


	do {

		// 방향키 지정 받기
		printf("dir (w : 0 / a : 1 / s : 2 / d : 3) : ");
    	scanf("%d", &dir);

		// game over인지 아닌지 game_over 변수에 return 받은 값 저장하기
		game_over = is_game_over();

		if (set_board(dir, b) == 1) { //setboard 하면서 보드 옮겨주고, 만약 옮겨졌으면 1, 보드 그대로면 0 return.

			make_two_or_four(); // 만약 위에서 보드가 옮겨졌으면 return 1이라 얘도 실행. 안옮겨지면 얘도 실행 안함.
			}
		
		// 보드 그리기
		for (i=0;i<4;i++) {
			for(j=0;j<4;j++) {
				printf("%d ", b[i][j]);
			}
			printf("\n");
		}
	}while(game_over != 1); // game_over 값이 0인 이상 계속 진행. 만약 모든 칸 다차서 game_over = 1이면 그만.

    return 0;

}