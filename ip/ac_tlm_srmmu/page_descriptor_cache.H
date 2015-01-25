/**
 * @file      page_descriptor_cache.h
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
 * @brief     Definition file. 
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

#include <iostream>
#include <map>
#include <stdio.h>
#include <stdint.h>

#define PDC_ENTIRE 4
#define PDC_HIT    0
#define PDC_MISS   1

using namespace std;

//!Defines a pair [context number + virtual address] as cache tag
typedef pair<uint32_t, uint32_t> tag;
//!Define the PDC cache as [context_number][virtual_address][page_description_entry] 
typedef map<tag, uint32_t> pdc;

class page_descriptor_cache 
{
  private:
	 //!Define the PDC Cache
    pdc pdc_cache;
	 string name;
    string pol; //!Cache replacement policie
    uint32_t count;
    uint32_t size;

    /**
    * Selects a tlb entry to remove based on the policie
    *
	 **/
    void remove();

	 /**
    * Gets the device name for debbuging purpouses
    * @returns a string containg the device name
    **/
    string get_name();

    /**
    * Gets the PDC cache size
	 * @return a unsigned value with the cache size
    **/
    uint32_t get_size();

  public:

    /**
    * Find if a page entry is in the PDC cache
	 * @param a unsigned with the context number
	 * @param a unsigned with the virtual address
	 * @param a reference to a local where the page description entry
	 *        founded will be stored
 	 * @return a integer 0 if the page has been found or 1 if we have a miss
    **/
    int find(uint32_t context_num, uint32_t vaddr, uint32_t& paddr);

    /**
    * Store a translated page entry to PDC cache
	 * @param a unsigned with the context number
	 * @param a unsigned with the virtual address
	 * @param a unsigned with the page description entry
 	 * @return a integer 0 if success or 1 if an error occourr
    **/
    int fill(uint32_t context_num, uint32_t vaddr, uint32_t& page_d);

    /**
    * Store a translated page entry to PDC cache
	 * @param a unsigned with the context number
	 * @param a unsigned with the virtual address
 	 * @return a integer 0 if success or 1 if an error occourr
    **/
    void flush(int type);

    /**
    * Default constructor
	 * @param a string with the device name
	 * @param a unsigned with max size (number of lines) of cache. 
	 *        Default = 0 (infinity)
	 * @param a string with the cache replacement policie. Default = R(Random)
    **/
    page_descriptor_cache( string nm, uint32_t sz=0, string policie="R" );

	 /**
    * Default destructor 
	 **/
    ~page_descriptor_cache();
};
