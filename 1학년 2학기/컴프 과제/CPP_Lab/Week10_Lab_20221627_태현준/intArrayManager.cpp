#include <cstdio>
#include "intArrayManager.h"

IntArrayManager::IntArrayManager(){

    this->length = 0;
    this->arr = NULL;
}

IntArrayManager::IntArrayManager(int len, int *array){

    this->length = len;

    this->arr = new int[len];
    for (int i=0; i<len; i++)
        this->arr[i] = array[i];
}

IntArrayManager::~IntArrayManager(){

    delete[] this->arr;
}

bool IntArrayManager::compare ( IntArrayManager &y ){

    int a;
    bool b;
    if (this->length > y.length) {
        a = y.length;
        b = false;
    }
    else {
        a = this->length;
        b = true;
    }

    for(int i=0; i<a; i++){
        if (this->arr[i] > y.arr[i])
            return false;
        else if (this->arr[i] == y.arr[i])  
            continue;
        else
            return true;
    }

    return b;
}

void IntArrayManager::print(){

    printf("array (length %d) : ", this->length);
    for(int i=0; i<this->length; i++)
        printf("%d ", this->arr[i]);
    printf("\n");

}

void IntArrayManager::concatenate ( IntArrayManager &y ){

    int a = this->length;
    int b = y.length;

    int c[a];
    for (int i=0; i<a; i++)
        c[i] = this->arr[i];

    delete[] this->arr;
    this->arr = new int[a+b];
    this->length = a+b;

    for (int i=0; i<a; i++)
        this->arr[i] = c[i];
    for (int i=0; i<b; i++)
        this->arr[a+i] = y.arr[i];
}