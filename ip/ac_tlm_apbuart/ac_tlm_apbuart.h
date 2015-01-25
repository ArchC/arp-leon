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
 * @brief     Definition file. 
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

#ifndef AC_TLM_APBUART_H
#define AC_TLM_APBUART_H

#include <systemc.h>
#include <stdio.h>
#include <queue>

#include "serversocket.H"
#include "ac_tlm_protocol.H"
#include "ac_tlm_port.H"



/*Uncomment the line if you want debug tlm methods*/
//#define DEBUG_MODEL

#ifdef DEBUG_MODEL
#  define dbg_message(args...) fprintf (stderr, "APBUART DEBUG: " args)
#  define err_message(args...) fprintf (stderr, "APBUART ERROR: " args)
#  define _d_(arg) arg
#else
#  define dbg_message(args...)
#  define err_message(args...) fprintf (stderr, "APBUART ERROR: " args)
#  define _d_(arg)
#endif


//!UART bit macros
//!Data Register bit mask
#define DATA(n)                           (n & 0xFF)
//!Status Register bit mask
#define DATA_READY                        (1 << 0)
#define TRASMITTER_SHIFT_REGISTER_EMPTY   (1 << 1)
#define TRASMITTER_FIFO_EMPTY             (1 << 2)
#define TRANSMITTER_BREAK_RECEIVE         (1 << 3)
#define OVERRUN                           (1 << 4)
#define PARITY_ERROR                      (1 << 5)
#define FRAMING_ERROR                     (1 << 6)
#define TRANSMITTER_HALF_FULL             (1 << 7)
#define RECEIVER_HALF_FULL                (1 << 8)
#define TRANSMITTER_FULL                  (1 << 9)
#define RECEIVER_FULL                     (1 << 10)
#define TRANSMITTER_FIFO_COUNT(n)         (n & (0x3f << 20)) >> 20
#define RECEIVER_FIFO_COUNT(n)            (n & (0x3f << 26)) >> 26
//!Control Register bit mask
#define RECEIVER_ENABLE                   (1 << 0)
#define TRANSMITTER_ENABLE                (1 << 1)
#define RECEIVER_INTERRUPT_ENABLE         (1 << 2)
#define TRANSMITTER_INTERRUPT_ENABLE      (1 << 3)
#define PARITY_SELECT                     (1 << 4)
#define PARITY_ENABLE                     (1 << 5)
#define LOOP_BACK_MODE                    (1 << 7)
#define TRANSMITTER_FIFO_INTERRUPT_ENABLE (1 << 9)
#define TRANSMITTER_FIFO_RECEIVER_ENABLE  (1 << 10)
//!Scaler Register bit mask
#define SCALER_RELOAD_VALUE(n) (n & 0xFFF)

//!UART Register Offsets
#define DATA_OFFSET    0x0
#define STATUS_OFFSET  0x4
#define CONTROL_OFFSET 0x8
#define SCALER_OFFSET  0xC

#define IRQ_SEND_ADDR 0x80000204

using std::queue;
using tlm::tlm_transport_if;

/*isolates from ArchC namespace*/
namespace user
{
  class ac_tlm_apbuart : 
  public sc_module,
  public ac_tlm_transport_if
 {

   private:
     //! UART Registers
     uint32_t DataR; 
     uint32_t StatusR;
     uint32_t ControlR;
     uint32_t ScalerR;

     //!Socket variables
     uint32_t socket_port;
     ServerSocket server;
     ServerSocket plug;
     /*Most of socket programs like netcat or telnet sent a string instead a
       character after press <enter>. So we need a buffer to hold the string
       and sent one char at time to UART.
     */
	  queue<char> buffer;

   public:
	  //!TLM port
     sc_export< ac_tlm_transport_if > target_export;
     //!IRQ port
     ac_tlm_port IRQ_port;

     /**
     * UART write method
	  * @param   a const unsigned value correponding to the register address
	  * @param   a const unsigned value correponding to the data to be write
	  * @return  a TLM response status SUCCESS or ERROR
     **/
     ac_tlm_rsp_status uart_write(const uint32_t & , const uint32_t &);

     /**
     * UART read method
	  * @param   a const unsigned value correponding to the register address
	  * @param   a const unsigned value correponding to the readed data
	  * @return  a TLM response status SUCCESS or ERROR
     **/
     ac_tlm_rsp_status uart_read(const uint32_t & , uint32_t &);  

     /**
     * UART receive method (SystemC Thread)
	  * checks if UART receive any character from a external program
     */
	  void uart_receive();

	  /**
	  * Implementation of TLM transport method that
	  * handle packets of the protocol doing apropriate actions.
	  * This method must be implemented (required by SystemC TLM).
	  * @param request is a received request packet
	  * @return A response packet to be send
	  */
     ac_tlm_rsp transport( const ac_tlm_req &request );


	  /**
	  * Generate IRQ interrupt to processor
	  *
	  **/
     void generate_interrupt();

     /**
     * APBUART default constructor
	  * @param   a string with the module name
	  * @param   a unsigned value with the socket port where the connection
	  *          will be done
     **/
     ac_tlm_apbuart(sc_module_name module_name, uint32_t socket_port = 6000U);
	  /**
	  * APBUART default destructor
     **/
     ~ac_tlm_apbuart();
  };
};

#endif /*AC_TLM_APBUART_H*/
