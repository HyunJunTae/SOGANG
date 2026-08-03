#include <stdio.h>


int binsearch(int list[], int searchnum, int left, int right) {

    while(left <= right) {
        int middle = (left+right)/2;

        if (searchnum == list[middle]) {
            return middle;
        }
        else if (searchnum < list[middle])
        {
            right = middle-1;
        }
        else {
            left = middle+1;
        }
    }

    return -1;
}

int main(void) {

    int list[10] = {1,3,4,6,8,9,11,13,24,25};
    
    int result = binsearch(list, 13, 0, 9);

    printf("%d", result);

    return 0;
}