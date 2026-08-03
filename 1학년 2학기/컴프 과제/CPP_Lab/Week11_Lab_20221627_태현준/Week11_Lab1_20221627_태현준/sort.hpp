#ifndef SIMPLE_SORT
#define SIMPLE_SORT

#include "max.hpp"

// TODO: Fill in this function.
template <typename T>
void simple_sort(T arr[], int n) {

    int k=0;
    for (int i=0; i<n; i++) {
            find_max (arr+k, n);
            k += 1;
            n -= 1;
    }
}

#endif
