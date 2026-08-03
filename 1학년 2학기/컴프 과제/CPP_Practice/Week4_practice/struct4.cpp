#include <cstdio>
#include <cstring>

struct StudentProfile{
    static int total;
    int id;
    char name[26];
    double gradePoint;
};

int StudentProfile::total = 10;

int main(void) {
    StudentProfile a, b;

    printf("%d %d\n", a.total, b.total);
    a.total = 0;
    printf("%d %d\n", a.total, b.total);
    a.total += 1;
    b.total += 1;
    printf("%d %d\n", a.total, b.total);

    return 0;
}