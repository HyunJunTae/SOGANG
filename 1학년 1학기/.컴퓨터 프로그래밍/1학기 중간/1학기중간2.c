#include <stdio.h>

 void caesar(char* alphabet, int key);

 int main(void){
    int key;
    char alphabet1, alphabet2, alphabet3;
    scanf("%d %c", &key, &alphabet1);
    caesar(&alphabet1, key);
    scanf("%c", &alphabet2);
    caesar(&alphabet2, key);
    scanf("%c", &alphabet3);
    caesar(&alphabet3, key);
    printf("%c%c%c\n", alphabet1, alphabet2, alphabet3);
    return 0;
 }

 void caesar(char* alphabet, int key){
    *alphabet += key;
    int check_alphabet = *alphabet;
    if (check_alphabet < 97 ){
        *alphabet += 26;
    }
    if(check_alphabet > 122){
        *alphabet -= 26;
    }
 }