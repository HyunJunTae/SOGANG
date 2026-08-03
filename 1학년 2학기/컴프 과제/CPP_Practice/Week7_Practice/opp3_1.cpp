#include <stdio.h>
#include <string.h>
#define MAXLEN 32

class UnivMem {
protected:
    int id;
    char name[MAXLEN+1];
public:

    UnivMem(){};

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
        id = i;
        strcpy(name, n);
        GPA = g;
    }

    void Print(){
        printf("Undergrad : %s (%d)\n", name, id);
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

    void Print(){
        printf("Prof : %s (%d)\n", name, id);
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

    return 0;
}