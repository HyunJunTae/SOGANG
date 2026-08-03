#include <stdio.h>

void toUpper(char arr[]);

int main(void){
    char sentence[50];
    
    for (int i=0; i<50; i++){
        char c;
        scanf("%c", &c);
        if (c=='\n'){
            break;
        }
        sentence[i]=c;
    }
    
    toUpper(sentence);
    
    printf("%s", sentence);
    
    printf("\n");
    return 0;
}

void toUpper(char arr[]){
    for (int i=0;i<50;i++){
        if (arr[i] >= 97 && arr[i] <= 122){
            arr[i] -= 32;
        }else{
            continue;
        }
    }
    return;
}