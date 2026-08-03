#include <cstdio>
#include <cstring>

struct StudentProfile{
    int id;
    char name[20];
    double gradePoint;
};

bool operator ==(const StudentProfile &x, const StudentProfile &y) {
    if (x.id == y.id)
        return true;
    return false;
}

void printProfile (StudentProfile x) {
    printf("%d\n", x.id);
    printf("%s\n", x.name);
    printf("%.1f\n", x.gradePoint);
}

int main(void){
    StudentProfile chulsoo, avatar;

    chulsoo.id = 12345678;
    // chulsoo.name = "Gang Chulsoo";
    // 이렇게 하면 안되네
    strcpy(chulsoo.name, "Gang Chulsoo");
    chulsoo.gradePoint = 4.3;
    printProfile(chulsoo);

    avatar = chulsoo;
    strcpy(avatar.name, "Chulsoo2");
    printProfile(avatar);

    return 0;
}