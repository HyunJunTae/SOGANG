// oop 1과 같은 클래스 내용 사용.
// UnivMem 이중포인터를 사용하는 내용. Upcasting.


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



void printMem(UnivMem **mems, int n) {
    // mems = [&p, &s] 2중포인터.
    for (int i=0; i<n; i++)
    // 자식클래스는 부모클래스의 member를 사용 가능. 자식클래스에 입력된 member중 부모클래스의 것을, 부모클래스의 Print()함수로 출력.
        mems[i]->Print();
}

int main(void) {
    // UnivMem 의 member인 2칸짜리 배열을 만들고, 그 배열에 p, s 의 주소를 넣는다.
    // members = [&p, &s]가 됨.
    // 왜 UnivMem으로 배열 만듬? Print() 사용하기 위해서.
    // 함수 사용할 때 int 가져가려면 함수 괄호에서도 (int num) 이런식으로 해주는 거랑 같음.
    UnivMem *members[2];
    Professor p(0, "Jaeseung Choi", "AS711");
    Student s(1, "GangSeo Kim", 4.3);

    members[0] = &p;
    members[1] = &s;

    printMem(members, 2);

    return 0;
}