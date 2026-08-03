#include <cstdio>
#include <string.h>
#define MAXLEN 32

class Student {
private:
    int year = 1;
    int id;
    char name[MAXLEN+1];
public:
    Student(int _id, const char* _name) : id(_id) { // 이렇게 한 번에 가능.
        // id = _id;
        strncpy(name, _name, MAXLEN);
        name[MAXLEN] = '\0';
    }

    void Print() {
        printf("Id : %d\n", id);
        printf("Name : %s\n", name);
    }
};

int main(void) {
    Student x(0, "John");
    Student y(1, "Mary");

    x.Print();
    y.Print();

    return 0;
}