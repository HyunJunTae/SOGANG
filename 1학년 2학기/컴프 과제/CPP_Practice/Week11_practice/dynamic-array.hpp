#ifndef DYNAMIC_ARRAY
#define DYNAMIC_ARRAY

#include <iostream>

using namespace std;

template <typename T>
class DynamicArray {
private:
    T *ptr;
    int length;
public:
    // Constructor. Note that it does not matter even if 'arr' is empty and
    // 'len' is 0. Assume that input 'len' is always non-negative. Don't change
    // this function.
    DynamicArray(T arr[], int len) {
        ptr = new T[len];
        length = len;
        for (int i = 0; i < len; i++) {
            ptr[i] = arr[i];
        }
    }

    // Print the elements of the array one by one. Don't change this function.
    void Print() {
        for (int i = 0; i < length; i++) {
            cout << ptr[i];
            if (i == length - 1) {
                cout << endl;
            } else {
                cout << ", ";
            }
        }
    }

    // Destructor.
    ~DynamicArray();
    // Append array 'da' at the end of this array.
    void Append(DynamicArray *da);
    // Return a fresh subsequence that starts from 'idx' and includes up to 'n'
    // consecutive elements in this array. If 'idx' < 0 or 'idx' >= 'length',
    // return an empty DynamicArray. If the subsequence reaches the end before
    // including 'n' elements, return a subsequence that stops there.
    DynamicArray* Subseq(int idx, int n);
};



// TODO: Define the remaining methods here.
template <typename T>
DynamicArray<T>::~DynamicArray() {
    delete[] this->ptr;
}





template <typename T>
void DynamicArray<T>::Append(DynamicArray *da) {

    int k = this->length + da->length;

    // 새 배열 생성. 길이는 this + da.
    T *arr = new T[this->length];

    // 새 배열에 this->ptr 입력.
    for (int i=0; i<this->length; i++)
        arr[i] = this->ptr[i];
    
    // ptr 없애고 새로운 길이로 다시 만들기.
    delete[] this->ptr;
    ptr = new T[k];
    
    // this->ptr에 arr(기존 this->ptr) 입력.
    for (int i=0; i<this->length; i++) {
        this->ptr[i] = arr[i];
    }

    // this->ptr에 da->ptr 입력.
    int j = 0;
    for (int i=this->length; i<k; i++) {
        this->ptr[i] = da->ptr[j];
        j += 1;
    } 

    this->length = k;

    delete[] arr;
}





template <typename T>
DynamicArray<T>* DynamicArray<T>::Subseq(int idx, int n) {

    if (idx < 0 || idx >= this->length) {
        auto d = new DynamicArray<T>(NULL, 0);
        return d;
    }
    
    if (idx + n > this->length)
        n = this->length - idx;
    
    auto d = new DynamicArray<T>(this->ptr+idx, n);
    return d;
}


#endif