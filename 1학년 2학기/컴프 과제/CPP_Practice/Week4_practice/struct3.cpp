#include <iostream>
#include <cstdio>
#include <string>
using namespace std;

// 문자열을 배열에 담는게 아니라 string을 이용해서 담기.
// 출력을 iostream으로 하기.

struct StudentProfile{
    int id;
    string name;
    double gradePoint;
};

bool operator ==(const StudentProfile &x, const StudentProfile &y) {
    if (x.id == y.id)
        return true;
    return false;
}

void printProfile (StudentProfile x) {
    printf("%d\n", x.id);
    cout << x.name;
    printf("\n%.1f\n", x.gradePoint);
}

int main(void){
    StudentProfile chulsoo, avatar;

    chulsoo.id = 12345678;
    chulsoo.name = "Gang Chulsoo";
    chulsoo.gradePoint = 4.3;
    printProfile(chulsoo);

    avatar = chulsoo;
    avatar.name = "Chulsoo2";
    printProfile(avatar);

    return 0;
}