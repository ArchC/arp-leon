/**
 * @file      ac_tlm_gptimer.h
 * @author    Rogerio Alves Cardoso
 *
 * @author    The ArchC Team
 *            http://www.archc.org/
 *
 *            Computer Systems Laboratory (LSC)
 *            IC-UNICAMP
 *            http://www.lsc.ic.unicamp.br/
 *
 * @version   0.1
 * @date      
 *
 * @brief     Implements the Gaisler Timer Unit (GPTimer)
 *
 * @attention Copyright (C) 2002-2005 --- The ArchC Team
 *
 *   This library is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU Lesser General Public
 *   License as published by the Free Software Foundation; either
 *   version 2.1 of the License, or (at your option) any later version.
 *
 *   This library is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *   Lesser General Public License for more details.
 *
 *
 */

#ifndef AC_TLM_GPTIMER_H
#define AC_TLM_GPTIMER_H

#include <systemc.h>
#include <string>
#include <stdio.h>
#include "ac_tlm_protocol.H"
#include "ac_tlm_port.H"

//!Number of timers
#define NUM_TIMERS 4

//!Timer Configuration Register Initial values
#define GPTIMER_CONFIG_REG_PATTERN  0x342
//!Control Register Initial Values
#define GPTIMER_CONTROL_REG_PATTERN 0x3 

//!FIXME: pass this as argument to constructor
#define BASE_IRQ 0x8
#define IRQ_SEND_ADDR 0x80000204

//!Control Register masks
#define GPTIMER_ENABLE_MASK      (1 << 0)
#define GPTIMER_RESTART_MASK     (1 << 1)
#define GPTIMER_LOAD_MASK        (1 << 2)
#define GPTIMER_INT_ENABLE_MASK  (1 << 3)
#define GPTIMER_INT_PENDING_MASK (1 << 4)
#define GPTIMER_CHAIN_MASK       (1 << 5) /* Not supported */
#define GPTIMER_DEBUG_HALT_MASK  (1 << 6) /* Not supported */

#define TIMER_BASE 0x10

//!Decrement Unit
typedef struct{
   int prescalar_value;
   unsigned int prescalar_reload;
}LEON3_prescalar_regs;

//!Timer Unit
typedef struct{
   int counter;         //timer value;
   unsigned int reload; //timer reload;
   unsigned int control;//timer control
}LEON3_timers;

using tlm::tlm_transport_if;

/*isolates from ArchC namespace*/
namespace user
{
  class ac_tlm_gptimer : 
  public sc_module,
  public ac_tlm_transport_if
 {
  private:

		/**
      * decrement_timer
      * Decrements the prescalar register by one on each clock cycle
      * SystemC method
      **/
  		void decrement_timer();

		/**
      * timer_tick
      * Generates a tick every time the prescalar register underflows
      **/
  		inline void timer_tick();
  
		//!GPTIMER registers
  		LEON3_timers *timers;
  		LEON3_prescalar_regs prescalar;
  		uint32_t configuration_reg;

  public:
   
      //!Clock in port
      sc_in<bool> clk;
      //!TLM Port
      sc_export< ac_tlm_transport_if > target_export;
      //!IRQ TLM port 
      ac_tlm_port IRQ_port;

      /**
      * Default constructor
      * @param a string with the module name
      * @param a integer with the clock frequency
      **/
      ac_tlm_gptimer(sc_module_name module_name, uint32_t freq);

		/**
      * Default destructor
      **/
    	~ac_tlm_gptimer();

      /**
      * GPTIMER write method
	   * @param   a const unsigned value correponding to the register address
	   * @param   a const unsigned value correponding to the data to be write
	   * @return  a TLM response status SUCCESS or ERROR
      **/
      ac_tlm_rsp_status gptimer_write(const uint32_t & , const uint32_t &);

      /**
      * GPTIMER read method
	   * @param   a const unsigned value correponding to the register address
	   * @param   a const unsigned value correponding to the readed data
	   * @return  a TLM response status SUCCESS or ERROR
      **/
  	   ac_tlm_rsp_status gptimer_read(const uint32_t & , uint32_t &); 

      /**
      * Interrupt send method
      * Send a selected interruption to IRQ Controller
      * @param id of timer who generate the interruption
      **/
   	inline void ac_tlm_irq_send(uint32_t id); 

      /**
      * GPTIMER hit method
      * selects what to do when one timer underflows
      * @param id of timer who underflow
      **/
   	inline void gptimer_hit(uint32_t id);
		/**
		* Implementation of TLM transport method that
		* handle packets of the protocol doing apropriate actions.
		* This method must be implemented (required by SystemC TLM).
		* @param request is a received request packet
		* @return A response packet to be send
		*/
		ac_tlm_rsp transport( const ac_tlm_req &request );
   
 };
};
#endif /*AC_TLM_GPTIMER_H*/
