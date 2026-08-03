#include <stdio.h>

void my_strcat(char first[], char second[]) {

  // first, second 각각의 자릿수를 나타내는 i, j 정의
  int i = 0, j = 0;

  // first 길이 i에 입력. sogang 입력하면 i = 6
  while(1) {
    if (first[i] == '\n') {
      break;
    }
    i++;
  }

  // first의 i 번째 자리에 second의 j 번째 자리 원소를 넣음.
  // 다 넣으면 first 마지막에 null 문자 넣기.
  while(1) {
    if(second[j] == '\0') {
      first[i] = '\0';
      break;
    }
    first[i] = second[j];
    i++;
    j++;
  }
}

int main() {

  char first[100];
  char second[100];

  printf("Enter the first string: ");
  fgets( first, 100, stdin );
  printf("Enter the second string: ");
  fgets( second, 100, stdin );

  printf("Concatenating the first string and the second string...\n");

  my_strcat(first, second);

  printf("Concatenated string: %s\n", first);

  return 0;
  
}