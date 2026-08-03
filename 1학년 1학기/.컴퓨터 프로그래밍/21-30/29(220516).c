#include <stdio.h>

float average(int nxn[], int i, int N) {

	int j;
	float add = 0;

	for (j=0;j<N;j++) {
		add = add + nxn[j];
	}
	add = add / N;

	return add;
}



int main(void) {

	int N, i, j;
	float avg;

	printf("Input N : ");
	scanf("%d", &N);

	int NXN[N][N];

	for (i=0;i<N;i++) {
		for (j=0;j<N;j++) {
			scanf("%d", &NXN[i][j]);
		}
	}
	// 여기서는 1 2 3 4 5 6 7 8 9 한 번에 입력하면 1 2 3 까지밖에 입력 안됨. 아마 i=0상태에서 j 세 번 하고, i=1인 새로운 j 반복문으로
	// 돌아가는거라 그런듯.

	

	for (i=0;i<N;i++) {
		avg = average(NXN[i], i, N);
		printf("%.2f ", avg);
	}

	printf("\n");

	return 0;


}
