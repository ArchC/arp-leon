/**
 * \file leon_cpu_helper.H                 				
 *															
 * \brief LEON-3/SPARC v8 CPU helper file.                			
 * 												            
 * \author											    
 *  Rogerio Alves Cardoso								    
 *  http://www.lsc.ic.unicamp.br  							
 *														    										
 *															
 * \description                                           
 *  Complex macro definition and stub methods			   
 *															
 *															
 * \attention 
 *  Copyright (C) 2002-2013 --- The ArchC Team	
 *														    
 *  This library is free software; you can redistribute    
 *  it and/or modify it under the terms of the GNU         
 *  Lesser General Public                                  
 *  License as published by the Free Software Foundation; 
 *	either version 2.1 of the License, or (at your option) 
 *  any later version.                                     
 *															
 *  This library is distributed in the hope that it        
 *  will be useful, but WITHOUT ANY WARRANTY; 				
 *  without even the implied warranty of				
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  	
 *  See the GNU Lesser General Public License for 			
 *  more details.											
 *															
 **/

#ifndef _LEON_3_CPU_HELPER_H
#define _LEON_3_CPU_HELPER_H

#define DEFAULT_STACK_SIZE (256*1024)//!<Default stack size
#define FLUSH_NOP
//#define PRINT_INSTRUCTION //!< Will print instruction. NOTE: will slow down simulation
#define WITH_SRMMU //!< defines if MMU is attached to processor

#define RAM_START_ADDR 0x40000000    //!<RAM Start address 
#define NWINDOWS 8                   //!<Default Leon register window size

//!Bit manipulation macros
#define bit(x,n) (((x) >> n) & 1)
#define bits(x, n, m) (x << (31 - (n)) >> (31 - (n) + (m)))
#define bitmask(n, m) ((0xffffffff) >> (m) << (31 - (n) + (m)) >> (31 - (n)))

#define clear_bits(x,n,m) ((x) &= ~bitmask((n), (m)))
#define clear_bit(x,n) clear_bits(x,n,n)

#define set_bits(x, n, m) ((x) |= bitmask((n), (m)))
#define set_bit(x, b)  set_bits(x, b, b)

#define sign_extend(n, b) (int) ((n << (32 - b)) >> (32 - b))
#define zero_extend(n) (uint32_t) n

#define sign_ext13(n) sign_extend(n, 13)
#define sign_ext22(n) sign_extend(n, 22)
#define sign_ext7(n) sign_extend(n, 7)
#define sign_extend_byte(n) sign_extend(n, 8)
#define sign_extended_halfword(n) sign_extend(n, 16)

//!PSR Bits macros
#define PSRicc_N bit(PSR.read(), 23)
#define PSRicc_Z bit(PSR.read(), 22)
#define PSRicc_V bit(PSR.read(), 21)
#define PSRicc_C bit(PSR.read(), 20)

#define set_psr_bits(psr, x, n, m) (x == 1) ? PSR.write(set_bits(psr, n, m)) : PSR.write(clear_bits(psr, n, m)) 

#define set_psr_bit(psr, x, b) set_psr_bits(psr, x, b, b)

#define SET_PSRicc_N(x) set_psr_bit(PSR, x, 23)
#define SET_PSRicc_Z(x) set_psr_bit(PSR, x, 22)
#define SET_PSRicc_V(x) set_psr_bit(PSR, x, 21)
#define SET_PSRicc_C(x) set_psr_bit(PSR, x, 20)
#define SET_PSR_S(x)    set_psr_bit(PSR, x, 7)
#define SET_PSR_PS(x)   set_psr_bit(PSR, x, 6)
#define SET_PSR_ET(x)   set_psr_bit(PSR, x, 5)
#define SET_PSR_EF(x)   set_psr_bit(PSR, x, 12)
#define SET_PSR_EC(x)   set_psr_bit(PSR, x, 13)
#define SET_PSR_CWP(x)  PSR.write(PSR.read() & ~0x1f); \
                        PSR.write((PSR.read() | x))

#define PSR_PIL  (bits(PSR.read(), 11, 8))  //Processor Interrupt Level
#define PSR_CWP  (bits(PSR.read(), 4, 0))
#define PSR_S    (bit(PSR.read(), 7))
#define PSR_PS   (bit(PSR.read(), 6))
#define PSR_ET   (bit(PSR.read(), 5))
#define PSR_EF   (bit(PSR.read(), 12))
#define PSR_EC   (bit(PSR.read(), 13))

//!Write/Read Register macros
#define writeReg(n, data) \
			if(n != 0){ \
		  		if(n >= 1 && n<= 7) { \
					REGS[n] = ac_word(data); \
			   } else {  \
					RB[((n-8) + (PSR_CWP*16)) % (16 * NWINDOWS)] = ac_word(data); \
				}	\
			}

#define readReg(n) \
		  		((n >= 0 && n<= 7) ? (int)REGS[n] : (int)RB[((n-8) + (PSR_CWP*16)) % (16 * NWINDOWS)])


#endif //_LEON_3_CPU_HELPER_H
