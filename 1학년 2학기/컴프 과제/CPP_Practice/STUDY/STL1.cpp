#include <stdio.h>
#include <vector>

int main(void) {



    std::vector<int> onggibot;

    for (int i=1; i<6; i++) {
        onggibot.push_back(i);
    }

    for(std::vector<int>::iterator it = onggibot.begin(); it != onggibot.end(); it++) {
        printf("%d ", *it);
    }
    printf("%\n");

    std::vector<int>::iterator it = onggibot.begin();
    printf("%d\n", *it);
    it = onggibot.end();
    printf("%d\n", *it);
    it--;
    printf("%d\n", *it);

    return 0;
}