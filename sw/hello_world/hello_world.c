#include <stdio.h> 

int main(int ac, char *av[]){
  int i;
  for(i=0;i<10;i++)
    printf("Hi from processor SPARC v8!\n");
  exit(0); // To avoid cross-compiler exit routine
  return 0; // Never executed, just for compatibility
}

