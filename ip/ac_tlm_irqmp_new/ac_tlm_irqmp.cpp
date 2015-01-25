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
**/

#include "ac_tlm_irqmp.h"
 
using user::ac_tlm_irqmp;

//!Default constructor implementation 
ac_tlm_irqmp::ac_tlm_irqmp( sc_module_name module_name, uint32_t ncpu ):
    sc_module( module_name ),
    target_export("iport")
{
	//!Binding target export
	target_export( *this );

	//!Create ports to CPU's
   CPU_port = new sc_port< ac_tlm_transport_if >[ncpu];

   //!CPU counter init
   CPU_counter = ncpu;

   //!IRQMP Registers
	irqmp    = new LEON3_irqmp_registers;

   //!IRQMP registers initialization
   irqmp->pending   = 0x0;
   irqmp->level     = 0x0;
   irqmp->clear     = 0x0;
   irqmp->broadcast = 0x0;

   for (int i = 0; i < MAX_CPU; i++)
   {
   	irqmp->force[i]       = 0x0;
   	irqmp->mask[i]        = 0x0;
   	irqmp->extended[i]    = 0x0;
   }

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

   /*
   for(int i = 0; i < CPU_counter; i++)
   {
     pending = (irqmp->pending | irqmp->force);
     pending &= ((1 << 15) | irqmp->mask[i]);
     if(pending != 0)
     {
        irl = intr_prioritization(pending, irqmp->level);
        if (irl)
        {
		    request.type = WRITE;
   	    request.data = irl;
   	    response = CPU_port[i]->transport(request);

        }
     }     
   }


   */
}

//!Prioritize interruptions 
inline uint8_t ac_tlm_irqmp::intr_prioritization(uint32_t &pending, uint32_t &level)
{
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

	switch(internal_address)
   {
		case INTERRUPT_LEVEL_OFFSET:
			  d &= 0xFFFF << 1;
			  irqmp->level = d;
			  dbg_message("Level register write <- %x\n", irqmp->level);
			  return SUCCESS;
		case INTERRUPT_PENDING_OFFSET:
			  irqmp->pending = irqmp->pending | LEON_HARD_INT(d);
			  dbg_message("Pending register write <- %x\n", irqmp->pending);
			  return SUCCESS;
		case INTERRUPT_FORCE_0_OFFSET:  //!0x8 alias to CPU #0 force register
			  d &= 0xFFFF << 1;
			  irqmp->force[0] = d;
			  dbg_message("Force register write <- %x\n", irqmp->force);
			  return SUCCESS;
		case INTERRUPT_CLEAR_OFFSET:
			  d &= 0xFFFF << 1;
			  irqmp->clear =  d;
			  dbg_message("Clear register write <- %x\n", d);
			  irqmp_interrupt_clear(d);
			  return SUCCESS;		
		case INTERRUPT_MPSTATUS_OFFSET:
			  dbg_message("MPStatus register write\n");
			  d &= 0xF80FFFFF;	
			  irqmp->status = d;
			  return SUCCESS;
		case INTERRUPT_BROADCAST_OFFSET:/*Broadcast*/
			  d &= 0xFFFF << 1;
			  dbg_message("Broadcast register write\n");
			  irqmp->broadcast = d;
			  //!write force register for all avaliable CPU's
			  for (int i = 0; i < CPU_counter; i++)
					 irqmp->force[i] = intr_prioritization(irqmp->broadcast, irqmp->level);
		
           return SUCCESS;	
		//case INTERRUPT_MASK_OFFSET: //TODO: Support to SMP
		//	  d &= 0xFFFF << 1;
		//	  irqmp_rb[0].mask = d;
		//	  dbg_message("Processor mask register write <- %x\n", irqmp_rb[0].mask);
		//	  return SUCCESS;		 
		default:
			//return ERROR;
         break;
    }

    if(internal_address >= INTERRUPT_MASK_OFFSET && internal_address < INTERRUPT_FORCE_OFFSET)
    {
		 int cpu = (internal_address - INTERRUPT_MASK_OFFSET) / 4;
		 d &= 0xFFFF << 1;
		 irqmp->mask[cpu] = d;
		 dbg_message("Processor #%d mask register write <- %x\n", cpu, irqmp->mask[cpu]);
       return SUCCESS;
    }

    if(internal_address >= INTERRUPT_FORCE_OFFSET && internal_address < INTERRUPT_EXT_OFFSET)
    {
		 int cpu = (internal_address - INTERRUPT_FORCE_OFFSET) / 4;
		 d &= 0xFFFF << 1;
		 irqmp->force[cpu] = d;
		 dbg_message("Processor #%d force register write <- %x\n", cpu, irqmp->force[cpu]);
       return SUCCESS;
    }

    if(internal_address >= INTERRUPT_EXT_OFFSET)
    {
       //!Extended Interrupts are not supported
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
		case INTERRUPT_FORCE_0_OFFSET:   //!0x8 alias to CPU #0 force register
			  data = irqmp->force[0];
			  dbg_message("Force register read -> %x\n", data);
			  return SUCCESS;
		case INTERRUPT_CLEAR_OFFSET:    //!0xC
			  data = irqmp->clear;
			  dbg_message("Clear register read -> %x\n", irqmp->clear);
			  return SUCCESS;
		case INTERRUPT_MPSTATUS_OFFSET: //!0x10
           //TODO:
 			  //data = 0x0;
			  // dbg_message("MP Status register read (unsupported)\n");	
			  return SUCCESS;
		case INTERRUPT_BROADCAST_OFFSET://!0x14
			  //TODO:
			  //data = 0x0;			
			  //dbg_message("Broadcast register read (unsupported)\n");	
			  return SUCCESS;	
		//case INTERRUPT_MASK_OFFSET:     //!0x40
           //FIXME: Make this for multiprocessors
			  //data = irqmp_rb[0].mask;
			  //dbg_message("Processor mask register read -> %x\n", data);
			  //return SUCCESS;
		default:
			  //err_message("Unknown offset error (read)\n");
			  //return ERROR;
           break;
    }

    if(internal_address >= INTERRUPT_MASK_OFFSET && internal_address < INTERRUPT_FORCE_OFFSET)
    {
		 int cpu = (internal_address - INTERRUPT_MASK_OFFSET) / 4;
       return SUCCESS;
    }

    if(internal_address >= INTERRUPT_FORCE_OFFSET && internal_address < INTERRUPT_EXT_OFFSET)
    {
		 int cpu = (internal_address - INTERRUPT_FORCE_OFFSET) / 4;
       return SUCCESS;
    }

    if(internal_address >= INTERRUPT_EXT_OFFSET)
    {
       //!Extended Interrupts are not supported
       return ERROR;
    }
}
