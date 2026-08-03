#include <iostream>
#include "lecture.cpp"
#include "student.cpp"
#include "professor.cpp"

// I will fix this function to test your class implementation
void test() {
    Lecture lec1("CSE2015");
    Lecture lec2("CSE2035");
    Lecture lec3("CSE2055");
    Lecture lec4("CSE2075");
    Professor p(102233, "Jaeseung", &lec1);
    Professor o(102234, "Hajin", &lec3);
    Student s1(20220001, "Alice");
    Student s2(20220002, "Brian");

    s1.Register(&lec1);
    s1.Register(&lec2);
    s1.Register(&lec3);
    s1.Register(&lec4);
    p.GradeStudent(20220001, B);
    // p.GradeStudent(20220001, C);
    o.GradeStudent(20220001, A);
    s1.Print();
    s1.PrintGrade();
    cout << "=============\n";

    s2.Register(&lec1);
    s2.Register(&lec2);
    p.GradeStudent(20220002, C);
    o.GradeStudent(20220001, A);
    s2.Print();
    s2.PrintGrade();
    cout << "=============\n";
}

int main() {
    test();
    return 0;
}
