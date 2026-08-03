#include <iostream>
#include <string>
using namespace std;

template <typename T>

class student {
private :
    T student_name;
    T student_number;
    T student_class[4];
    int class_count = 0;

public:
    student(string name, int number) {
        this->student_name = name;
        this->student_number = number;
    }

    student(string clas) {
        this->student_class[class_count] = clas;
        class_count += 1;
    }

    string get_name() {return student_name;}
    int get_number() {return student_number;}
    string get_class() {return student_class[0];}
};


int main(void) {
    student<string> stu1("hyunjun", 20221627);
}