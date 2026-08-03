#include <iostream>
using std::cout;
using std::endl;

int main(void) {
    // char *pchar = (char*)malloc(sizeof(char));
    char* pchar = new char;
    *pchar = 'a';
    cout << "memory address: " << (void*)pchar << "\tvalue : " << *pchar << endl;
    cout << "heap size : " << sizeof(*pchar) << endl;
    delete pchar;
}