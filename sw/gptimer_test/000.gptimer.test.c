/**
 * @file      000gptimer.test.c
 * @author    Rogerio Alves Cardoso
 *
 *            Computer Systems Laboratory (LSC)
 *            IC-UNICAMP
 *            http://www.lsc.ic.unicamp.br
 *
 * @version   1.0
 * @date      
 * @brief     It is a simple program to test GPTIMER access
 *            this program just access GPTIMER
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
 struct gptimer *timers = (struct gptimer *) addr;

 printf("Read Configuration Register: 0x%x\n", timers->configreg);

 exit(0); // To avoid cross-compiler exit routine
 return 0; // Never execute just for compatibility
}
