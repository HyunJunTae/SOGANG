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
	int a[4][4];
	int i, j;

	for (i=0;i<4;i++) {
		for (j=0;j<4;j++) {
			a[i][j] = b[i][j];
		}
	}

	b[0] = a[12]
	b[1] = a[8]
	b[2] = a[4]
	b[3] = a[0]
	b[4] = a[13]
	b[5] = a[9]
	b[6] = a[5]
	b[7] = a[1]
	b[8] = a[14]
	b[9] = a[10]
	b[10] = a[6]
	b[11] = a[2]
	b[12] = a[15]
	b[13] = a[11]
	b[14] = a[7]
	b[15] = a[3]
	/* user code */
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

	for (i=0;i<=4;i++){
		for (j=0;j<=3;j++){
			for(k=0;k<=3;k++){
				if ( b[i][k] == 0 && b[i][k+1] != 0 ) {
					b[i][k] = b[i][k+1];
					b[i][k+1] = 0;
				}
			}
		}
	}

	for (i=0;i<4;i++) {
        	for (j=0;j<3;j++) {
            		if (b[i][j] == b[i][j+1]) {
                		b[i][j] = b[i][j] + b[i][j+1];
                		b[i][j+1] = 0;
            		}
        	}
    	}

	for (i=0;i<=4;i++){
                for (j=0;j<=3;j++){
                        for(k=0;k<=3;k++){
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

