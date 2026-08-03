#include <stdio.h>
#include <string.h>
#define max_string_size 31
#define max_pattern_size 31

void pmatch_all(char* string, char* pat, int* failure) {

    int i=0;
    int j=0;

    int lens = strlen(string);
    int lenp = strlen(pat);

    int match_numbers[max_string_size];
    int count = 0;

    while (i < lens) {

        while (j < lenp) {

            if (i > lens) break;

        if (string[i] == pat[j]) {
            i++;
            j++;
        }
        else if (j == 0) i++;
        else j=failure[j-1] + 1;

        }

        if (i > lens) break;

        match_numbers[count] = i-lenp;
        count++;
        
        i = i - lenp + 1;
        j = 0;
    }

    for (int i=0; i<count; i++) {
        printf("%d\n", match_numbers[i]);
    }
    
};


void fail(char *pat, int* failure) {

    int i, j;
    int n = strlen(pat);
    failure[0] = -1;

    for (j = 1; j<n; j++) {
        i = failure[j-1];

        while ( (pat[j] != pat[i+1]) && (i>= 0)) i = failure[i];

        if (pat[j] == pat[i+1]) failure[j] = i+1;

        else failure[j] = -1;

    }
}

int main(void) {

    char string[max_string_size];
    char pat[max_pattern_size];
    int failure[max_pattern_size];

    scanf("%s", string);
    scanf("%s", pat);

    fail(pat, failure);

    pmatch_all(string, pat, failure);

    return 0;
}