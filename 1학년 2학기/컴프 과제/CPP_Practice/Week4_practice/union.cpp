#include <iostream>
#include <cstring>
using namespace std;

union student {
    int id;
    int phone_num;
    char name[30];
};

int main(void) {

    union student s1;

    // s1.id = 25;
    s1.phone_num = 0101234567;
    strcpy(s1.name, "Euhyun");
    s1.id = 25;

    cout << "ID : " << s1.id << endl;
    cout << "Phone number : " << s1.phone_num << endl;
    cout << "Name : " << s1.name << endl;

    return 0;
}

