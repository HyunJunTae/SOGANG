#include <iostream>
#include <string>
using namespace std;

class student {
private :
    string student_name;
    int student_number;
    string student_class[4];
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
    student stu1("hyunjun", 20221627);
    student stu1("CSE_201");

    cout << stu1.get_name() << endl;
    cout << stu1.get_number() << endl;
    cout << stu1.get_class() << endl;
}