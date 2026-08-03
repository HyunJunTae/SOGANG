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
    void Print() {
        printf("%s (%d)\n", name, id);
    }
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

int main() {
    Professor p(0, "Euhyun Moon", "AS813");
    Student s(1, "YoonSeo Jung", 4.3);
    UnivMem m(2, "Onggibot");
    UnivMem *m1 = &p;
    UnivMem *m2 = &s;
    p.Print();
    s.Print();
    m.Print();
    m1->Print();
    m2->Print();
    return 0;
}