#include <stdio.h>

int main(){
	int inputPrice;
	scanf("%d", &inputPrice);

	//3개월 이자 이자 복리로.
	int sixmonth = inputPrice * (1 + 0.05) * (1 + 0.05) * (1 + 0.05);

	printf("%-10s\t:\t%10d\t%10d\n", "Month", 3, 6);
	printf("%-10s\t:\t%10d\t%10d\n", "Payment", inputPrice, sixmonth);
	printf("%-10s\t:\t%10.2f\t%10.2f\n", "Pay/month", (float)inputPrice / 3, (float)sixmonth / 6);
	return 0;
}