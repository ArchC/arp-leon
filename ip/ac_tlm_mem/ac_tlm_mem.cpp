/**
 * @file      ac_tlm_mem.cpp
 * @author    Bruno de Carvalho Albertini
 * 
 * @author    The ArchC Team
 *            http://www.archc.org/
 *
 *            Computer Systems Laboratory (LSC)
 *            IC-UNICAMP
 *            http://www.lsc.ic.unicamp.br/
 * 
 * @version   0.1
 * @date      Sun, 02 Apr 2006 08:07:46 -0200
 * 
 * @brief     Implements a ac_tlm memory.
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
 * 
 */

//////////////////////////////////////////////////////////////////////////////
// Standard includes
// SystemC includes
// ArchC includes

#include "ac_tlm_mem.h"

//////////////////////////////////////////////////////////////////////////////

/// Namespace to isolate memory from ArchC
using user::ac_tlm_mem;

/// Constructor
ac_tlm_mem::ac_tlm_mem( sc_module_name module_name,
                        unsigned int start_address,
		        unsigned int end_address ) :
  sc_module( module_name ),
  m_start_address(start_address),
  m_end_address(end_address),
  target_export("iport")
{
    /// Binds target_export to the memory
    target_export( *this );

    sc_assert(m_start_address <= m_end_address);
    sc_assert((m_end_address-m_start_address+1)%4 == 0);
    unsigned int size = (m_end_address-m_start_address+1);

    /// Initialize memory vector
    memory = new uint8_t[ size ];
    for(unsigned int i = 0; i < size; ++i) memory[i] = 0;

}

/// Overloaded Constructor
ac_tlm_mem::ac_tlm_mem( sc_module_name module_name, unsigned int size) :
  sc_module( module_name ),
  target_export("iport")
{
    /// Binds target_export to the memory
    target_export( *this );

    m_start_address = 0;
    m_end_address   = 0;

    /// Initialize memory vector
    memory = new uint8_t[ size ];
    for(unsigned int i = 0; i < size; ++i) memory[i] = 0;

}

/// Destructor
ac_tlm_mem::~ac_tlm_mem() {

  delete [] memory;

}

/** Internal Write
  * Note: Always write 32 bits
  * @param a is the address to write
  * @param d id the data being write
  * @returns A TLM response packet with SUCCESS
*/
ac_tlm_rsp_status ac_tlm_mem::writem( const uint32_t &a , const uint32_t &d )
{
  memory[(a - m_start_address)]   = (((uint8_t*)&d)[0]);
  memory[(a - m_start_address)+1] = (((uint8_t*)&d)[1]);
  memory[(a - m_start_address)+2] = (((uint8_t*)&d)[2]);
  memory[(a - m_start_address)+3] = (((uint8_t*)&d)[3]);
  return SUCCESS;
}

/** Internal Read
  * Note: Always read 32 bits
  * @param a is the address to read
  * @param d id the data that will be read
  * @returns A TLM response packet with SUCCESS and a modified d
*/
ac_tlm_rsp_status ac_tlm_mem::readm( const uint32_t &a , uint32_t &d )
{
  (((uint8_t*)&d)[0]) = memory[(a - m_start_address)];
  (((uint8_t*)&d)[1]) = memory[(a - m_start_address)+1];
  (((uint8_t*)&d)[2]) = memory[(a - m_start_address)+2];
  (((uint8_t*)&d)[3]) = memory[(a - m_start_address)+3];
  //cout << "mem read " << hex << "address " << a << " " << "data " << d << endl;
  return SUCCESS;
}

bool ac_tlm_mem::direct_read(uint32_t &a , uint32_t &d)
{
return (readm(a, d) == SUCCESS);
}

bool ac_tlm_mem::direct_write(uint32_t &a , uint32_t &d)
{
return (writem(a, d) == SUCCESS);
}



