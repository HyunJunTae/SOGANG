#include <stdio.h>
#include <string>
#include <iostream>
using namespace std;

int main(void) {
    
    string a = "abcdef";
    string b = "abcdeg";

    a > b ? printf("a>b\n") : printf("a<b\n");

    int num = a.compare(b);
    cout << "compare : " << num << endl;

    
    string c = a.substr(1,3);
    cout << "substr : " << c << endl; // string은 printf로 출력할 수 없음. 하려면 c_str로 string을 변형해줘야함.
    // printf("substr : %s\n", c.c_str());


    c.insert(1,a);
    cout << "insert : " << c << endl;


    c.replace(0, 6, b);
    cout << "replace : " << c << endl;


    c.erase(3, 10);
    cout << "erase : " << c << endl;


    int StartLoc = a.find(c);
    cout << "find : " << StartLoc << endl;
    StartLoc = a.rfind(c);
    cout << "rfind : " << StartLoc << endl;

    string z = "abccbd";
    StartLoc = z.find('b');
    cout << "find : " << StartLoc << endl;
    StartLoc = z.rfind('b');
    cout << "rfind : " << StartLoc << endl;

    



    return 0;

}