#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <termios.h>
#include <sys/time.h>
#include <unistd.h>

/* game over flag & board state */
int game_over, b[4][4];
FILE *fp;

void init(){
	int i, j;
	game_over = 0;
	for(i=0; i<4; i++)
		for(j=0; j<4; j++)
			b[i][j] = 0;
}


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

void draw_board(int tot, int command){
	int i, j, k, c[8][8], score;
	/* console clear */
	system("clear");

	score = 0;
	/* user code */
	
	/* calculate score & check sum of all block equasls variable tot */



	printf("    Score : %d\n", score);
	fprintf(fp, "%d %d\n", score, command);

	for(i=0; i<4; i++){
		for(j=0; j<4; j++){
			c[i][j] = 32;
			for(k=0; k<50; k++){
				if( b[i][j] == (1<<k) ){
					c[i][j] = 32 + (k%6);
					break;
				}
			}

		}
	}
	for(i=0; i<4; i++){
		for(j=0; j<4; j++)
			fprintf(fp, "%d ", b[i][j]);
		fprintf(fp, "\n");
	}

	for(i=0; i<4; i++){
		puts("");
		puts("");
		for(j=0; j<4; j++)
			printf("\033[%dm%5d\033[0m", c[i][j], b[i][j]);
		puts("");
		puts("");
	}
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


int getch(void)
{
	char   ch;
	int   error;
	static struct termios Otty, Ntty;

	fflush(stdout);
	tcgetattr(0, &Otty);
	Ntty = Otty;
	Ntty.c_iflag = 0;
	Ntty.c_oflag = 0;
	Ntty.c_lflag &= ~ICANON;
#if 1
	Ntty.c_lflag &= ~ECHO;
#else
	Ntty.c_lflag |= ECHO;
#endif
	Ntty.c_cc[VMIN] = 0;
	Ntty.c_cc[VTIME] = 1;

#if 1
#define FLAG TCSAFLUSH
#else
#define FLAG TCSANOW
#endif

	if (0 == (error = tcsetattr(0, FLAG, &Ntty)))
	{
		error = read(0, &ch, 1);
		error += tcsetattr(0, FLAG, &Otty);
	}

	return (error == 1 ? (int)ch : -1);
}


int GetCommand() {
	int ch = getch();

	switch (ch)
	{
	case 'd':
	case 'D': return 0;
	case 'w':
	case 'W': return 1;
	case 'a':
	case 'A': return 2;
	case 's':
	case 'S': return 3;
	default: return -1;
	}

	return -1;
}


int main() {
	int command = -1;
	fp = fopen("output.txt", "w");
	init();
	srand(time(NULL));

	int tot = 0;

	/* make init board */
	/* user code */

	draw_board(tot, -1);

	
	do {
		command = GetCommand();

		if (command != -1) {
			if( set_board(command, b) ){

				/* user code */

				draw_board(tot, command);
			}
		}

	} while (!game_over);

	system("clear");
	printf("game over!\n");
	fclose( fp );
}