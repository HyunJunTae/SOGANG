#include <stdio.h>

int compare(int x, int y) {
    if (x<y) return -1;
    else if (x==y) return 0;
    else return 1;
}

#define COMPARE(x, y) (x<y) ? -1 : (x==y) ? 0:1

int binsearch(int list[], int searchnum, int left, int right)
{

    while (left <= right)
    {
        int middle = (left + right) / 2;


        switch (compare(searchnum, list[middle])) {
            case -1 : right = middle - 1;
            case 0  : return middle;
            case 1  : left = middle + 1; 
        }
    }

    return -1;
}

int binsearch2(int list[], int searchnum, int left, int right)
{

    while (left <= right)
    {
        int middle = (left + right) / 2;

        switch (COMPARE(searchnum, list[middle]))
        {
        case -1:
            right = middle - 1;
        case 0:
            return middle;
        case 1:
            left = middle + 1;
        }
    }

    return -1;
}

int main(void)
{

    int list[10] = {1, 3, 4, 6, 8, 9, 11, 13, 24, 25};

    int result = binsearch2(list, 13, 0, 9);

    printf("%d", result);

    return 0;
}