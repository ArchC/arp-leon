/**
 * \file      router.h
 * \author    Rogerio Alves
 * 
 * \author    The ArchC Team
 *            http://www.archc.org/
 *
 *            Computer Systems Laboratory (LSC)
 *            IC-UNICAMP
 *            http://www.lsc.ic.unicamp.br/
 *
 * \version   0.1
 * 
 * \brief     Defines an ac_tlm router that works like a simplyfied
 *            bus structure.
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


#ifndef _BUS_GENERIC_ROUTER_
#define _BUS_GENERIC_ROUTER_

#include <systemc>
#include "ac_tlm_protocol.H"
#include "ac_tlm_port.H"
#include "bitutils.h"

#define PROM_BASEADDR    0x00000000
#define PROM_ENDADDR     0x3FFFFFFF 
#define RAM_BASEADDR     0x40000000
#define RAM_ENDADDR      0x7FFFFFFF
#define APBUART_BASEADDR 0x80000100
#define APBUART_ENDADDR  0x800001FF 
#define IRQMP_BASEADDR   0x80000200
#define IRQMP_ENDADDR    0x800002FF 
#define GPTIMER_BASEADDR 0x80000300
#define GPTIMER_ENDADDR  0x800003FF 
#define APBROM_BASEADDR  0x800FF000
#define APBROM_ENDADDR   0x800FFFFF 
#define AHBROM_BASEADDR  0xFFFFF000
#define AHBROM_ENDADDR   0xFFFFFFFF 

using tlm::tlm_transport_if;

/*Namespace to isolate router from ArchC*/
namespace bus {
namespace generic {

class router :
  public sc_module,
  public ac_tlm_transport_if // Using ArchC TLM protocol
{
public:

  /* Exposed port with ArchC interface */
  sc_export< ac_tlm_transport_if > target_export;
  ac_tlm_port MEM_port;
  ac_tlm_port UART_port;
  ac_tlm_port GPTIMER_port;
  ac_tlm_port IRQMP_port;
  ac_tlm_port PROM_port;
  ac_tlm_port AHBROM_port;
  ac_tlm_port APBROM_port;

  /**
   * Implementation of TLM transport method that
   * handle packets of the protocol doing apropriate actions.
   * This method must be implemented (required by SystemC TLM).
   * \param request is a received request packet
   * \return A response packet to be send
  */
  ac_tlm_rsp transport( const ac_tlm_req &request );


  /** 
   * Default constructor.
   */
  router( sc_module_name module_name );

  /** 
   * Default destructor.
   */
  ~router();

};

} //namespace generic
} //namespace bus

#endif //_BUS_GENERIC_ROUTER_
