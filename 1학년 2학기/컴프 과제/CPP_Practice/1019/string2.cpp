#include <stdio.h>
#include <string>
#include <iostream>
using namespace std;


int main(void) {

    // string s1 = "hi" + 41;
    string s1 = "hi";
    s1 += 41;
    cout << "string s1 : " << s1 << endl;
    


    // string s2 = "hi" + '?';
    string s2 = "hi";
    s2 += '?';
    cout << "string s2 : " << s2 << endl;


    // string s3 = "hi" + "there";
    string s3 = "hi";
    s3 += "there";
    cout << "string s3 : " << s3 << endl;





    return 0;
}