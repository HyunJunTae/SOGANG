#ifndef FIND_MAX
#define FIND_MAX

// TODO: Fill in this function.
template <typename T>   
void find_max(T arr[], int n) {

    if (arr[0] == '\0' && n == 0){
        return;
    }

    T max_num = arr[0];
    int max_point = 0;

    for(int i=0; i<n; i++) {
        if (max_num < arr[i]) {
            max_num = arr[i];
            max_point = i;
        }
    }

    T temp;
    temp = arr[max_point];
    arr[max_point] = arr[0];
    arr[0] = temp;
}

#endif
