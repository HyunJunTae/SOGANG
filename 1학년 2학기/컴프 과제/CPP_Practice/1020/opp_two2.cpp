// inheritance protected로.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXLEN 32

class UnivMem {
protected:
    int id;
    char name[MAXLEN+1];
public:
    UnivMem(){};

    UnivMem(int id, const char *name) {
        this->id = id;
        strcpy(this->name, name);
    }

    virtual void Print() {
        printf("Id : %d\n", this->id);
        printf("Name : %s\n", this->name);
    }
    // virtual void Print() = 0;
};

class Student : public UnivMem {
private:
    double GPA;
public:
    Student(int id, const char* name, double g) : UnivMem(id, name) {
        this->GPA = g;
    }
    void Print(){
        printf("Undergrad. Id : %d, Name : %s\n", this->id, this->name);
    }
};


int main(void) {
    Student x(0, "Chulsoo", 4.3);
    UnivMem *s1 = &x;
    UnivMem y(0, "mary");

    x.Print();
    s1->Print();

    return 0;
}