#include <cstdio>
#include <string.h>
#define MAXLEN 32

class Student {
private:
    int id;
    char name[MAXLEN+1];
public:
    // class 이름과 같은 이름으로 선언. 전의 init 등 필요없고 바로 입력 가능.
    Student(int _id, const char* _name) {
        id = _id;
        strncpy(name, _name, MAXLEN);
        name[MAXLEN] = '\0';
    }

    void Print() {
        printf("Id : %d\n", id);
        printf("Name : %s\n", name);
    }
};

int main(void) {
    // 이런 식으로 입력.
    Student x(0, "John");
    Student y(1, "Mary");

    x.Print();
    y.Print();

    return 0;
}