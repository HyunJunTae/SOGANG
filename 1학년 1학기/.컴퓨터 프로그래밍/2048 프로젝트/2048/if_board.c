// if문의 조건파트에서 set_board 해도 셋보드가 되는가??


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



	return 1;
}



int main(void) {

	int i, j, k, num, num1, num2, num1n, num2n;
	int check = 0;
	int b[4][4] = {0};
    int dir;
    printf("dir (w : 0 / a : 1 / s : 2 / d : 3) : ");
    scanf("%d", &dir);
	
	b[0][3] = 2;
	b[1][2] = 4;

	// 배열출력

	printf("Before set_board\n");
    for (i=0;i<4;i++) {
        for(j=0;j<4;j++) {
            printf("%d ", b[i][j]);
        }
        printf("\n");
    }

	printf("------------------------------\n");
	printf("After set_board\n");

    if (set_board(dir, b) == 1) {
        for (i=0;i<4;i++) {
            for(j=0;j<4;j++) {
                printf("%d ", b[i][j]);
            }
        printf("\n");
        }
    }

    return 0;

}

// if 조건절 안에서 setboard 해도 셋보드가 이루어진다.