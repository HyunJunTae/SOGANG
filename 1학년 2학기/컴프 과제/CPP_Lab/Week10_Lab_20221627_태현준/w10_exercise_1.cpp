#include <cstdio>
#include <cstring>
#include <memory>


void multiply (int *x, int *y){
    *x = *x * *y;
}


void abs (int *x){

    if (*x<0)
        *x = *x * (-1);


}

void factorial (int *x){
    
    int a = 1;

    for (int i=2; i<=*x; i++) {
        a = a * i;
    }

    *x = a;;
}

int main(){

    int *x, *y;

    // your code here
    // please assign memory space to pointer x and y.
    x = (int*)malloc(1 * sizeof(int)); 
    y = (int*)malloc(1 * sizeof(int)); 

    *x = -5;
    *y = 5;

    abs(x);
    printf("*x is %d.\n", *x);
    multiply(x,y);
    printf("*x is %d.\n", *x);
    factorial (y);
    printf("*y is %d.\n", *y);

    // your code here
    // flush the allocated memory space for x and y.
    free(x);
    free(y);
    

    return 0;

}