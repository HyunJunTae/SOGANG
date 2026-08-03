#include <stdio.h>


int* toArabia(char word[]) {
	



}



void toRome(int n) {



}




int main(void) {
	


	char word1[10];
	char word2[10];

	int i, num;
	for(i=0;i<10;i++) {
		scanf("%d", &num);
		if(num == '\n') {
			break;
		}
		word1[i] = num;
	}
	for(i=0;i<10;i++) {
                scanf("%d", &num);
                if(num == '\n') {
                        break;
                }
                word2[i] = num;
        }


	int* num1;
	int* num2;
	num1 = toArabia(word1);
	num2 = toArabia(word1);

	return 0;
}
