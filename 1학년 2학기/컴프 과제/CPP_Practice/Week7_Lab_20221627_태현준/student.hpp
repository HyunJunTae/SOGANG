#ifndef STUDENT_CLASSS
#define STUDENT_CLASSS

#include <iostream>
#include <string.h>
#include "lecture.hpp"
#include "univmem.hpp"

#define MAX_NAME_LEN 32
#define MAX_LECTURE 3

class Student: public UnivMem {
private:
    int lecture_cnt;
    Lecture* lectures[MAX_LECTURE];
public:
    Student(int _id, const char *_name): UnivMem(_id, _name), lecture_cnt(0)
    { }

    // Print the grades of this student. For each of the lecture, print its
    // lecture code and the student's grade in the following format:
    // "<lecture code> : <grade string>\n"
    // You can use GradeToString() function provided in lecture.hpp file.
    void PrintGrade();

    // If this student can take more lectures, try to add this student to 'lec'.
    // Return false if this registration was unsuccessful. If the registration
    // was successful, update 'lectures' and 'lecture_cnt' properties to reflect
    // the registration, and then return true.
    bool Register(Lecture *lec);
};

#endif
