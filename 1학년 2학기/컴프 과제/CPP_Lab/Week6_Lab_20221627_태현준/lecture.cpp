#include "lecture.hpp"

// TODO: Fill in these functions.

bool Lecture::AddStudent(int id) {

    // 배열 다 차있는지 확인
    if (student_cnt == 4) {
        return false;
    }

    // 입력받은 id가 이미 있는지 확인
    for (int i=0; i<4; i++) {
        if (ids[i] == id) {
            return false;
        }
    }

    this->ids[student_cnt] = id;
    this->grades[student_cnt] = NA;

    student_cnt += 1;

    return true;


}

bool Lecture::SetGradeOf(int id, Grade grade) {

    // id가 ids배열 안에 몇 번째 자리에 있는지 확인.
    // 자리를 place에 저장
    int place = 4;
    for (int i=0; i<4; i++) {
        if (ids[i] == id)
            place = i;
    }

    // ids배열 안에 없으면 return false.
    if (place == 4)
        return false;

    // 있으면
    grades[place] = grade;

    return true;
}

bool Lecture::GetGradeOf(int id, Grade *pGrade) {
    
    // id가 ids배열 안에 몇 번째 자리에 있는지 확인.
    // 자리를 place에 저장
    int place = 4;
    for (int i=0; i<4; i++) {
        if (ids[i] == id)
            place = i;
    }

    // ids배열 안에 없으면 return false.
    if (place == 4)
        return false;

    // 있으면
    *pGrade = grades[place];

    return true;
}

