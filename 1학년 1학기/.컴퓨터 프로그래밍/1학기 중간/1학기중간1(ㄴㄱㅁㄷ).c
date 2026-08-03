#include <stdio.h>



void checkWB(char col, int row, char* answer) {

	if ( (int)col % 2 == 0 ) {
		if ( row % 2 == 0 ) {
			*answer = col;
		}
		else {
			*answer = row;
		}
	}
	else {
		if (row % 2 == 0) {
			*answer = row;
		}
		else {
			*answer = col;
		}
	}

}


int main (void) {
	char a, answer;
	int b;

	scanf("%c%d", &a,&b);

	checkWB(a, b, &answer);

	if ( answer == a ) {
		printf("B\n");
	}
	else {
		printf("W\n");
	}

	return 0;




}
