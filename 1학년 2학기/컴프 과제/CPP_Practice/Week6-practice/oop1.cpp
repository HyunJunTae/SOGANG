// 기본적인 class의 inheritance에 대한 내용.


#include <stdio.h>
#include <string.h>
#define MAXLEN 32

class UnivMem {
private:
    int id;
    char name[MAXLEN+1];
public:
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
    Student(int i, const char *n, double g) 
    :UnivMem(i, n)  {
        // id = i 같은 식으로 바로 접근 불가능. id는 UnivMem 의 private에 있으니까.
        GPA = g;
    }
};

class Professor: public UnivMem {
private:
    char office[MAXLEN+1];
public:
    Professor(int i, const char *n, const char *o)
    :UnivMem(i, n) {
        strcpy(office, o);
    }
};



int main(void) {
    Professor p(0, "Jaeseung Choi", "AS711");
    Student s(1, "GangSeo Kim", 4.3);

    // printf("%d %s %s", p.id, p.name, p.office);
    // 오류남. id 와 name 은 Professor class가 아니라 UnivMem 에서, Private 로 선언된 것.

    // UnivMem *a, *b를 선언해서 포인터 만들고, 그 포인터에 p, s의 주소값을 연결.
    UnivMem *a;
    UnivMem *b;
    a = &p;
    b = &s;

    a->Print();
    b->Print();

    return 0;
}