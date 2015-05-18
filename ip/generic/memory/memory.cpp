/**
 * \file      memory.cpp
 * \author    Rogerio Alves
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
 * \brief     Implements a generic memory.
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

#include "memory.h"

namespace generic {

memory::memory( sc_module_name module_name,
                uint32_t start_address,
		            uint32_t end_address ) :
  sc_module( module_name ),
  target_export("iport"),
  size_(0),
  start_address_(start_address),
  end_address_(end_address) {

  //Binds target_export to the memory
  target_export( *this );

  sc_assert(start_address_ <= end_address_);
  sc_assert((end_address_ - start_address_+1) % 4 == 0);

  size_ = (end_address_-start_address_+1);

  /// Initialize memory vector
  memory_ = new uint8_t[ size_ ];
  for(unsigned int i = 0; i < size_; ++i) 
    memory_[i] = 0;
}

memory::memory( sc_module_name module_name, uint32_t size) :
  sc_module( module_name ),
  target_export("iport"),
  size_(size),
  start_address_(0),
  end_address_(0) {

 //Binds target_export to the memory
  target_export( *this );

  memory_ = new uint8_t[ size_ ];
  for(uint32_t i = 0; i < size_; ++i) 
      memory_[i] = 0;
}

memory::~memory() {
  delete [] memory_;
  memory_ = NULL;
}

ac_tlm_rsp_status memory::mem_write( const uint32_t &addr , const uint32_t &data ) {
  memory_[(addr - start_address_)]   = (((uint8_t*)&data)[0]);
  memory_[(addr - start_address_)+1] = (((uint8_t*)&data)[1]);
  memory_[(addr - start_address_)+2] = (((uint8_t*)&data)[2]);
  memory_[(addr - start_address_)+3] = (((uint8_t*)&data)[3]);

  return SUCCESS;
}

ac_tlm_rsp_status memory::mem_read( const uint32_t &addr , uint32_t &data ) {
  (((uint8_t*)&data)[0]) = memory_[(addr - start_address_)];
  (((uint8_t*)&data)[1]) = memory_[(addr - start_address_)+1];
  (((uint8_t*)&data)[2]) = memory_[(addr - start_address_)+2];
  (((uint8_t*)&data)[3]) = memory_[(addr - start_address_)+3];

  return SUCCESS;
}

uint32_t memory::get_size() {
  return size_;
}

bool memory::direct_read(uint32_t& addr , uint32_t& data)
{
return (mem_read(addr, data) == SUCCESS);
}

bool memory::direct_write(uint32_t& addr , uint32_t& data)
{
return (mem_write(addr, data) == SUCCESS);
}

ac_tlm_rsp memory::transport( const ac_tlm_req &request ) {

  ac_tlm_rsp response;

  switch( request.type ) {
    case READ :
      response.status = mem_read( request.addr , response.data );
      break; 
    case WRITE:
      response.status = mem_write( request.addr , request.data );
      break;
    case LOCK:
      break;
    case UNLOCK:
      break;
    default :
      response.status = ERROR;
      break; 
  }
  return response;
}

} //namespace generic

