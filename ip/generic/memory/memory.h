/**
 * \file      memory.h
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
 * \brief     Defines a generic memory.
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

#ifndef _GENERIC_MEMORY_H_
#define _GENERIC_MEMORY_H_

#include <systemc>
#include "ac_tlm_protocol.H"

using tlm::tlm_transport_if;

namespace generic
{
class memory :
  public sc_module,
  public ac_tlm_transport_if
{
public:

  sc_export< ac_tlm_transport_if > target_export;

  /**
   * \brief Memory write
   * 
   * \param addr memory address to be writem
   * \param data data to be writem
   * 
   * \return a TLM response status. SUCCESS or ERROR
   */
  ac_tlm_rsp_status mem_write( const uint32_t& addr, const uint32_t& data );

  /**
   * \brief Memory read
   * 
   * \param addr memory address where read
   * \param data readed data
   * 
   * \return a TLM response status. SUCCESS or ERROR
   */
  ac_tlm_rsp_status mem_read( const uint32_t& addr, uint32_t& data );

  bool direct_read(uint32_t& addr, uint32_t& data);
  bool direct_write(uint32_t& addr, uint32_t& data);

  /**
   * Implementation of TLM transport
   * \param request is a received request packet
   * \return A response packet to be send
  */
  ac_tlm_rsp transport( const ac_tlm_req &request );


  /** 
   * \brief Overloaded constructor.
   * 
   * \param start_addr Memory start address size in bytes.
   * \param end_addr Memory end address size in bytes.
   */
  memory( sc_module_name module_name , uint32_t start_addr, uint32_t end_addr );

  /**
   * \brief gets memory size
   * \return memory size in bytes
   */
  uint32_t get_size();

  /** 
   * \brief Default constructor.
   * 
   * \param size memory size in bytes
   * 
   */
  memory( sc_module_name module_name , uint32_t size );
  /** 
   * Default destructor.
   */
  ~memory();

private:

  uint8_t *memory_;
  uint32_t size_;
  uint32_t start_address_;
  uint32_t end_address_;

};

} //namespace generic

#endif //_GENERIC_MEMORY_H_

