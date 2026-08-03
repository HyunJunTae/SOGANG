#include <stdio.h>

int main(void) {
  char buf[32];
  int n;
  // ... More variables may exist

  puts("Provide your input:");
  scanf("%31s", buf);
  n = strlen(buf);

  // ... More code will follow

}
