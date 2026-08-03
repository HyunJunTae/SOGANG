#include <iostream>
#include <map>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <cstring>

#define MAX_SUBSTR_N 20

using namespace std;

int main(){

    char str[100] = "dkfjdkfjkdfkdjkfdkjfkdjkdkfjkkdkkfkkkdjffjdkjfkdkjf";
    char substr[MAX_SUBSTR_N];
    int sublen = 3;
    substr[3]='\0';

    map<string, int> dict;

    int len = strlen ( str );

    for (int i=0;i<len-sublen+1;i++){
        //TODO: add a n-substring in map container.
        map<string, int>::iterator it;

        substr[0] = str[i];
        substr[1] = str[i+1];
        substr[2] = str[i+2];

        if (dict.find(substr) == dict.end()) {
            dict[substr] = 1;
        }
        else {
            dict[substr] += 1;
        }

    }

    //TODO: print information in map.
    for(map<string, int>::iterator it = dict.begin(); it != dict.end(); it++) {
        cout << it->first << " " << it->second << '\n';
    }

    return 0;
}