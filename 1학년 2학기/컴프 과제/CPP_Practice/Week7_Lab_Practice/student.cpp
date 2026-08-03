#include <cstdio>
#include "lecture.hpp"
#include "student.hpp"

using std::cout;
using std::endl;


// TODO: Fill in these functions.

void Student::PrintGrade() {
    Grade g;

    for(int i=0; i<lecture_cnt; i++) {
        lectures[i]->GetGradeOf(GetID(), &g); // 식에 클래스 포인터 형식이 있어야 하는데 "char*" 형식이 있음.
        // GetGradeOf 를 사용하기 위해서는 Lecture 클래스의 포인터가 필요하다?
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
    this->lectures[lecture_cnt] = lec; // "Lecture*" 형식의 값을 "char*" 형식의 엔터티에 할당할 수 없습니다.
    // Lecture 형식의 값은 Lecture에만 담을 수 있다.
    // int에는 int만, char에는 char만 담을 수 있듯이.
    this->lecture_cnt += 1;

    // lec->AddStudent(GetID());

    return true;
}
