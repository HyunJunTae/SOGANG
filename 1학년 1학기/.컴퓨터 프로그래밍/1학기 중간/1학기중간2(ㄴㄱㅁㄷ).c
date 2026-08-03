#include <stdio.h>


void caesar(char* alphabet, int key) {

	*alphabet = *alphabet + key;

	if ((int)*alphabet > 122) {
		*alphabet = *alphabet - 26;
	}
	if ((int)*alphabet < 97) {
                *alphabet = *alphabet + 26;
	}

}



int main(void) {

	char a,b,c;
	int  d;

	scanf("%d %c%c%c", &d, &a,&b,&c);

	caesar(&a, d);
	caesar(&b, d);
	caesar(&c, d);

	printf("%c%c%c\n", a,b,c);

	return 0;


}
