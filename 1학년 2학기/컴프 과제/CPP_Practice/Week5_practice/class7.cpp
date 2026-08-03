#include <cstdio>
#include <string.h>
#define MAXLEN 32

class Student {
private:
    int year = 1;
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
};

int main(void) {
    Student x(0, "John");
    
    x.Print();

    return 0;
}