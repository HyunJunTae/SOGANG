#include <cstdio>
#include <memory>

class A{
public:
    A(){
        val = 5;
    }

    A(int temp):val(temp){}
    int val, temp;
}

int main(void) {

    A *objPt1 = new A;

}