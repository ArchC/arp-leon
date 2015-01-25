/**
 * @file      loader.h
 * @author    
 * 
 * @author    The ArchC Team
 *            http://www.archc.org/
 *
 *            Computer Systems Laboratory (LSC)
 *            IC-UNICAMP
 *            http://www.lsc.ic.unicamp.br/
 * 
 * @version   0.1
 * @date      Wed, 16 Mar 2006 08:07:46 -0200
 * 
 * @brief     Linux Kernel loader for SPARC.
 * 
 * @attention Copyright (C) 2002-2006 --- The ArchC Team
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

#include "ac_utils.H"
#include "leon3_parms.H"
#include "ac_tlm_mem.h"
#ifdef __CYGWIN__ //Fix for Cygwin users, that do not have elf.h
#include "elf32-tiny.h"
#else
#include <elf.h>
#endif /* __CYGWIN__ */
#include <stdio.h>

//#define LOADER_DUMP

using user::ac_tlm_mem;

void load_elf(leon3 &proc, ac_tlm_mem &mem, 
              char* filename, unsigned int offset, unsigned int mem_size){
#ifdef LOADER_DUMP
    FILE *dump_file;
    dump_file = fopen("dump","w");
#endif
    Elf32_Ehdr    ehdr;
    Elf32_Shdr    shdr;
    Elf32_Phdr    phdr;
    int           fd;
    unsigned int  i;
    unsigned int data_mem_size=(mem_size);/*32MB*/

  if (!filename || ((fd = open(filename, 0)) == -1)) {
    AC_ERROR("Openning application file '" << filename << 
             "': " << strerror(errno) << endl);
    exit(EXIT_FAILURE);
  }

  //Test if it's an ELF file
  if ((read(fd, &ehdr, sizeof(ehdr)) != sizeof(ehdr)) ||  // read header
      (strncmp((char *)ehdr.e_ident, ELFMAG, 4) != 0) ||  // test magic number
      0) {
    close(fd);
    AC_ERROR("File '" << filename << "' is not an elf. : " << strerror(errno) << endl);
    exit(EXIT_FAILURE);
  }

  //Set start address
  proc.ac_start_addr = convert_endian(4,ehdr.e_entry, proc.ac_mt_endian);

  //printf("\nPC start address 0x%x NPC at 0x%x\n\n",proc.ac_start_addr, proc.npc.read() );
  if (convert_endian(2,ehdr.e_type, proc.ac_mt_endian) == ET_EXEC) {
    //It is an ELF file
    AC_SAY("\nReading ELF application file: " << filename << endl);
    
    for (i=0; i<convert_endian(2,ehdr.e_phnum, proc.ac_mt_endian); i++) {
      //Get program headers and load segments
      lseek(fd, convert_endian(4,ehdr.e_phoff, proc.ac_mt_endian) + 
      convert_endian(2,ehdr.e_phentsize, proc.ac_mt_endian) * i, SEEK_SET);
      if (read(fd, &phdr, sizeof(phdr)) != sizeof(phdr)) {
          AC_ERROR("Reading ELF program header\n");
	  close(fd);
	  exit(EXIT_FAILURE);
      }
  
      if (convert_endian(4,phdr.p_type, proc.ac_mt_endian) == PT_LOAD) {
	Elf32_Word j;
	Elf32_Addr p_vaddr = convert_endian(4,phdr.p_vaddr, proc.ac_mt_endian);
	Elf32_Word p_memsz = convert_endian(4,phdr.p_memsz, proc.ac_mt_endian);
	Elf32_Word p_filesz = convert_endian(4,phdr.p_filesz, proc.ac_mt_endian);
	Elf32_Off  p_offset = convert_endian(4,phdr.p_offset, proc.ac_mt_endian);
        #ifdef LOADER_DUMP
        fprintf(dump_file,"\n Section Info:");
     	fprintf(dump_file,"\n ELF VAddr 0x%.8x",p_vaddr);
        fprintf(dump_file,"\n ELF Mem Size 0x%.8x",p_memsz);
        fprintf(dump_file,"\n ELF File Size 0x%.8x",p_filesz);
        fprintf(dump_file,"\n ELF Offset 0x%.8x",p_offset);
        #endif
	//Error if segment greater then memory
	if (data_mem_size < p_memsz) { 
 		AC_ERROR("not enough memory in ArchC model to load application.\n");
 		close(fd);
 		exit(EXIT_FAILURE);
	}
  
	//Set heap to the end of the segment
	if (proc.ac_heap_ptr < (p_vaddr-offset) + p_memsz) proc.ac_heap_ptr = (p_vaddr-offset) +  p_memsz; 
	if(!proc.dec_cache_size)
 	proc.dec_cache_size = proc.ac_heap_ptr;
	//Load and correct endian
	lseek(fd, p_offset, SEEK_SET);

        printf("Writting section 0x%.8x ELF to memory address : 0x%.8x\n",p_vaddr, p_vaddr - offset);

	for (j=0; j < p_filesz; j+=sizeof(leon3_parms::ac_word)) {
 		uint32_t tmp;
 		ssize_t ret_value = read(fd, &tmp, sizeof(leon3_parms::ac_word));/*TODO: Alterar e tornar esse sizeof dinamico*/
 		uint32_t d        =  convert_endian(sizeof(leon3_parms::ac_word), tmp, 1);/*WARNING: Dont invert endian here*/
                uint32_t addr     = (uint32_t)((p_vaddr-offset)+j); /*cast to uint32_t*/
 		mem.direct_write(addr, d); /*direct write*/
                #ifdef LOADER_DUMP
     		fprintf(dump_file,"\n\t 0x%.8x \t0x%.8x",addr, d);
                #endif
	}
	uint32_t d = 0;
	for(j = p_vaddr+p_filesz; j <= p_memsz-p_filesz; j++){
                uint32_t addr = (uint32_t)((p_vaddr-offset)+j);
		mem.direct_write(addr, d);
                #ifdef LOADER_DUMP
     		fprintf(dump_file,"\n\t 0x%.8x \t0x%.8x",addr, d);
                #endif
        }
      }
  
    }
  };
  #ifdef LOADER_DUMP
  fclose(dump_file);
  #endif	
  close(fd);
}
