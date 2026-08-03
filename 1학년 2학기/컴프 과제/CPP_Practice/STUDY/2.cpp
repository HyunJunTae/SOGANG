#include <cstdio>
#include <memory>

class A{
public:
    A(){
    val = 5;
    }
    A(int temp):val(temp){}
    A(char temp):a(temp){}
    int val, temp;
    char a;
    float b;
    double c;
};

int main(){
    A *objPt1 = new A;
    auto *objPt2 = new A(3);
    auto *objArrayPt = new A [4] {'a'};
    printf ("The size of class A is %u.\n", sizeof(A));
    printf ("The size of class objPt1 is %u.\n", sizeof(objPt1));
    printf ("The size of class objPt2 is %u.\n", sizeof(objPt2));
    printf ("The size of class onjArrayPt is %u.\n", sizeof(objArrayPt));
    printf("%c\n", objArrayPt->a);
    printf("objPt1->val: %d\n", objPt1->val );
    printf("objPt2->val: %d\n", objPt2->val );
    delete objPt1, objPt2;
    delete[] objArrayPt;
    return 0;
}