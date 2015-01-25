/*
*                Standalone BogoMips program
*
* Based on code Linux kernel code in init/main.c and
* include/linux/delay.h
*
* For more information on interpreting the results, see the BogoMIPS
* Mini-HOWTO document.
*
* version: 1.3
*  author: Jeff Tranter (Jeff_Tranter@Mitel.COM)
*/

#include <stdio.h>
#include <time.h>


/* portable version */
static void delay(int loops)
{
 long i;
 printf("Delay time: %d\n",loops);
 for (i = loops; i >= 0 ; i--);
}

int
main(void)
{
 unsigned long loops_per_sec = 1;
 unsigned long ticks;

 printf("Calibrating delay loop.. ");
 fflush(stdout);

 while ((loops_per_sec <<= 1)) { //Provavelmente ele fica preso nesse loop
   ticks = clock();
   delay(loops_per_sec);
   ticks = clock() - ticks;

   printf("Debug:: Ticks %d Clock: %d CLOCKS_PER_SEC %d\n",ticks,clock(),CLOCKS_PER_SEC);
   if (ticks >= CLOCKS_PER_SEC) {
     loops_per_sec = (loops_per_sec / ticks) * CLOCKS_PER_SEC;
     printf("ok - %lu.%02lu BogoMips\n",
        loops_per_sec/500000,
        (loops_per_sec/5000) % 100
        );
     return 0;
   }
 }
 printf("failed\n");
 return -1;
}
