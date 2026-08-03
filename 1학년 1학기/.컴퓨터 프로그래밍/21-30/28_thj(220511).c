#include <stdio.h>

void alpha(char c[]) {
	
	int i;

	for (i=0;i<=49;i++) {
		if (97 <= c[i] && c[i] <= 122) {
			c[i] = c[i] - 32;
		}
	}
}

int main(void) {

	int i;
	char c[50] = {0};

    for (int i=0; i<50; i++){
        char a;
        scanf("%c", &a);
        if (a=='\n'){
            break;
        }
        c[i]=a;
    }
	// char형 변수 a에는 한 글자씩밖에 저장이 안되니까 scanf로 쭉 입력해놓고 한 글자씩 담아서 배열 c에 옮긴다. 50자리까지.

	alpha(c);

	for (i=0;i<=49;i++) {
		if (c[i] != 0) {
			printf("%c", c[i]);
		}
	}

	return 0;
}
