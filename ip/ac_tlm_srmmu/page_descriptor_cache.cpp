/**
 * @file      page_descriptor_cache.cpp
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
 * @brief     Implementation file. 
 *            Implements the PDC cache for SparcV8 SRMMU
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

#include "page_descriptor_cache.H"

string page_descriptor_cache::get_name()
{
  return name;
}

uint32_t page_descriptor_cache::get_size()
{
  return size;
}

//!Find a PDC entry method implementation
int page_descriptor_cache::find( uint32_t context_num, uint32_t vaddr, uint32_t& paddr )
{
  //!Assemble the tag (ctxn + vaddr) and find the cache line
  pdc::const_iterator it = pdc_cache.find(make_pair(context_num, vaddr));
  if(it != pdc_cache.end())
  {
    paddr = it->second; //!return the page descriptor
	 return PDC_HIT; //!0x0
  }
  return PDC_MISS;  //!0x1
}

//!Remove a PDC entry method implementation
void page_descriptor_cache::remove()
{
  //!TODO: Implement this
  fprintf(stderr, "Unsupported\n");
}

//!Insert a PDC entry method implementation
int page_descriptor_cache::fill( uint32_t context_num, uint32_t vaddr, uint32_t& page_d )
{
  tag t(context_num, vaddr);
  if(size == 0)
  {
    pdc_cache.insert(make_pair(t, page_d));
    return 0;
  } else {
    //Check if pdc is full
    if(size < count)
    {
      pdc_cache.insert(make_pair(t, page_d));
      count++;
      return 0;
    } else {
		//!TODO: Unsupported
      //
		return 0;
    }
  }
  return 1;
}

//!PDC Flush method implementation
void page_descriptor_cache::flush( int type )
{
  //!TODO: Implements other flush methods. For now we just flush the entire cache
  //       Linux Kernel seems to always flush the entire cache
  switch(type)
  {
    case PDC_ENTIRE:
		pdc_cache.clear();
      break;
    default:
		fprintf(stderr, "Flush Error\n");
  }
}

//!Default constructor implementation
//!FIXME: Use inialization list
page_descriptor_cache::page_descriptor_cache( string nm, uint32_t sz, string policie )
{
  name  = nm;
  pol   = policie;
  size  = sz;
  count = 0;
  //FIXME: Remove this
  fprintf(stderr, "PDC created\n");
}

//!Default destructor implementation
page_descriptor_cache::~page_descriptor_cache(){}
