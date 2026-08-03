#include <cstdio>
#include <memory>
#include "intArrayManager.cpp"


int main(){

    int tmp1[3] = {1,2,3};
    int tmp2[3] = {4,5,6};

    IntArrayManager *A = new IntArrayManager ( 3, tmp1 );
    IntArrayManager *B = new IntArrayManager ( 3, tmp2 );

    A->print();
    B->print();

    if (A->compare ( *B ) )
        printf("array A comes earlier than array B.\n");

    A->concatenate ( *B );
    A->print();

    delete A;
    delete B;

    return 0;
    
}