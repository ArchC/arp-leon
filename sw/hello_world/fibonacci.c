#include <stdio.h>

int main()
{
   int n = 50;
   int i;
   printf("The %d first terms of Fibonnaci sequence: \n",n);
   for(i=0; i<n; i++){
       printf("%d ", fibonacci(i+1));
  }
   printf("\n");
   return 0;
}

int fibonacci(int num)
{
   if(num==1 || num==2)
       return 1;
   else
       return fibonacci(num-1) + fibonacci(num-2);
}
