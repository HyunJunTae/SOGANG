#include <stdio.h>
#define SWAP(x, y, t) ((t) = (x), (x) = (y), (y) = (t))

void sort(int list[], int n)
{

    int t, min;

    for (int i = 0; i < n-1; i++)
    {
        min = i;

        for (int j = i + 1; j < n; j++)
        {
            if (list[j] < list[min])
            {
                min = j;
            }
        }
        SWAP(list[i], list[min], t);
    }
}

int main(void)
{

    // int list[5] = {5, 8, 3, 1, 4};
    int list[5] = {5, 4, 2, 3, 1};

    sort(list, 5);

    for (int i = 0; i < 5; i++)
    {
        printf("%d ", list[i]);
    }

    return 0;
}