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
	return d[]
}




int main(void) {

	int i;
	int len = 0;
	char c[12] ={0};
	char a;
	int count = 10;
	double num;

	while (count != 0 ) {
		scanf("%c", &a);
		count = count -1;

		if ( a != "-" && a != ".") {
			count = count + 1;
		}
		c[i] = a;
		len = len + 1;
	}

	num = CtoD(c, len);
	printf("Result : %10.6f", num);

	return 0;

}
