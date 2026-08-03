#include <stdio.h>
#include <map>

int main (void) {

    std::map<char, int> onggibot1;
    std::map<char, int>::iterator it;


    onggibot1['a'] = 1;
    onggibot1['b'] = 2;
    onggibot1['c'] = 3;
    onggibot1['d'] = 4;
    onggibot1['e'] = 5;
    onggibot1['f'] = 6;
    onggibot1['A'] = 7;


    for (std::map<char, int>::iterator it = onggibot1.begin(); it != onggibot1.end(); it++) {
        printf("%c : %d  ", it->first, it->second);
    }
    printf("\n");

    it = onggibot1.begin();
    onggibot1.erase(it);
    onggibot1.erase('c');

    for (std::map<char, int>::iterator it = onggibot1.begin(); it != onggibot1.end(); it++) {
        printf("%c : %d  ", it->first, it->second);
    }
    printf("\n");

    std::map<char,int>::iterator it1 = onggibot1.find('b');
    std::map<char,int>::iterator it2 = onggibot1.find('e');
    it2;
    onggibot1.erase(it1, it2);

    for (std::map<char, int>::iterator it = onggibot1.begin(); it != onggibot1.end(); it++) {
        printf("%c : %d  ", it->first, it->second);
    }
    printf("\n");

    return 0;
}