#include <stdio.h>


void surface_volume(int w, int d, int h, int* surface, int* volume) {

	*surface = 2*w*d + 2*d*h + 2*w*h;
	*volume = w*d*h;


}



int main(void) {

	int w, d, h, surface, volume;

	scanf("%d %d %d", &w, &d, &h);

	surface_volume(w, d, h, &surface, &volume);

	printf("%dcm^2 %dcm^3\n", surface, volume);

	return 0;

}
