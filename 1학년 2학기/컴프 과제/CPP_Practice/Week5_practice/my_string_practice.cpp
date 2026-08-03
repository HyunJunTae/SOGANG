#include <cstdio>
#include <string.h>
#include <stdlib.h>
#include <iostream>

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

    // Get the length of the string.
    size_t GetLength() {
        // int num = 0;

        // while(1) {
        //     if (this->arr[num] != '\0')
        //         num += 1;

        //     else if (this->arr[num] == '\0')
        //         break;
        // }
        // return num;

        return strlen(this->arr);
    }

    // Make the string into an empty string.
    void Clear() {
        // int i=0;

        // while(1) {
        //     arr[i] = '\0';
        //     i++;

        //     if (arr[i] == '\0')
        //         break;
        // }
        int k = strlen(this->arr);
        for(int i=0; i<k; i++) {
            this->arr[i] = '\0';
        }
    }

    // Append 's' at the end of this string.
    void Append(MyString *s) {
        realloc(this->arr, strlen(this->arr)+strlen(arr));

        printf("%s\n", this->arr);
        printf("%s\n", arr);

        // int i;
        // i = GetLength();
        // int j=0;

        // while(1) {
        //     this->arr[i] = arr[j];
        //     i++;
        //     j++;
        //     if (arr[j] == '\0')
        //         break;
        // }

        // strcat(this->arr, s.name);`

    }

    // Insert 's' at position 'idx' of this string. idx=0 means the start.
    void Insert(MyString *s, int idx) {
        int i;
        i = GetLength();

        if (idx > i)
            return;

        for(int j = i; j>idx; j--) {
            arr[j] = arr[j-1];
        }

        arr[idx] = 's';
    }

    // Starting from the position 'idx', remove 'n' characters.
    void Remove(int idx, int n) {
        int i;
        i = GetLength();

        if (idx > i)
            return;

            
        for(int i=idx; i<idx+n; i++) 
            arr[i] = '\0';

        for(int i=idx; i<idx+n; i++) {
            if (arr[i+n] != '\0')
                arr[i] = arr[i+n];
        }

    }
};



using std::cout;
using std::endl;

// I will fix this function to test your class implementation
void test() {
    MyString s1("ABC");
    MyString s2(" DEF");
    // s1.Append(&s2);
    s1.Print();
    cout << s1.GetLength() << endl;

}

int main() {
    test();
    return 0;
}
