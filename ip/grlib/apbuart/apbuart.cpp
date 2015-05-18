/**
 * \file      apbuart.h
 * \author    Rogerio Alves Cardoso
 * 
 *            The ArchC Team
 *            http://www.archc.org/
 *
 *            Computer Systems Laboratory (LSC)
 *            IC-UNICAMP
 *            http://www.lsc.ic.unicamp.br/
 *
 * \version   0.2
 * \brief     Implements the Gaisler UART with APB interface (APBUART)
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
*/

#include "apbuart.h" 


namespace grlib {

//Default Constructor
apbuart::apbuart(sc_module_name module_name, 
                 uint32_t socket_port): sc_module( module_name ),
                                        target_export("iport"),
                                        IRQ_port("IRQ_port", 0U),
                                        socket_port(socket_port),
                                        server(socket_port)
{
  //Binding target export
  target_export( *this );

  DataR        = 0x0;
  StatusR      = 0x6; //TX Shift Register and TX FIFO empty
  ControlR     = 0x7; //TX/RX enabled. RX interruption enabled
  ScalerR      = 0x0;

   //Wait for socket connection
  fprintf(stderr, "APBUART: waiting for connection on port ...\n");
  server.accept(plug);
  
  SC_HAS_PROCESS(apbuart);
  SC_THREAD(uart_receive);
}

apbuart::~apbuart(){ }

void apbuart::generate_interrupt()
{
   ac_tlm_req request;
   ac_tlm_rsp response;

   request.type   = WRITE;
   request.data   = 0x2;
   request.dev_id = 0x66;
  //!FIXME:
   request.addr = IRQ_SEND_ADDR;
   response = IRQ_port->transport(request);

  if (response.status == ERROR)
    fprintf(stderr, "TLM Error when trying send a processor interruption\n");
  //return response.status;
}

void apbuart::uart_receive()
{
   for(;;) {

      wait(1, SC_MS); //!Sleep for 1ms

      if(plug.data_ready() && (ControlR & RECEIVER_ENABLE)) {
          string data_rxd;
          plug >> data_rxd;
          //Bufferize the received string
          for ( int i=0; i < data_rxd.length(); i++)
               buffer.push(data_rxd[i]);
          //Write only one char to Holding Register
          DataR = (char) DATA(buffer.front());
          buffer.pop(); //Remove the readed char

          StatusR |= DATA_READY;
          StatusR &= ~TRASMITTER_SHIFT_REGISTER_EMPTY;
          StatusR &= ~TRASMITTER_FIFO_EMPTY;

          if(ControlR & RECEIVER_INTERRUPT_ENABLE)
            generate_interrupt();
      }
   } //for
}

ac_tlm_rsp_status apbuart::uart_transmitter()
{
  StatusR |= DATA_READY;
  StatusR &= ~TRASMITTER_SHIFT_REGISTER_EMPTY;
  StatusR &= ~TRASMITTER_FIFO_EMPTY; 
  
  if(ControlR & TRANSMITTER_ENABLE) {
    string data_txd = "";
    data_txd = (char) DATA(DataR);
    plug << data_txd;

    StatusR &= ~DATA_READY;
    StatusR |= TRASMITTER_SHIFT_REGISTER_EMPTY;
    StatusR |= TRASMITTER_FIFO_EMPTY;
    DataR = 0x0;

    if(ControlR & TRANSMITTER_INTERRUPT_ENABLE)
       generate_interrupt();
  }
  return SUCCESS;
}

ac_tlm_rsp apbuart::transport( const ac_tlm_req &request ) 
{
  ac_tlm_rsp response;

  switch( request.type ) {
    case WRITE:
         response.status = uart_write(request.addr, request.data);
         return response;
    case READ:
         response.status = uart_read(request.addr, response.data);
         return response;
    default:
         response.status = ERROR;
         return response; 
  }
}

ac_tlm_rsp_status apbuart::uart_write(const uint32_t& addr , const uint32_t& data)
{

  uint32_t internal_address = addr & 0xFF;

  switch(internal_address){
   case DATA_OFFSET:     //!0x0
    DataR = DATA(data);
    return uart_transmitter();
   case STATUS_OFFSET:   //!0x4
    return SUCCESS;
   case CONTROL_OFFSET:  //!0x8
    ControlR = data;
    return SUCCESS;
   case SCALER_OFFSET:   //!0xC
    ScalerR = data;
    return SUCCESS;
   default:
    return ERROR;
  }
}

ac_tlm_rsp_status apbuart::uart_read(const uint32_t& addr , uint32_t& data)
{

  uint32_t internal_address = addr & 0xFF;

  switch(internal_address){
   case DATA_OFFSET:     //!0x0
      data = DataR;
      //!if buffer is not empty we read the next char
      if(!buffer.empty())
      {
        DataR = (char) DATA(buffer.front());
        buffer.pop(); //!Remove the readed char
            if(ControlR & RECEIVER_INTERRUPT_ENABLE)
        {
          //!Generate a interruption to processor
          generate_interrupt();
         }
      }else //!buffer is empty
      {
          StatusR &= ~DATA_READY;
          StatusR |= TRASMITTER_SHIFT_REGISTER_EMPTY;
         StatusR |= TRASMITTER_FIFO_EMPTY;  
         DataR = 0x0;
      }
        return SUCCESS;
   case STATUS_OFFSET:   //!0x4
      data = StatusR;
      return SUCCESS;
   case CONTROL_OFFSET:  //!0x8
      data = ControlR;
      return SUCCESS;
   case SCALER_OFFSET:   //!0xC
      data = ScalerR;
      return SUCCESS;
   default:
      return ERROR;
  }
}

} //namespace grlib




