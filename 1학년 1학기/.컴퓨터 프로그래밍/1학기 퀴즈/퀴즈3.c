#include <stdio.h>

float toFahrenheit(float celcius);

int main(void) {
	float celcius, answer;

	printf("celcius : ");
	scanf("%f", &celcius);

	answer = toFahrenheit(celcius);

	printf("fahrenheit : %.1f\n", answer);

	return 0;

}

float toFahrenheit(float celcius) {
	float answer;

	answer = celcius * 9 / 5 + 32;

	return answer;
}



