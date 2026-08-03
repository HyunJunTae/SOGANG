#include <stdio.h>


void solution(int* matrix, int row,int col, int x1,int y1,int x2,int y2,int n1, int n2) {

	int num;
	
	num = *(matrix + row*(n1-1) + col*(y1-1) + x1);

	x1 = x1 + 1;
	if ( x1 == x2) {
		x1 = n1;
		y1 = y1 + 1;
	}
	if ( y1 > y2) {
		return;
	}

	num =  num;
	printf("%d", num);
}



int main(void) {

	int row, col;
	scanf("%d %d", &row, &col);


	int matrix[row][col];
	int i, j;
	for(i=0;i<row;i++) {
		for(j=0;j<col;j++) {
			scanf("%d", &matrix[i][j]);
		}
	}

	
	int x1, y1, x2, y2;
	scanf("%d %d %d %d", &x1, &y1, &x2, &y2);
	int n1 = x1;
	int n2 = y2;
	solution(*matrix, row, col, x1,y1,x2,y2,n1, n2);
	
	return 0;
}
