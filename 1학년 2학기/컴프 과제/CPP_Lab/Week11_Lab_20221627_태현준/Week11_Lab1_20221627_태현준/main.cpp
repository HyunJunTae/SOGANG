#include <iostream>
#include "max.hpp"
#include "sort.hpp"

using namespace std;

// I will fix this file to test your implementation.

template <typename T>
void print_array(T arr[], int n) {
    for (int i = 0; i < n; i++) {
        cout << arr[i];
        if (i == n - 1) {
            cout << endl;
        } else {
            cout << ", ";
        }
    }
}

int main() {

    char arr[5] = {};
    find_max(arr, 0);
    print_array(arr, 5);

    int i_arr[5] = { 2, 8, 3, 5, 10 };
    double d_arr[4] = { 3.4, 9.7, 4.5, 6.3};

    find_max<int>(i_arr, 5);
    print_array<int>(i_arr, 5); // Must print "10, 8, 3, 5, 2"

    simple_sort<int>(i_arr, 5);
    print_array<int>(i_arr, 5); // Must print "10, 8, 5, 3, 2"

    find_max<double>(d_arr, 4);
    print_array<double>(d_arr, 4); // Must print "9.7, 3.4, 4.5, 6.3"

    simple_sort<double>(d_arr, 4);
    print_array<double>(d_arr, 4); // Must print "9.7, 6.3, 4.5, 3.4"
    return 0;
}
