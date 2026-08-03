#include <cstdio>
#include <string.h>

#define MAXLEN 32

class UnivMem {
protected:
    int id;
    char name[MAXLEN + 1];
public:
    UnivMem(int i, const char *n) {
        id = i;
        strcpy(name, n);
    }
    virtual void Print() = 0; // 어차피 virtual 하면 다음 주소로 redirect 시키는 거니까, 여기의 Print에서는 안에 내용 담을 필요 없음.
};

class Student: public UnivMem {
private:
    double GPA; // Grade Point Average
public:
    Student(int i, const char *n, double g) : UnivMem(i, n) {
        GPA = g;
    }
	void Print() {
        printf("Undergrad. %s (%d)\n", name, id);
    }
};

class Professor: public UnivMem {
private:
    char office[MAXLEN + 1]; // Ex)"AS813"
public:
    Professor(int i, const char *n, const char *o) : UnivMem(i, n) {
        strcpy(office, o);
    }
    void Print() {
        printf("Prof. %s (%d)\n",
               name, id);
    }
};

void printMem(UnivMem **mems, int n) {
    for (int i = 0; i < n; i++)
        mems[i]->Print();
}

int main() {
    UnivMem *members[2];
    Professor p(0, "Euhyun Moon", "AS813");
    Student s(1, "YoonSeo Jung", 4.3);
    //UnivMem m(2, "John"); // Not allowed.
    members[0] = &p;
    members[1] = &s;
    printMem(members, 2);
    return 0;
}
