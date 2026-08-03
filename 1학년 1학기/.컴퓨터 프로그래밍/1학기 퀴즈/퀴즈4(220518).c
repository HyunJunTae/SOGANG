#include <stdio.h>

int number(int n, int a) {

        int count = 0;
        int i = 1;

        while (a*i < n) {
                i = i + 1;
                count = count + 1;
        }

        return count;
}

int get(int n, int p) {
	int count = 0;
	int a = p;


	while ( n > a ) {

		count = count + number(n, a);
		a = a * p;
	}

	return count;
}


int main(void) {

	int n, p;

	scanf("%d %d", &n, &p);

	printf("%d\n", get(n, p));

	return 0;

}
