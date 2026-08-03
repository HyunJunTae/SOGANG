#ifndef UNIVMEM_CLASSS
#define UNIVMEM_CLASSS

#include <iostream>
#include <string.h>

#define MAX_NAME_LEN 32

using std::cout;
using std::endl;

class UnivMem {
private:
    int id;
    char name[MAX_NAME_LEN + 1];
public:
    UnivMem(int _id, const char* _name): id(_id) {
        strncpy(name, _name, MAX_NAME_LEN);
        name[MAX_NAME_LEN] = '\0';
    }

    int GetID() { return id; }

    const char* GetName() { return name; }

    void Print() {
        cout << name << "(" << id << ")" << endl;

    }
};

#endif
