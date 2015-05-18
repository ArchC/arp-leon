/**
 * \file     irqmp.h
 * \author   Rogerio Alves Cardoso
 *
 *           The ArchC Team
 *           http://www.archc.org/
 *
 *           Computer Systems Laboratory (LSC)
 *           IC-UNICAMP
 *           http://www.lsc.ic.unicamp.br/
 *   
 *
 * \brief     Implements the Grlib IRQMP Multiprocessor Interrupt Controller
 *
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
**/

#ifndef irqmp_H
#define irqmp_H

#include <systemc.h>
#include <stdio.h>
#include <assert.h>
#include "ac_tlm_protocol.H"
#include "ac_tlm_port.H"

#define IRQMP_LEVEL_OFFSET      0x0
#define IRQMP_PENDING_OFFSET    0x4
#define IRQMP_FORCE0_OFFSET     0x8
#define IRQMP_CLEAR_OFFSET      0xC
#define IRQMP_MPSTATUS_OFFSET   0x10
#define IRQMP_BROADCAST_OFFSET  0x14
#define IRQMP_MASK_OFFSET       0x40
#define IRQMP_FORCE_OFFSET      0x80
#define IRQMP_EXTENDED_OFFSET   0xC0

#define LEON_HARD_INT(x)        (1 << (x))
#define MAX_INTERRUPT 15
#define MAX_CPU 16

//!Leon3 IRQ Controller Registers
typedef struct{
  uint32_t level;              //!<Interrupt Level Register 
  uint32_t pending;            //!<Interrupt Pending Register
  uint32_t clear;              //!<Interrupt Clear Register
  uint32_t broadcast;          //!<Interrupt Broadcast register
  uint32_t status;             //!<Processor Status
  uint32_t force[MAX_CPU];     //!<Interrupt Force Register Bank
  uint32_t mask[MAX_CPU];      //!<Interrupt Mask Register Bank
  uint32_t extended[MAX_CPU];  //!<Extended Interruptions Register Bank
} LEON3_irqmp_registers;

using tlm::tlm_transport_if;

namespace grlib
{
class irqmp : public sc_module,
                     public ac_tlm_transport_if {
private:
 
  LEON3_irqmp_registers *irqmp_regs; //!<IRQMP Internal Registers
  uint32_t cpu_counter;

  /**
  * \brief Interrupt Prioritization method
  * Select a interruption to be forward to processor based
  * on the interruption level (1 or 0) and the interruption priority (15 to 1)
  * 
  * \param unsigned value of the pending register
  * \param unsigned value of the level register 
  * 
  * \return the selected interruption level that will be forward to processor
  **/
  inline uint8_t intr_prioritization(unsigned &, unsigned &);

  /**
  * Interrupt send method
  * Send a selected interruption to processor n
  **/
  void interruption_forward();

  /**
  * \brief IRQMP interrupt clear method
  * When the processor acknownledge a interruption, a signal is sent to IRQMP
  * and this method is activated cleaning the pending interruption
  * 
  * \param  a unsigned value with the acknoledge interruption to be cleaned
  **/
  void irqmp_interrupt_clear(uint32_t irl, uint32_t ncpu = 0);


public:

  sc_export< ac_tlm_transport_if > target_export; //!< IRQMP external interface
  sc_port< ac_tlm_transport_if > *CPU_port; //!<CPU port interface
  sc_in<bool> clk; //!< system clock

  /**
  * \brief Default constructor
  * 
  * \param a string with the module name
  * \param number of cpus attached to module
  */
  irqmp(sc_module_name module_name, uint32_t ncpu = 1);

  /**
  * \brief Default destructor
  */
  ~irqmp();

  /**
  * \brief  IRQMP write method
  * \param   a const unsigned value correponding to the register address
  * \param   a const unsigned value correponding to the data to be write
  * 
  * \return  a TLM response status SUCCESS or ERROR
  **/
  ac_tlm_rsp_status irqmp_write(const uint32_t & , const uint32_t &);

  /**
  * \brief  IRQMP read method
  * \param   a const unsigned value correponding to the register address
  * \param   a const unsigned value correponding to the readed data
  * 
  * \return  a TLM response status SUCCESS or ERROR
  **/
  ac_tlm_rsp_status irqmp_read(const uint32_t & , uint32_t &); 

  /**
  * \brief Implementation of TLM transport method that
  * handle packets of the protocol doing apropriate actions.
  * This method must be implemented (required by SystemC TLM).
  * 
  * \param request is a received request packet
  * 
  * \return A response packet to be send
  */
  ac_tlm_rsp transport( const ac_tlm_req &request );  
  };
}; //namespace grlib

#endif /*irqmp_H*/
