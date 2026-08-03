#include <cstdio>
#include <string.h>
#define MAXLEN 32

class Student {
private:
    int year = 1;
    int id;
    char name[MAXLEN+1];
public:
    // _name을 this->name으로 바꿈.
    Student(int _id, const char* name) : id(_id) {
        printf("this = %p\n", this); /////////////////////// 이거랑
        strncpy(this->name, name, MAXLEN);
        this->name[MAXLEN] = '\0';
    }

    // 얘도 하나만 가지고 만들기 가능. Student 여러 개 만드는 거 가능.
    Student(int _id) : id(_id) {
        strcpy(this->name, "noname");
    }

    void Print() {
        printf("Id : %d\n", id);
        printf("Name : %s\n", name);
    }
};

int main(void) {
    Student x(0, "John");
    Student y(1);
    
    printf("&x %p\n", &x); /////////////////////// 이거는 같은 주소값. x와 this는 같은주소값?

    return 0;
}