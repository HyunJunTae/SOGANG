#include <cstdio>
#include <string.h>
#define MAXLEN 32

// private과 public.
class Student {
private:
    int id;
    char name[MAXLEN+1];
public:
    void init(int _id, const char* _name) { 
        id = _id;
        strcpy(name, _name);
    }

    void Print() {
        printf("Id : %d\n", id);
        printf("Name : %s\n", name);
    }
};

// main에서 . 이용해서 id, name 입력하지 않고
// class 자체에서 id, name을 입력하는 init 함수 작성.
// 만약 strcpy를 strncpy로 바꿀 경우, 밑의 주석처럼 하는 경우 하나하나 다 바꿔야 하지만,
// init으로 하는 경우 init에서 하나만 바꾸면 됨. 더 간결해짐.
int main(void) {
    Student x, y;
    Student *ps;

    // x.id = 0;
    // strcpy(x.name, "John");
    // y.id = 1;
    // strcpy(y.name, "Mary");
    x.init(0, "John");
    y.init(1, "Mary");

    x.Print();
    ps = &y;
    ps -> Print();

    return 0;
}