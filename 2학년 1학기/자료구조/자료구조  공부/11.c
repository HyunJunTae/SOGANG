#include <stdio.h>


typedef struct humanBeing
{
    char name[10];
};

typedef struct
{
    char name[10];
}Dog;

int main(void) {

    struct humanBeing tom;
    Dog mary;

    return 0;
}