#include <stdio.h>

void surface_volume(int w, int d, int h, int *surface, int *volume);

int main(void){
    int w, d, h, surface, volume;
    scanf("%d %d %d", &w, &d, &h);
    surface_volume(w, d, h, &surface, &volume);
    printf("%dcm^2 %dcm^3\n", surface, volume);
    return 0;
}

void surface_volume(int w, int d, int h, int *surface, int *volume){
    *surface = (w*d + w*h + d*h)* 2;
    *volume = (w*d*h);
}