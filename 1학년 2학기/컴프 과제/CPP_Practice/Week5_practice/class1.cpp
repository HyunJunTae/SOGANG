#include <cstdio>
#include <string.h>
#define MAXLEN 32


// class는 data + code.
class Student {
public:
    int id;
    char name[MAXLEN+1];
    void Print() {
        printf("Id : %d\n", id);
        printf("Name : %s\n", name);
    }
};

int main(void) {
    Student x, y;
    Student *ps;

    // struct처럼 . 사용.
    x.id = 0;
    strcpy(x.name, "John");
    y.id = 1;
    strcpy(y.name, "Mary");

    // 클래스로 선언한 개체(x, y 등)에 다른 포인터 변수를 연결할 수도 있음.
    x.Print();
    ps = &y;
    ps -> Print();

    return 0;
}