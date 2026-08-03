// This is called a 'header guard'
#ifndef MY_STRING
#define MY_STRING

#include <cstdio>
#include <string.h>
#include <stdlib.h>

class MyString {
private:
    char *arr;
    size_t length;
public:
    // Let's promise to implement our string this way.
    MyString(const char *s) {
        length = strlen(s);
        if (length == 0) {
            arr = NULL;
        }
        else {
            arr = (char*) malloc(length);
            memcpy(arr, s, length);
        }
    }

    void Print() {
        for (int i = 0; i < length; i++) {
            putchar(arr[i]);
        }
        putchar('\n');
    }

    // Destructor.
    ~MyString();
    // Get the length of the string.
    size_t GetLength();
    // Make the string into an empty string.
    void Clear();
    // Append 's' at the end of this string.
    void Append(MyString *s);
    // Insert 's' at position 'idx' of this string. idx=0 means the start.
    void Insert(MyString *s, int idx);
    // Starting from the position 'idx', remove 'n' characters.
    void Remove(int idx, int n);
};

#endif
