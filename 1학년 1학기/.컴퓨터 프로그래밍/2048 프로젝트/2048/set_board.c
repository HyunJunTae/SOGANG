#include <stdio.h>

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



int main(void) {

	int i, j, k, num, num1, num2, num1n, num2n;
	int check = 0;
	int b[4][4] = {0};
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

	printf("dir (w : 0 / a : 1 / s : 2 / d : 3) : ");
    scanf("%d", &dir);

    set_board(dir, b);

    printf("---------------------------\n");
    
    for (i=0;i<4;i++) {
        for(j=0;j<4;j++) {
            printf("%d ", b[i][j]);
        }
        printf("\n");
    }

	printf("dir (w : 0 / a : 1 / s : 2 / d : 3) : ");
    scanf("%d", &dir);

    set_board(dir, b);

    printf("---------------------------\n");
    
    for (i=0;i<4;i++) {
        for(j=0;j<4;j++) {
            printf("%d ", b[i][j]);
        }
        printf("\n");
    }

    return 0;

}