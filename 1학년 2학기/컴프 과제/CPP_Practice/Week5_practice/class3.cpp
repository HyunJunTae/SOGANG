#include <cstdio>
#include <string.h>
#define MAXLEN 32

class Student {
private:
    int id;
    char name[MAXLEN+1];
public:
    void init(int _id, const char* _name) { //////////////// const char*가 아니라 그냥 char*로 하면 오류남. why??
        id = _id;
        strcpy(name, _name);
    }

    //name만 바꾸는 함수도 선언 가능.
    void SetName(const char* _name) {
        strcpy(name, _name);
    }

    void Print() {
        printf("Id : %d\n", id);
        printf("Name : %s\n", name);
    }
};

int main(void) {
    Student x, y;
    Student *ps;

    // x.id = 0;
    // strcpy(x.name, "John");
    // y.id = 1;
    // strcpy(y.name, "Mary");
    x.init(0, "John");
    y.init(1, "Mary");
    x.SetName("Jason");

    x.Print();
    ps = &y;
    ps -> Print();

    return 0;
}