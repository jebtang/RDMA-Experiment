#include <stdio.h>
#include <string.h>

int main(){

  char *d="safsdafds";
  char *s;

  *d = *s;

  printf("%ld", strlen(d));

  printf("%ld", strlen(s));


  return 0;
}
