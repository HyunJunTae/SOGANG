// class inheritance 에서 protected member에 대한 내용.


#include <stdio.h>
#include <string.h>
#define MAXLEN 32

class UnivMem {
protected:
    int id;
    char name[MAXLEN+1];
public:

    UnivMem(){}; // 빈 생성자를 추가로 정의해야 정상적으로 작동함.

    UnivMem(int i, const char *n) {
        id = i;
        strcpy(name, n);
    }
    void Print() {
        printf("Id : %d\n", id);
        printf("Name : %s\n", name);
    }
};


class Student: public UnivMem {
private:
    double GPA;
public:
    Student(int i, const char *n, double g) {
        // 부모 클래스에서 member를 protected로 선언하면, 자식 클래스에서 바로 사용 가능.
        id = i;
        strcpy(name, n);
        GPA = g;
    }
};

class Professor: public UnivMem {
private:
    char office[MAXLEN+1];
public:
    Professor(int i, const char *n, const char *o) {
        id = i;
        strcpy(name, n);
        strcpy(office, o);
    }
};


int main(void) {
    Professor p(0, "Jaeseung Choi", "AS711");
    Student s(1, "GangSeo Kim", 4.3);

    UnivMem *a, *b;
    a = &p;
    b = &s;

    a->Print();
    b->Print();
    // printf("%d %s %s", p.id, p.name, p.office);

    return 0;
}