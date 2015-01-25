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

#include "ac_tlm_irqmp.h"
 
using user::ac_tlm_irqmp;

//!Default constructor implementation 
ac_tlm_irqmp::ac_tlm_irqmp( sc_module_name module_name ):
    sc_module( module_name ),
    target_export("iport")
{
	//!Binding target export
	target_export( *this );
   //!IRQMP Registers

	//!FIXME: Cannot be synthetized
	irqmp = new LEON3_irqmp_registers;
   irqmp->pending = 0;
   irqmp->level   = 0;
   irqmp->force   = 0;
   irqmp->mask[0] = 0;

	SC_HAS_PROCESS(ac_tlm_irqmp);
   SC_METHOD(irqmp_interrupt_send);
     sensitive << clk;
}

//!Default destructor
ac_tlm_irqmp::~ac_tlm_irqmp()
{
   delete irqmp;
   irqmp = NULL;
}

//!IRQMP Interrupt clear method implementation
void ac_tlm_irqmp::irqmp_interrupt_clear(uint32_t irl)
{
   if ((LEON_HARD_INT(irl) & irqmp->force) >> (irl))
        irqmp->force   &= ~LEON_HARD_INT(irl);
   else
        irqmp->pending &= ~LEON_HARD_INT(irl);
}

//!IRQMP Interrupt send method implementation
void ac_tlm_irqmp::irqmp_interrupt_send()
{
 	        
	ac_tlm_req request;
	ac_tlm_rsp response;
	uint32_t pending = 0;
   uint32_t irl = 0;

   //!Check if we have any interruption pending
   //!NOTE: Interruption 15 cannot be masked by processor, most of OS do not
   //       safe handling this interrupt. So we force to unmask this
	pending = (irqmp->pending | irqmp->force) & ((1 << 15) | irqmp->mask[0]);

   //!Prioritize any pending interruption
   if(pending != 0)
      irl = intr_prioritization(pending, irqmp->level);
   else
      return;
     
   //!Send interruption to processor n
   if(irl != 0)
   {
   	dbg_message("Sent interruption %x to processor 0\n", irl);
		request.type = WRITE;
   	request.data = irl;
   	response = CPU_port[0]->transport(request);

      if (response.status == ERROR)
          err_message("TLM Transport error while sent interruption to processor 0\n");
   }
}

//!Prioritize interruptions 
inline uint8_t ac_tlm_irqmp::intr_prioritization(uint32_t &pending, uint32_t &level){

   uint8_t i;
   uint32_t priority = pending & level;

   //!Level 1
   if (priority) {
       for (i = MAX_INTERRUPT; i > 0; i--) 
           if ((LEON_HARD_INT(i) & priority) >> (i))
                return i;
    //!Level 0
    } else {
        for (i = MAX_INTERRUPT; i > 0; i--)
            if ((LEON_HARD_INT(i) & pending) >> (i))
                return i;
    }
}

//!TLM Transport method implementation
ac_tlm_rsp ac_tlm_irqmp::transport( const ac_tlm_req &request ) 
{

	ac_tlm_rsp response;

   switch( request.type ) {
     case WRITE:
          response.status = irqmp_write(request.addr, request.data);
          return response;
     case READ:
          response.status = irqmp_read(request.addr, response.data);
          return response;
     default:
			 err_message("TLM transport error\n");
          response.status = ERROR;
			 return response; 
     }
} 

//!IRQMP write method implementation
ac_tlm_rsp_status ac_tlm_irqmp::irqmp_write(const uint32_t &addr , const uint32_t &data)
{ 
	
	uint32_t internal_address = addr & 0xFF;
   uint32_t d = data; //!

	switch(internal_address){
		case INTERRUPT_LEVEL_OFFSET:
			  d &= 0xFFFF << 1;
			  irqmp->level = d;
			  dbg_message("Level register write <- %x\n", irqmp->level);
			  return SUCCESS;
		case INTERRUPT_PENDING_OFFSET:
			  irqmp->pending = irqmp->pending | LEON_HARD_INT(d);
			  dbg_message("Pending register write <- %x\n", irqmp->pending);
			  return SUCCESS;
		case INTERRUPT_FORCE_OFFSET:
			  d &= 0xFFFF << 1;
			  irqmp->force = d;
			  dbg_message("Force register write <- %x\n", irqmp->force);
			  return SUCCESS;
		case INTERRUPT_CLEAR_OFFSET:
			  d &= 0xFFFF << 1;
			  irqmp->clear =  d;
			  dbg_message("Clear register write <- %x\n", d);
			  irqmp_interrupt_clear(d);
			  return SUCCESS;		
		case INTERRUPT_MPSTATUS_OFFSET:
           //!Unsupported
			  dbg_message("MPStatus register write (unsupported)\n");
			  return SUCCESS;
		case INTERRUPT_BROADCAST_OFFSET:/*Broadcast*/
			  //!Unsupported
			  dbg_message("Broadcast register write (unsupported)\n");
           return SUCCESS;	
		case INTERRUPT_MASK_OFFSET:
			  d &= 0xFFFF << 1;
			  irqmp->mask[0] = d;
			  dbg_message("Processor mask register write <- %x\n", irqmp->mask[0]);
			  return SUCCESS;		 
		default:
			return ERROR;
        } 
}

//!IRQMP Read method implementation
ac_tlm_rsp_status ac_tlm_irqmp::irqmp_read(const uint32_t &addr , uint32_t &data)
{
 
	uint32_t internal_address = addr & 0xFF;

   switch(internal_address)
   {
		case INTERRUPT_LEVEL_OFFSET:    //!0x0
			  data = irqmp->level;
			  dbg_message("Level register read -> %x\n", data);	
			  return SUCCESS;
		case INTERRUPT_PENDING_OFFSET:  //!0x4
			  data = irqmp->pending;
			  dbg_message("Pending register read -> %x\n", data);
			  return SUCCESS;
		case INTERRUPT_FORCE_OFFSET:    //!0x8
			  data = irqmp->force;
			  dbg_message("Force register read -> %x\n", data);
			  return SUCCESS;
		case INTERRUPT_CLEAR_OFFSET:    //!0xC
			  data = irqmp->clear;
			  dbg_message("Clear register read -> %x\n", irqmp->clear);
			  return SUCCESS;
		case INTERRUPT_MPSTATUS_OFFSET: //!0x10
           //TODO:
 			  data = 0x0;
			  dbg_message("MP Status register read (unsupported)\n");	
			  return SUCCESS;
		case INTERRUPT_BROADCAST_OFFSET://!0x14
			  //TODO:
			  data = 0x0;			
			  dbg_message("Broadcast register read (unsupported)\n");	
			  return SUCCESS;	
		case INTERRUPT_MASK_OFFSET:     //!0x40
           //FIXME: Make this for multiprocessors
			  data = irqmp->mask[0];
			  dbg_message("Processor mask register read -> %x\n", data);
			  return SUCCESS;
		default:
			  err_message("Unknown offset error (read)\n");
			  return ERROR;
   }
}
