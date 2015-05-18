/**
 * \file     irqmp.cpp
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

#include "irqmp.h"

namespace grlib {
//!Default constructor implementation 
irqmp::irqmp( sc_module_name module_name, uint32_t ncpu ):
    sc_module( module_name ),
    cpu_counter(ncpu),
    target_export("iport")
{
  //!Binding target export
  target_export( *this );
  assert(ncpu > 0 && ncpu <= MAX_CPU);

  //!Create ports to CPU's
  CPU_port    = new sc_port< ac_tlm_transport_if >[ncpu];
  //!IRQMP Registers
  irqmp_regs = new LEON3_irqmp_registers;

  //!IRQMP registers initialization
  irqmp_regs->pending   = 0x0;
  irqmp_regs->level     = 0x0;
  irqmp_regs->clear     = 0x0;
  irqmp_regs->status    = 0x0;
  irqmp_regs->status    |= ((ncpu-1) << 28); //Set number of CPUs in system
  irqmp_regs->status    |= (1 << 27); //Enable broadcast ?
  irqmp_regs->broadcast = 0x0;

  for (int i = 0; i < MAX_CPU; i++)
  {
  irqmp_regs->force[i]       = 0x0;
  irqmp_regs->mask[i]        = 0x0;
  irqmp_regs->extended[i]    = 0x0;
  }

  SC_HAS_PROCESS(irqmp);
    SC_METHOD(interruption_forward);
      sensitive << clk;
}

//!Default destructor
irqmp::~irqmp()
{
   delete irqmp_regs;
   irqmp_regs = NULL;
}

//!IRQMP Interrupt clear method implementation
void irqmp::irqmp_interrupt_clear(uint32_t irl, uint32_t cpu_id)
{
   assert(cpu_id >= 0 && cpu_id < MAX_CPU);
   irqmp_regs->force[cpu_id] &= ~LEON_HARD_INT(irl);
   irqmp_regs->pending       &= ~LEON_HARD_INT(irl);
}

//!IRQMP Interrupt send method implementation
void irqmp::interruption_forward()
{
          
  ac_tlm_req request;
  ac_tlm_rsp response;
  uint32_t pending = 0;
  uint32_t irl = 0;

  //!Check if we have any interruption pending
  for (int i = 0; i < cpu_counter; i++) {
    //!IRQ 15 cannot be masked by processor, most of OS do not
    // safe handling this interrupt. So we force to unmask this
    irqmp_regs->mask[i] |= (1 << 15);

    pending = (irqmp_regs->pending | irqmp_regs->force[i]) & irqmp_regs->mask[i];

    if (pending)
    {
     irl = intr_prioritization(pending, irqmp_regs->level);
     request.type = WRITE;
     request.data = irl;
     response = CPU_port[i]->transport(request);
    }
    pending =  0;
  }
}

//!Prioritize interruptions 
inline uint8_t irqmp::intr_prioritization(uint32_t &pending, uint32_t &level){

  uint8_t i;
  uint32_t level1 = pending & level;

  //!Level 1
  if (level1) {
     for (i = MAX_INTERRUPT; i > 0; i--) 
         if ((LEON_HARD_INT(i) & level1) >> (i))
              return i;
  //!Level 0
  } else {
      for (i = MAX_INTERRUPT; i > 0; i--)
          if ((LEON_HARD_INT(i) & pending) >> (i))
              return i;
  }
}

//!TLM Transport method implementation
ac_tlm_rsp irqmp::transport( const ac_tlm_req &request ) 
{

  ac_tlm_rsp response;

  /*
  We use dev_id field from TLM protocol to identify the device, devices id 0 to 15
  are reserved to identify cpu's. Any other cases are interpreted as a read/write request
  to IRQMP device.
  A request with cpu #dev_id sent a acknowledge signal and we can properly clear a 
  (forced) interruption.
  */

  switch(request.dev_id)
  {
  case 0 ... MAX_CPU-1:
    irqmp_interrupt_clear(request.data, request.dev_id);
    return response;
  default:
    switch( request.type ) {
      case WRITE:
        response.status = irqmp_write(request.addr, request.data);
        return response;
      case READ:
        response.status = irqmp_read(request.addr, response.data);
        return response;
      default:
        response.status = ERROR;
        return response; 
    }
  }
} 

//!IRQMP write method implementation
ac_tlm_rsp_status irqmp::irqmp_write(const uint32_t &addr , const uint32_t &data)
{   
  uint32_t iaddr = addr & 0xFF;
  uint32_t d = data; //!data is a const

  switch(iaddr) {
    case IRQMP_LEVEL_OFFSET:      //!0x0
      d &= 0xFFFE;
      irqmp_regs->level |= LEON_HARD_INT(d);
      return SUCCESS;
    case IRQMP_PENDING_OFFSET:    //!0x4
      irqmp_regs->pending |= LEON_HARD_INT(d);
      return SUCCESS;
    case IRQMP_FORCE0_OFFSET:     //!0x8
      d &= 0xFFFE;
      irqmp_regs->force[0] = LEON_HARD_INT(d);
      return SUCCESS;
    case IRQMP_CLEAR_OFFSET:      //!0xC
      d &= 0xFFFE;
      irqmp_regs->pending &= ~LEON_HARD_INT(d);
      return SUCCESS;   
    case IRQMP_MPSTATUS_OFFSET:   //!0x10
      d &= 0xFFFF;
      irqmp_regs->status &= (~d);
      return SUCCESS;
    case IRQMP_BROADCAST_OFFSET:  //!0x14
      d &= 0xFFFE;
      irqmp_regs->broadcast = d;
      for (int i = 0; i < cpu_counter; i++)
          irqmp_regs->force[i] |= LEON_HARD_INT(d);
      return SUCCESS;     
    default:
      break;
   }

  if(iaddr >= IRQMP_MASK_OFFSET && iaddr < IRQMP_FORCE_OFFSET) {
    int cpu = (iaddr - IRQMP_MASK_OFFSET) / 4;
    assert(cpu >= 0 && cpu < MAX_CPU);
    d &= 0xFFFE;
    irqmp_regs->mask[cpu] = d;
    return SUCCESS;
  }

  if(iaddr >= IRQMP_FORCE_OFFSET && iaddr < IRQMP_EXTENDED_OFFSET) {
    int cpu = (iaddr - IRQMP_FORCE_OFFSET) / 4;
    assert(cpu >= 0 && cpu < MAX_CPU);
    d &= 0xFFFE;
    irqmp_regs->force[cpu] |= LEON_HARD_INT(d);
    return SUCCESS;
  }

  if(iaddr >= IRQMP_EXTENDED_OFFSET && iaddr < 256) {
    //!TODO:Extended interruptions are unsupported
      return ERROR;
  }

  return ERROR;
}

//!IRQMP Read method implementation
ac_tlm_rsp_status irqmp::irqmp_read(const uint32_t &addr , uint32_t &data)
{
   //!Calculates device internal address
  uint32_t iaddr = addr & 0xFF;

  switch(iaddr) {
    case IRQMP_LEVEL_OFFSET:    //!0x0
      data = irqmp_regs->level;
      return SUCCESS;
    case IRQMP_PENDING_OFFSET:  //!0x4
      data = irqmp_regs->pending;
      return SUCCESS;
    case IRQMP_FORCE0_OFFSET:   //!0x8
      //!Alias to CPU #0 Force register
      data = irqmp_regs->force[0];
      return SUCCESS;
    case IRQMP_CLEAR_OFFSET:    //!0xC
      //!Always read as 0x0
      data = 0x0;   
      return SUCCESS;
    case IRQMP_MPSTATUS_OFFSET: //!0x10
      data = irqmp_regs->status;
      return SUCCESS;
    case IRQMP_BROADCAST_OFFSET://!0x14
      data = irqmp_regs->broadcast;      
      return SUCCESS; 
    default:
      break;
   }

  if(iaddr >= IRQMP_MASK_OFFSET && iaddr < IRQMP_FORCE_OFFSET){
    int cpu = (iaddr - IRQMP_MASK_OFFSET) / 4;
    assert(cpu >= 0 && cpu < MAX_CPU);
    data = irqmp_regs->mask[cpu];
    return SUCCESS;
  }

  if(iaddr >= IRQMP_FORCE_OFFSET && iaddr < IRQMP_EXTENDED_OFFSET) {
    int cpu = (iaddr - IRQMP_FORCE_OFFSET) / 4;
    assert(cpu >= 0 && cpu < MAX_CPU);
    data = irqmp_regs->force[cpu];
    return SUCCESS;
  }

  if(iaddr >= IRQMP_EXTENDED_OFFSET && iaddr < 256) {
    //!TODO:Extended interruptions are unsupported
    return ERROR;
  }

  return ERROR;
}

} //namespace grlib