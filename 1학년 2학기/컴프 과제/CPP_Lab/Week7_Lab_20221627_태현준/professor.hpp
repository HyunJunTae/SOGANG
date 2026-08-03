#ifndef PROFESSOR_CLASSS
#define PROFESSOR_CLASSS

#include <iostream>
#include <string.h>
#include "lecture.hpp"
#include "univmem.hpp"

#define MAX_NAME_LEN 32

using std::cout;
using std::endl;

class Professor: public UnivMem {
private:
    Lecture *lecture;
public:
    Professor(int _id, const char* _name, Lecture * _lecture):
        UnivMem(_id, _name), lecture(_lecture)
    { }

    // Update the student's grade in 'lecture'. Set the grade of the student
    // with 'student_id' into 'student_grade'. Return false if such a student
    // does not exist in 'lecture'.
    bool GradeStudent(int student_id, Grade student_grade);
};

#endif
