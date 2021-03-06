/**
 * @file      002gptimer.test.c
 * @author    Rogerio Alves Cardoso
 *
 *            Computer Systems Laboratory (LSC)
 *            IC-UNICAMP
 *            http://www.lsc.ic.unicamp.br
 *
 * @version   1.0
 * @date      
 * @brief     It is a simple program to test GPTIMER test all basic functions
 *            this program is based on gptimer test program from grlib
 *            
 *
 * @attention Copyright (C) 2002-2006 --- The ArchC Team
 * 
 * This program is free software; you can redistribute it and/or modify 
 * it under the terms of the GNU General Public License as published by 
 * the Free Software Foundation; either version 2 of the License, or 
 * (at your option) any later version. 
 * 
 * This program is distributed in the hope that it will be useful, 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 * GNU General Public License for more details. 
 * 
 * You should have received a copy of the GNU General Public License 
 * along with this program; if not, write to the Free Software 
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#include <stdio.h>
#include "gptimer_test.h"


int main(int ac, char *av[]){
 
 unsigned int addr = GPTIMER_BASEADDR;
 unsigned int i;
 struct gptimer *timers = (struct gptimer *) addr;
 
 timers->scalercnt = 31;
 timers->scalerload = 31;
 /*timer reset*/
 for(i = 0; i < 4; i++){timers->timer[i].control = 0;}
 
 /*test basic functions*/
 for(i = 0; i < 4; i++){
     
     timers->timer[i].counter = 0;/*Reset Counters*/
     timers->timer[i].reload = 15;/*Reload Time*/
     timers->timer[i].control = 0x6;/*Configure Control Reg*/
 }
 for(i =0; i<100000;i++){}//Need some time to update

 for(i = 0; i < 4; i++){
     timers->timer[i].control = 0xf;/*Enable Timers*/    
 }
 for(i =0; i<10000000;i++){}//Need some to se counters running
 /**Check with gdb if the irq is sent to controller*/

 exit(0); // To avoid cross-compiler exit routine
 return 0; // Never execute just for compatibility
}
