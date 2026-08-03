#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <algorithm>

using namespace std;

struct StudentProfile{
    int id;
    char name[256];
    double gradePoint;
};

bool operator <(const StudentProfile x, const StudentProfile y){

    // Task 1: Compare gradePoints of two students and return x.gradePoint < y.gradePoint.
    return x.gradePoint < y.gradePoint;

}

StudentProfile students[100];
int studentN;

void printStudents(){

    printf ("\nThe list of students.\n");

    for(int i=0; i < studentN; i++){
       
        // Task 2: Print the profile of every student.
        printf("%s %d %f\n", students[i].name, students[i].id, students[i].gradePoint);
    }
}

bool cmp_id (const StudentProfile x, const StudentProfile y){

    // Task 3: Compare ids of two students and return x.id < y.id
    return x.id < y.id;

    

}
bool cmp_id_dec (const StudentProfile x, const StudentProfile y){

    // Task 4: Compare ids of two students and return x.id > y.id
    return y.id < x.id;


}

 int main(){
    std::ifstream in ("input.txt", ifstream::in);
    
    in >> studentN;

    char buf [256];
    for(int i=0; i < studentN; i++){

        in.getline(buf, 256); // due to the remained characters.
        in.getline(buf, 256);

        strncpy ( students[i].name, buf, strlen (buf) );

        in >> students[i].id;
        in >> students[i].gradePoint;
    }

    std::sort (students, students + studentN);

    printStudents();
    
    std::sort (students, students + studentN, cmp_id);

    printStudents();
        
    std::sort (students, students + studentN, cmp_id_dec);

    printStudents();
    
    in.close();

    return 0;
}