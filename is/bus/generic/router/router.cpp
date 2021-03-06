/**
 * \file      router.cpp
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
 * \brief     Implements a ac_tlm router that works like a simplyfied
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
 * 
 */

#include "router.h"


namespace bus {
namespace generic {

router::router( sc_module_name module_name ) :  sc_module( module_name ),
                                                MEM_port("MEM_port", 33554432U),
                                                UART_port("UART_port", 0U),
                                                GPTIMER_port("GPTIMER_port", 0U),
                                                IRQMP_port("IRQMP_port", 0U),
                                                PROM_port("PROM_port",1048576U),
                                                AHBROM_port("AHBROM_port",4096U),
                                                APBROM_port("APBROM_port",4096U),
                                                target_export("iport") {
  target_export( *this );
}

router::~router() { }

ac_tlm_rsp router::transport( const ac_tlm_req &request ) {

  ac_tlm_rsp response;
  ac_tlm_req u_request;

  if(request.addr >= RAM_BASEADDR && request.addr < RAM_ENDADDR){

    u_request.data = request.data;
    u_request.type = request.type;
    u_request.dev_id = request.dev_id;
    u_request.addr = request.addr - RAM_BASEADDR;/*Memory access addr - offset*/
    response = MEM_port->transport(u_request);

  }else if(request.addr >= PROM_BASEADDR && request.addr <= PROM_ENDADDR){

    u_request.data   = request.data;
    u_request.type   = request.type;
    u_request.dev_id = request.dev_id;
    u_request.addr   = request.addr;
    response.data = 0x0;

  }else if(request.addr >= APBUART_BASEADDR && request.addr < APBUART_ENDADDR){

    u_request.data = request.data;
    endian_swap32(u_request.data);
    u_request.type = request.type;
    u_request.dev_id = request.dev_id;
    u_request.addr = request.addr;
    response = UART_port->transport(u_request);
    endian_swap32(response.data);

  }else if (request.addr >= IRQMP_BASEADDR && request.addr < IRQMP_ENDADDR){

    u_request.data = request.data;
    endian_swap32(u_request.data);
    u_request.type = request.type;
    u_request.dev_id = request.dev_id;
    u_request.dev_id =+ 20;
    u_request.addr = request.addr;
    response = IRQMP_port->transport(u_request);
    endian_swap32(response.data);

  }else if (request.addr >= GPTIMER_BASEADDR && request.addr < GPTIMER_ENDADDR){

    u_request.data = request.data;
    endian_swap32(u_request.data);
    u_request.type = request.type;
    u_request.dev_id = request.dev_id;
    u_request.addr = request.addr;
    response = GPTIMER_port->transport(u_request);
    endian_swap32(response.data);

  }else if (request.addr >= APBROM_BASEADDR && request.addr < APBROM_ENDADDR){

    u_request.data = request.data;
    endian_swap32(u_request.data);
    u_request.type = request.type;
    u_request.dev_id = request.dev_id;
    u_request.addr = request.addr - APBROM_BASEADDR;/*Memory access addr - offset*/
    response = APBROM_port->transport(u_request);
    endian_swap32(response.data);

  }else if (request.addr >= AHBROM_BASEADDR && request.addr < AHBROM_ENDADDR){
    u_request.data = request.data;
    endian_swap32(u_request.data);//reverse endian
    u_request.type = request.type;
    u_request.dev_id = request.dev_id;
    u_request.addr = request.addr - AHBROM_BASEADDR;/*Memory access addr - offset*/
    response = AHBROM_port->transport(u_request);
    endian_swap32(response.data);
  }else{
    cout << "ERROR::The requested address "  << hex << request.addr << " is beyond of address map" << endl;
    exit(1);
  }

  return response;
}

} //namespace generic
} //namespace bus