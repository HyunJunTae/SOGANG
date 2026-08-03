#include <cstdio>
#include "lecture.hpp"
#include "student.hpp"

using std::cout;
using std::endl;


// TODO: Fill in these functions.

void Student::PrintGrade() {
    Grade g;

    for(int i=0; i<lecture_cnt; i++) {
        lectures[i]->GetGradeOf(GetID(), &g);
        printf("%s : %s\n", lectures[i], GradeToString(g));
    }

}

bool Student::Register(Lecture *lec) {
    // 만약 이 학생 강의 개수가 3개이면 return false
    if (this->lecture_cnt == 3)
        return false;

    // 만약 이 강의가 다 찼거나, 이미 이 학생이 듣고있다면 return false
    if(lec->AddStudent(GetID()) == false)
        return false;


    // 아니면
    this->lectures[lecture_cnt] = lec;
    this->lecture_cnt += 1;

    lec->AddStudent(GetID());

    return true;
}
