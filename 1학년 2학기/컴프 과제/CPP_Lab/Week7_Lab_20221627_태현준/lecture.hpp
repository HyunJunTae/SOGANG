// This is called a 'header guard'
#ifndef LECTURE_CLASSS
#define LECTURE_CLASSS

#include <string.h>

#define MAX_CODE_LEN 32
#define MAX_STUDENT 4

enum Grade { A, B, C, NA }; // NA means "not available yet".

inline const char * GradeToString(Grade g) {
    switch(g) {
        case A:
            return "A";
        case B:
            return "B";
        case C:
            return "C";
        case NA:
        default:
            return "NA";
    }
}

class Lecture {
private:
    char code[MAX_CODE_LEN + 1];
    int student_cnt;
    int ids[MAX_STUDENT];
    Grade grades[MAX_STUDENT];
public:
    Lecture(const char * _code): student_cnt(0) {
        strncpy(code, _code, MAX_CODE_LEN);
        code[MAX_CODE_LEN] = '\0';
    }

    const char* GetCode() { return code; }

    // Add a student whose ID is 'id'. The student's grade must be initialized
    // as 'NA'. Store the information of the student at 'ids[student_cnt]' and
    // 'grades[student_cnt]'. After adding the student, increase 'student_cnt'
    // by one and return true. If the lecture is already full or 'id' is already
    // present in this lecture, don't add the student and just return false.
    bool AddStudent(int id);

    // Update the grade of the student whose ID is 'id' into 'grade'. If such
    // a student doesn't exist, just return false.
    bool SetGradeOf(int id, Grade grade);

    // Get the grade of the student whose ID is 'id', and store it at '*pGrade'.
    // If such a student doesn't exist, just return false.
    bool GetGradeOf(int id, Grade *pGrade);
};

#endif
