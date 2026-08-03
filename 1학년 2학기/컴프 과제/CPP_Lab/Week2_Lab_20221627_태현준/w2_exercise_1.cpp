#include <stdio.h>

char *my_strcpy(char* destination, const char* source) {

  for(int i=0; i<100; i++) {
    destination[i] = source[i];
  }

  return destination;

}


int main() {

  char source[100];
  char destination[100];

  printf("Enter the source string: ");
  fgets( source, 100, stdin );
  printf("Enter the destination string: ");
  fgets( destination, 100, stdin );

  printf("Copying source string into destination string...\n");

  my_strcpy(destination, source);

  printf("Source string: %s\n", source);
  printf("Destination string (copied from source string): %s\n", destination);

  return 0;
  
}