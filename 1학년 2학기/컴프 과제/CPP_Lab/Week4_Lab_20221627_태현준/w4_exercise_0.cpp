#include <cstdio>
#include <iostream>
#include <string>
using namespace std;

// Task 1: Define enum Attendance.
// your code here (just a single line of code to define enum)
enum Attendance {Late, Attend, Absent};

struct Student{
    std::string name;
    Attendance att;
};


void printStudent(Student stu){

    // Task 2: Print student's status.
    // your code here
    // printf("%s is %s", stu.name, stu.att);
    string a;
    if (stu.att == 0)
        a = "Late";
    else if (stu.att == 1)
        a = "Attend";
    else
        a = "Absent";
    
    cout << stu.name << " is " << a << endl;
}


int main () {

    Student student1, student2, student3;

    student1.name = std::string ("John");
    student1.att = Late;

    student2.name = std::string ("Caro");
    student2.att = Attend;

    student3.name = std::string ("Yiyi");
    student3.att = Absent;

    printStudent ( student1 );
    printStudent ( student2 );
    printStudent ( student3 );

    return 0;
}