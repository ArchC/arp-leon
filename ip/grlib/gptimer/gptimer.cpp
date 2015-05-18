/**
 * \file      gptimer.cpp
 * \author    Rogerio Alves Cardoso
 *
 *            The ArchC Team
 *            http://www.archc.org/
 *
 *            Computer Systems Laboratory (LSC)
 *            IC-UNICAMP
 *            http://www.lsc.ic.unicamp.br/
 *
 * \version   0.1
 *
 * \brief     Implements the Gaisler Timer Unit (GPTimer)
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
 */

#include "gptimer.h"
 
namespace grlib {

gptimer::gptimer(sc_module_name module_name, uint32_t freq):
    sc_module( module_name ),
    IRQ_port("IRQ_port", 0U),
    target_export("iport") {

  target_export( *this );

  timers = new LEON3_timers[NUM_TIMERS];
  configuration_reg = GPTIMER_CONFIG_REG_PATTERN;
 
  //!50HZ
  prescalar.prescalar_reload = freq - 1;
  prescalar.prescalar_value  = prescalar.prescalar_reload;

  //!Reset all timers
  for (int i = 0; i < NUM_TIMERS; i++){
    timers[i].control = 0x0;
    timers[i].counter = 0x0ffff;
    timers[i].reload  = 0x0ffff;
  }
   //!Sets thread behavior
  SC_HAS_PROCESS(gptimer);
    SC_METHOD(decrement_timer);
      sensitive << clk.pos();
}

//!Destructor
gptimer::~gptimer() { 
  delete[] timers;
  timers = NULL;
}

//!Decrement Scalar clock by 1
void gptimer::decrement_timer() {
  prescalar.prescalar_value -= 1;
    if(prescalar.prescalar_value < 0)  
       timer_tick();
}

//!Once tick is generated all timers are decremented
//!No Chaining mode support yet
inline void gptimer::timer_tick(){

  for( uint32_t id = 0; id < NUM_TIMERS; id++ )
  {
    if(timers[id].control & GPTIMER_ENABLE_MASK)
    {
      timers[id].counter = timers[id].counter - 1;
      if(timers[id].counter < 0)
         gptimer_hit(id);
    }
  }
}

//!GPTIMER underflow
inline void gptimer::gptimer_hit( uint32_t id ) {
   //!Check if interrupt is enabled
  if(timers[id].control & GPTIMER_INT_ENABLE_MASK) {
    timers[id].control |= GPTIMER_INT_PENDING_MASK;
    generate_interrupt(id);
  }
  //!Check if restart counter on underflow 
  if(timers[id].control & GPTIMER_RESTART_MASK)
    timers[id].counter = timers[id].reload;
}

//!IRQ send method implementation
inline void gptimer::generate_interrupt( uint32_t id )
{
  ac_tlm_req request;
  ac_tlm_rsp response;

  request.type = WRITE;
  request.data = (id + BASE_IRQ);
  request.addr = IRQ_SEND_ADDR;
  request.dev_id = 0x77;

  response = IRQ_port->transport(request);
}

//!TLM transport implementation
ac_tlm_rsp gptimer::transport( const ac_tlm_req &request ) {

  ac_tlm_rsp response;

  switch( request.type ) {
    case WRITE:
      response.status = gptimer_write(request.addr, request.data);
      break;
    case READ:
      response.status = gptimer_read(request.addr, response.data);
      break;
    default:
      response.status = ERROR; 
  }
  return response;
}

//!GPTIMER write method implementation
ac_tlm_rsp_status gptimer::gptimer_write(const uint32_t &a, const uint32_t &d){ 
  
  uint32_t internal_address = a & 0xff;
  uint32_t val = d;
  uint32_t id, timer_addr;

  switch(internal_address){
    case 0x0:
      val &= 0xffff;
      prescalar.prescalar_value = val;
      return SUCCESS;
    case 0x4:
      val &= 0xffff;
      prescalar.prescalar_reload = val;
      return SUCCESS;
    case 0x8:
      //!Read Only
      return SUCCESS;
    default:
      break;
  }
  //Thanks QEMU team!
  id = (internal_address - TIMER_BASE) / TIMER_BASE;
  timer_addr = internal_address % TIMER_BASE;

  if( id >= 0 && id < NUM_TIMERS ) {
    switch(timer_addr) {
      case 0x0:
        timers[id].counter = val;
        return SUCCESS;
      case 0x4:
        timers[id].reload = val;
        return SUCCESS;
      case 0x8:
        val &= 0x1f;
        if(val & GPTIMER_LOAD_MASK)
          timers[id].counter = timers[id].reload; //restart timers
          timers[id].control = val;
        return SUCCESS;
      default:
        break;
    }
  }
  return ERROR;
}

//!GPTIMER read method implementation
ac_tlm_rsp_status gptimer::gptimer_read(const uint32_t &a, uint32_t &d){ 

  uint32_t internal_address = a & 0xff;
  uint32_t id, timer_addr;

  switch(internal_address) {
    case 0x0:
      d = prescalar.prescalar_value;
      return SUCCESS; 
    case 0x4:
      d = prescalar.prescalar_reload;
      return SUCCESS; 
    case 0x8:
      d = configuration_reg;
      return SUCCESS; 
    default:
      break;
  }

  id = (internal_address - TIMER_BASE) / TIMER_BASE;
  timer_addr = internal_address % TIMER_BASE;

  if(id >=0 && id < NUM_TIMERS) {
    switch(timer_addr) {
      case 0x0:
        d = timers[id].counter;
        return SUCCESS; 
      case 0x4:
        d = timers[id].reload;
        return SUCCESS; 
      case 0x8:
        d = timers[id].control;
        return SUCCESS; 
      default:
        break;
    }
  }
  return ERROR;
}

} //namespace grlib
