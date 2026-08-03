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
	/* user code */ 
	
	int temp = b[0][0];
	b[0][0] = b[3][0];
	b[3][0] = b[3][4];
	b[3][4] = b[0][4];
	b[0][4] = temp;

	temp = b[0][1];
	b[0][1] = b[2][0];
	b[2][0] = b[3][2];
	b[3][2] = b[1][3];
	b[1][3] = temp;

	temp = b[0][2];
	b[0][2] = b[1][0];
	b[1][0] = b[3][1];
	b[3][1] = b[2][3];
	b[2][3] = temp;

	temp = b[1][1];
	b[1][1] = b[2][1];
	b[2][1] = b[2][2];
	b[2][2] = b[1][2];
	b[1][2] = temp;

	// for (int i=0; i<4; i++) {
	// 	for(int j=0; j<4; j++) {
			
	// 	}
	// }
}

int set_board(int dir, int b[4][4]){

	/* user code init */

	// 한 번이라도 움직일 수 있는 박스가 있다면 1, 없다면 0
	int setted = 0;

	// 왼쪽에서 오른쪽으로 숫자 합치기 전에, 판 rotate로 돌려주기 (ex 위로 미는 경우 rotate 1번 하고 민 뒤에 3번 rotate)

	switch (dir)
	{
	case 1: 
		rotate;
	case 2:
		rotate;
		rotate;
	case 3:
		rotate;
		rotate;
		rotate;	
	}

	// 왼쪽에서 오른쪽으로 숫자 합치는 메소드
	// 한 줄씩 4번
	for (int i=0; i<4; i++) {

		// 합치기
		// 한 줄에 4칸인데, 마지막 칸은 필요없으니까 3번만.
		// 오른쪽부터 왼쪽 순으로 검사함. 3->1
		for (int j=3; j>0; j--) {
			// 3, 2, 1 번 칸의 숫자를 이전 칸들과 비교.
			for (int k=j-1; k>=0; k--) {
				// 빈 칸이 0인 경우 다음 칸도 탐색.
				if (b[i][k] == 0) continue;
				// 빈 칸이 같은 수일 경우 합치고 왼쪽 칸은 0으로.
				else if (b[i][j] == b[i][k]) {
					b[i][j] *= 2;
					b[i][k] = 0;
					break;
				}
				// 빈 칸이 0이 아니고, 같은 수도 아닌 다른 수이면 거기서 멈추기.
				else break;
			}
		}

		// 옮기기
		int a1=b[i][0], a2 = b[i][1], a3 = b[i][2], a4 = b[i][3];
		// for (int j=2; j>=0; j--) {

		// 	// 만약 밀어야되는 숫자가 0이라면 스킵해도 됨
		// 	if (b[i][j] = 0) continue;

		// 	// 2, 1, 0 번 칸 숫자를 뒤로 1칸, 2칸, 3칸 검사해서 0이 있는 곳까지 밀기
		// 	for (int k=0; k<3-j; k++) {
		// 		// 만약 본 칸이 0이면, 다음 오른쪽 칸 탐색
		// 		if(b[i][3-k] == 0) continue;
		// 		// 만약 본 칸이 0이 아니면 해당 칸 이전까지 밀기.
		// 		else 
		// 	}
		// }

	}

	// 돌렸던 판 다시 되돌리기기
	switch (dir)
	{
	case 1:
		rotate;
		rotate;
		rotate;
	case 2:
		rotate;
		rotate;
	case 3:

		rotate;
	}

	return 1;
}

int is_game_over(){
	/* user code */

	/* if game over return 0, else then return 1 */
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
	/* user code */

	/* return 2 or 4 that makes in this times */

	/* if can not make two or four, then return 0 */
	return 0;	
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
