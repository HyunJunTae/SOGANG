// 동적배열이 문제인가?? -> class8에서 정적배열을 동적배열로 바꾸기만 해봄.

#include <cstdio>
#include <stdlib.h>
#include <string.h>

class Student {
private:
    int year = 1;
    int id;
    char* name;
    size_t length;
public:
    Student(int id, const char* name) {
        this->id = id;

        length = strlen(name);
        printf("%d\n", length);
        this->name = (char*)malloc(length);
        memcpy(this->name, name, strlen(name));
        // this->name[strlen(name)] = '\0';
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
        realloc(this->name, this->length + s->length);
        strcat(this->name, s->name);
    } // 잘되는디.

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