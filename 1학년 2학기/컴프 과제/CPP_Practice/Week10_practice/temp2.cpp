#include <iostream>
#include <string.h>
#include <string>
using namespace std;

// std::max 라는 함수가 std안에 내장되어있어서 아래 함수들의 이름을 max로 하면 오류 발생. maxx로 함.
template <typename T>
T maxx(T x, T y) {
    if (x >= y)
        return x;
    else
        return y;
}
char* maxx(char* s1, char* s2) {
    printf("*");
    if (strcmp (s1, s2) >= 0)
        return s1;
    else
        return s2;
}


int main(void) {
    char s1[4] = "abc";
    char s2[4] = "abd";
    string s3 = "ABC";
    string s4 = "ABD";
    
    cout << maxx(3, 2) << endl;
    cout << maxx(3.5, 9.8) << endl;
    cout << maxx(s1,s2) << endl;
    cout << maxx(s3, s4) << endl;

    // printf("int\t:\t%d\n", maxx(3, 2));
    // printf("double\t:\t%.2f\n", maxx(3.5, 9.8));
    // printf("char\t:\t%s\n", maxx(s1, s2));
    // printf("string\t:\t%s", maxx(s3, s4));
    // // string은 printf로 출력하면 글자 깨짐.
    return 0;
}