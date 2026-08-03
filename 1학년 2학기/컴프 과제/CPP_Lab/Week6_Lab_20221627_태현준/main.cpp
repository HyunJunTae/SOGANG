#include <iostream>
#include "lecture.cpp"

using std::cout;
using std::endl;

// I will fix this function to test your class implementation
void test() {
    Lecture lec1("CSE2015");
    Lecture lec2("CSE2035");
    Grade grade;

    // Test with the first lecture.
    lec1.AddStudent(20220001);
    lec1.AddStudent(20220002);
    lec1.SetGradeOf(20220002, A);
    // This should work and print "A" as grade.
    if (lec1.GetGradeOf(20220002, &grade)) {
        cout << "Grade of 20220002: " << GradeToString(grade) << endl;
    }
    // Update on unregistered student should not work. Nothing must be printed.
    lec1.SetGradeOf(20220003, B);
    if (lec1.GetGradeOf(20220003, &grade)) {
        cout << "Grade of 20220003: " << GradeToString(grade) << endl;
    }

    // Test with the second lecture.
    lec2.AddStudent(20220001);
    lec2.AddStudent(20220002);
    // lec2.AddStudent(20220003);
    lec2.AddStudent(20220004);
    lec2.AddStudent(20220004);
    // We should not be able to register more than MAX_STUDENT students.
    lec2.AddStudent(20220005);
    lec2.SetGradeOf(20220005, C);
    // Fifth student must have been rejected, so nothing must be printed.
    if (lec2.GetGradeOf(20220005, &grade)) {
        cout << "Grade of 20220005: " << GradeToString(grade) << endl;
    }

    lec1.SetGradeOf(20220002, B);
    if (lec1.GetGradeOf(20220002, &grade)) {
        cout << "Grade of 20220002: " << GradeToString(grade) << endl;
    }
    if (lec2.GetGradeOf(20220002, &grade)) {
        cout << "Grade of 20220002: " << GradeToString(grade) << endl;
    }
    lec2.SetGradeOf(20220004, B);
    if (lec2.GetGradeOf(20220004, &grade)) {
        cout << "Grade of 20220004: " << GradeToString(grade) << endl;
    }
}

int main() {
    test();
    return 0;
}
