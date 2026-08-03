// inheritance protected로.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXLEN 32

class Student {
private:
    int id;
    char name[MAXLEN+1];
public:
    Student(int id, const char* name) {
        this->id = id;
        strncpy(this->name, name, MAXLEN);
        this->name[MAXLEN] = '\0';
    }

    void Print() {
        printf("Id : %d\n", id);
        printf("Name : %s\n", name);
    }

    void cat(Student *s) {
        strcat(this->name, s->name);
    }
};

int main(void) {
    Student x(0, "Chulsoo");
    Student y(1, "Mary");

    x.Print();
    x.cat(&y);
    x.Print();

    return 0;
}