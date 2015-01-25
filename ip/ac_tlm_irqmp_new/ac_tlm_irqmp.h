/**
 * @file      ac_tlm_irqmp.cpp
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
 * @brief     Implements the Gaisler Interrupt Controller
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
 *   FIXME: this is a simple version of the IRQ Controller we wont support
 *          multiprocessors for now
**/

#ifndef AC_TLM_IRQMP_H
#define AC_TLM_IRQMP_H

#include <systemc.h>
#include <stdio.h>
#include "ac_tlm_protocol.H"
#include "ac_tlm_port.H"

#define INTERRUPT_LEVEL_OFFSET      0x0
#define INTERRUPT_PENDING_OFFSET    0x4
#define INTERRUPT_FORCE0_OFFSET     0x8
#define INTERRUPT_CLEAR_OFFSET      0xC
#define INTERRUPT_MPSTATUS_OFFSET   0x10
#define INTERRUPT_BROADCAST_OFFSET  0x14
#define INTERRUPT_MASK_OFFSET       0x40
#define LEON_HARD_INT(x)           (1 << (x))
#define MAX_INTERRUPT 15
#define MAX_CPU 16

//#define DEBUG_MODEL

#ifdef DEBUG_MODEL
#  define dbg_message(args...) fprintf (stderr, "IRQMP DEBUG: " args)
#  define err_message(args...) fprintf (stderr, "IRQMP ERROR: " args)
#  define _d_(arg) arg
#else
#  define dbg_message(args...)
#  define err_message(args...) fprintf (stderr, "IRQMP ERROR: " args)
#  define _d_(arg)
#endif

//!LEON3 IRQMP Controller Registers
typedef struct{

  uint32_t level;              //!Interrupt Level Register 
  uint32_t pending;            //!Interrupt Pending Register
  uint32_t clear;              //!Interrupt Clear Register
  uint32_t broadcast;          //!Interrupt Broadcast register
  uint32_t status;             //!Processor Status
  uint32_t force[MAX_CPU];     //!Interrupt Force Register Bank
  uint32_t mask[MAX_CPU];      //!Interrupt Mask Register Bank
  uint32_t extended[MAX_CPU];  //!Extended Interruptions Register Bank
}LEON3_irqmp_registers;


using tlm::tlm_transport_if;

namespace user
{
  class ac_tlm_irqmp : 
  public sc_module,
  public ac_tlm_transport_if
 {
  private:

   //!IRQMP Registers
   LEON3_irqmp_registers    *irqmp;

   //!IRQMP CPU counter
   uint32_t CPU_counter;
   
   /**
   * Interrupt Prioritization method
   * Select a interruption to be forward to processor based
   * on the interruption level (1 or 0) and the interruption priority (15 to 1)
   * @param unsigned value of the pending register
   * @param unsigned value of the level register 
   * @return the selected interruption level that will be forward to processor
   **/
   inline uint8_t intr_prioritization(unsigned &, unsigned &);

   /**
   * Interrupt send method
   * Send a selected interruption to processor n
   **/
   void irqmp_interrupt_send();

  public:

   //!IRQMP interface
   sc_export< ac_tlm_transport_if > target_export;
  
   //!CPU port interface
   sc_port< ac_tlm_transport_if > *CPU_port;

   sc_in<bool> clk;

   /**
   * Default constructor
   * @param a string with the module name
   * @param a unsigned integer with the number of cpu, default = 1 cpu (single core)
   **/
   ac_tlm_irqmp(sc_module_name module_name, uint32_t ncpu = 1);

   /**
   * Default destructor
   **/
   ~ac_tlm_irqmp();
   
   /**
   * IRQMP interrupt clear method
   * When the processor acknownledge a interruption, a signal is sent to IRQMP
   * and this method is activated cleaning the pending interruption
	* @param  a unsigned value with the acknoledge interruption to be cleaned
   **/
   void irqmp_interrupt_clear(uint32_t irl);

   /**
   * IRQMP write method
	* @param   a const unsigned value correponding to the register address
	* @param   a const unsigned value correponding to the data to be write
	* @return  a TLM response status SUCCESS or ERROR
   **/
   ac_tlm_rsp_status irqmp_write(const uint32_t & , const uint32_t &);

   /**
   * IRQMP read method
	* @param   a const unsigned value correponding to the register address
	* @param   a const unsigned value correponding to the readed data
	* @return  a TLM response status SUCCESS or ERROR
   **/
   ac_tlm_rsp_status irqmp_read(const uint32_t & , uint32_t &); 

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
#endif /*AC_TLM_IRQMP_H*/
