#include <cstdio>
#include <memory>

int main(void) {
    double *doubleVariablePt = new double (3.0);
    double *doubleArrayPt = new double[3] {1,2,3};
    char *charVariablePt = new char ('a');
    int *intVariablePt = new int (324);

    printf ( "The size of double pointer is %d.\n", sizeof(double*));
    printf ( "The size of double pointer is %d.\n", sizeof(doubleVariablePt));
    printf ( "The size of char pointer is %d.\n", sizeof(charVariablePt));
    printf ( "The size of int pointer is %d.\n", sizeof(intVariablePt));

    delete doubleVariablePt, charVariablePt, intVariablePt;
    return 0;
}