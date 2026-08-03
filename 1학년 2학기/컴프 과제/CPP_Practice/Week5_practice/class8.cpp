


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

    void cat(Student *s) {
        int i = strlen(this->name);

        strcat(this->name, "s");
    } // 이건 됨.

    void cat2(Student *s) {
        int i = strlen(this->name);

        this->name[i] = 's';
    } // 얘도 돼.

    void cat3(Student *s) {
        strcat(this->name, s->name);
    }

    void Print() {
        printf("Id : %d\n", id);
        printf("Name : %s\n", name);
    }
};

int main(void) {
    Student x(0, "John");
    Student y(1, "Mary");
    
    x.Print();
    x.cat3(&y);
    x.Print();

    return 0;
}