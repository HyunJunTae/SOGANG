#include <stdio.h>

double CtoD (char c[], int len) {

	int i;
	double d[12];

	for (i=0;i<len;i++) {

		if ( 48 <= (int)c[i] && (int)c[i] <= 57 ) {
			d[i] = (int)c[i] - 48;
		}
		else {
			d[i] = c[i];
		}	
	}
	return d[]; 
}




int main(void) {

	int i = 0;
	int len = 0;
	char c[12] ={0};
	char a;
	double num;

	while (a != "\n") {
		scanf("%c", &a);
		c[i] = a;
        i++;
		len = len + 1;
	}

	num = CtoD(c, len);
	printf("Result : %10.6f", num);

	return 0;

}
