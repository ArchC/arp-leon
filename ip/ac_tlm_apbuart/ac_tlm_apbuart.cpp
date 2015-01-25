/**
 * @file      ac_tlm_apbuart.h
 * @author    Rogerio Alves Cardoso
 *
 * @author    The ArchC Team
 *            http://www.archc.org/
 *
 *            Computer Systems Laboratory (LSC)
 *            IC-UNICAMP
 *            http://www.lsc.ic.unicamp.br/
 *
 * @version   0.2
 * @brief     Implementation file. 
 *            Implements the Gaisler UART with APB interface IP (APBUART)
 *            
 *
 * @attention Copyright (C) 2002-2014 --- The ArchC Team
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

#include "ac_tlm_apbuart.h" 

using user::ac_tlm_apbuart;

//Default Constructor
ac_tlm_apbuart::ac_tlm_apbuart(sc_module_name module_name, uint32_t socket_port):
    sc_module( module_name ),
    target_export("iport"),
    IRQ_port("IRQ_port", 0U),
    socket_port(socket_port),
    server(socket_port)
{
	//!Binding target export to the uart
   target_export( *this );

   //!UART Registers initialization
	DataR        = 0x0;
	StatusR      = 0x6;//!Transmitter shift register empty and Transmitter FIFO empty
	ControlR     = 0x7;//!Transmitter and receiver enabled, receiver interrupt enabled
	ScalerR      = 0x0;

   //!Wait for socket connection
	fprintf(stderr, "APBUART: waiting for connection on port ...\n");
	server.accept(plug);
	
	SC_HAS_PROCESS(ac_tlm_apbuart);
	SC_THREAD(uart_receive);
}

//!Default Destructor
ac_tlm_apbuart::~ac_tlm_apbuart(){}

//!Generate interrupt method definition
void ac_tlm_apbuart::generate_interrupt()
{
   ac_tlm_req request;
   ac_tlm_rsp response;

   request.type = WRITE;
   request.data = 0x2;
   request.dev_id = 0x66;
	//!FIXME:
   request.addr = IRQ_SEND_ADDR;
	dbg_message("Sent interruption IRQ = 2 to processor\n");

   response = IRQ_port->transport(request);

	if (response.status == ERROR)
		err_message("TLM Error when trying send a processor interruption\n"); 
}

//!UART receive method definition
void ac_tlm_apbuart::uart_receive()
{
   for(;;)
   {

     wait(1, SC_MS); //!sleep for 1ms 
	  //!Test if device receive a string from socket ou the buffer is not empty
	  if(plug.data_ready())
	  {
		  if(ControlR & RECEIVER_ENABLE)
        {
 				string data_rxd;
				plug >> data_rxd;
				dbg_message("UART receive\n");
				//!Bufferize the received string
				for ( int i=0; i < data_rxd.length(); i++ )
					   buffer.push(data_rxd[i]);
				//!Write only one char to Holding Register
				DataR = (char) DATA(buffer.front());
				buffer.pop(); //!Remove the readed char

				StatusR |= DATA_READY;
				StatusR &= ~TRASMITTER_SHIFT_REGISTER_EMPTY;
				StatusR &= ~TRASMITTER_FIFO_EMPTY;

            if(ControlR & RECEIVER_INTERRUPT_ENABLE)
				{
				  //!Generate a interruption to processor
				  generate_interrupt();
			   }
        }
     }
   } //!thread loop
}

//!Transport method definition
ac_tlm_rsp ac_tlm_apbuart::transport( const ac_tlm_req &request ) 
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
			err_message("TLM transport error\n");
         response.status = ERROR;
         return response; 
  }
}

//!UART write method definition
ac_tlm_rsp_status ac_tlm_apbuart::uart_write(const uint32_t &a , const uint32_t &d)
{

  uint32_t internal_address = a & 0xFF;

  switch(internal_address){
	 case DATA_OFFSET:     //!0x0
			DataR = DATA(d);
			dbg_message("Data Register Write <- %c\n", DataR);
			StatusR |= DATA_READY;
		   StatusR &= ~TRASMITTER_SHIFT_REGISTER_EMPTY;
		   StatusR &= ~TRASMITTER_FIFO_EMPTY; 
			if(ControlR & TRANSMITTER_ENABLE)
			{
				string data_txd = "";
			   data_txd = (char) DATA(DataR);
				plug << data_txd;
	
				if(ControlR & TRANSMITTER_INTERRUPT_ENABLE)
            { 
					generate_interrupt();
 				}
		      StatusR &= ~DATA_READY;
		      StatusR |= TRASMITTER_SHIFT_REGISTER_EMPTY;
			   StatusR |= TRASMITTER_FIFO_EMPTY;
				DataR = 0x0;
			}
		  	return SUCCESS;
	 case STATUS_OFFSET:   //!0x4
			//!Ready Only
			dbg_message("Status Register Write [Read Only] <- %x\n", d);
			return SUCCESS;
	 case CONTROL_OFFSET:  //!0x8
			ControlR = d;
			dbg_message("Control Register Write <- %x\n", ControlR);
			return SUCCESS;
	 case SCALER_OFFSET:   //!0xC
			ScalerR = d;
			dbg_message("Scaler Register Write <- %x\n", ScalerR);
			return SUCCESS;
	 default:
		   err_message("Unknown or unsupported register write\n");
			return ERROR;
  }
}

//!UART read method definition
ac_tlm_rsp_status ac_tlm_apbuart::uart_read(const uint32_t &a , uint32_t &d)
{

  uint32_t internal_address = a & 0xFF;

  switch(internal_address){
	 case DATA_OFFSET:     //!0x0
			d = DataR;
			dbg_message("Data Register Read  -> %c\n", d);
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
			d = StatusR;
			dbg_message("Status Register Read  -> %x\n", StatusR);
			return SUCCESS;
	 case CONTROL_OFFSET:  //!0x8
			d = ControlR;
			dbg_message("Control Register Read  -> %x\n", ControlR);
			return SUCCESS;
	 case SCALER_OFFSET:   //!0xC
			d = ScalerR;
			dbg_message("Scaler Register Read  -> %x\n", ScalerR);
			return SUCCESS;
	 default:
		   err_message("Unknown or unsupported register write\n");
			return ERROR;
  }
}





