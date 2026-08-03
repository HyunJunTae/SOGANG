#include <cstdio>
#include "lecture.hpp"
#include "professor.hpp"

// TODO: Fill in these functions.

bool Professor::GradeStudent(int student_id, Grade student_grade) {

    // student_id 가 
    if (lecture->SetGradeOf(student_id, student_grade) == false)
        return false;

    // lecture->SetGradeOf(student_id, student_grade);
    return true;
}