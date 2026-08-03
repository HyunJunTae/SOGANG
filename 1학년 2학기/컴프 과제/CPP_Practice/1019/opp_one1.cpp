#include <stdio.h>
#include <string.h>
#define MAXLEN 32

class Student {
private:
    int year = 1;
    int id;
    char name[MAXLEN+1];
public:
    Student(int _id, const char* name): id(_id) {
        printf("this = %p\n", this);
        strncpy(this->name, name, MAXLEN);
        this->name[MAXLEN] = '\0';
    }
};


int main(void) {
    Student x(0, "JiChul");
    printf(" &x = %p\n", &x);
    return 0;
}