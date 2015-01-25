/**
 * @file      ac_tlm_srmmu.H
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
 * @date      
 *
 * @brief     Defines Sparc Memory Management Unit (SRMMU).
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

#ifndef AC_TLM_SRMMU_H
#define AC_TLM_SRMMU_H

#include <systemc.h>
#include <stdio.h>

#include "ac_tlm_protocol.H"
#include "ac_tlm_port.H"
#include "bitutils.h"
#include "page_descriptor_cache.H"

/*Uncomment the line if you want debug tlm methods*/
//#define DEBUG_MODEL

#ifdef DEBUG_MODEL
#  define dbg_printf(args...) fprintf (stderr, "MMU DEBUG: " args)
#  define _d_(arg) arg
#else
#  define dbg_printf(args...)
#  define _d_(arg)
#endif


#define ASI_SYSR                0x02   
#define ASI_UINST               0x08   
#define ASI_SINST               0x09   
#define ASI_UDATA               0x0A   
#define ASI_SDATA               0x0B   
#define ASI_ITAG                0x0C   
#define ASI_IDATA               0x0D   
#define ASI_DTAG                0x0E   
#define ASI_DDATA               0x0F   
#define ASI_IFLUSH              0x10   
#define ASI_DFLUSH              0x11   
#define ASI_FLUSH_PAGE          0x10   
#define ASI_FLUSH_CTX           0x13   
#define ASI_DCTX                0x14   
#define ASI_ICTX                0x15   
#define ASI_MMUFLUSHPROBE       0x18   
#define ASI_MMUREGS             0x19   
#define ASI_MMU_BP              0x1C  
#define ASI_MMU_DIAG            0x1D   
#define ASI_MMU_DSU             0x1F   
#define ASI_LEON_NOCACHE        0x01
#define ASI_LEON_DCACHE_MISS    0x01
#define ASI_LEON_CACHEREGS      0x02
#define ASI_LEON_IFLUSH         0x10
#define ASI_LEON_DFLUSH         0x11
#define ASI_LEON_MMUFLUSH       0x18
#define ASI_LEON_MMUREGS        0x19
#define ASI_LEON_BYPASS         0x1C
#define ASI_LEON_FLUSH_PAGE     0x10
#define ASI_LEON_DIAG_ACCESS    0x1D
#define ASI_USERTXT             0x08
#define ASI_KERNELTXT           0x09
#define ASI_USERDATA            0x0A
#define ASI_KERNELDATA          0x0B
#define ASI_M_RES00             0x00   // ! Reserved 
#define ASI_M_UNA01             0x01   // ! Unassigned 
#define ASI_M_MXCC              0x02   // ! Access to TI VIKING MXCC registers 
#define ASI_M_FLUSH_PROBE       0x03   // ! Reference MMU Flush/Probe 
#define ASI_M_MMUREGS           0x04   // ! MMU Registers 
#define ASI_M_TLBDIAG           0x05   // ! MMU TLB only Diagnostics 
#define ASI_M_DIAGS             0x06   // ! Reference MMU Diagnostics 
#define ASI_M_IODIAG            0x07   // ! MMU I/O TLB only Diagnostics 
#define ASI_M_USERTXT           0x08   // ! Same as ASI_USERTXT 
#define ASI_M_KERNELTXT         0x09   // ! Same as ASI_KERNELTXT 
#define ASI_M_USERDATA          0x0A   // ! Same as ASI_USERDATA 
#define ASI_M_KERNELDATA        0x0B   // ! Same as ASI_KERNELDATA 
#define ASI_M_TXTC_TAG          0x0C   // ! Instruction Cache Tag 
#define ASI_M_TXTC_DATA         0x0D   // ! Instruction Cache Data  
#define ASI_M_DATAC_TAG         0x0E   // ! Data Cache Tag 
#define ASI_M_DATAC_DATA        0x0F   // ! Data Cache Data
#define ASI_M_FLUSH_PAGE        0x10   // ! Flush I&D Cache Line (page)
#define ASI_M_FLUSH_SEG         0x11   // ! Flush I&D Cache Line (seg)
#define ASI_M_FLUSH_REGION      0x12   // ! Flush I&D Cache Line (region)
#define ASI_M_FLUSH_CTX         0x13   // ! Flush I&D Cache Line (context)
#define ASI_M_FLUSH_USER        0x14   // ! Flush I&D Cache Line (user)
#define ASI_M_BCOPY             0x17   // ! Block copy 
#define ASI_M_IFLUSH_PAGE       0x18   // ! Flush I Cache Line (page)
#define ASI_M_IFLUSH_SEG        0x19   // ! Flush I Cache Line (seg)
#define ASI_M_IFLUSH_REGION     0x1A   // ! Flush I Cache Line (region)
#define ASI_M_IFLUSH_CTX        0x1B   // ! Flush I Cache Line (context)
#define ASI_M_IFLUSH_USER       0x1C   // ! Flush I Cache Line (user)
#define ASI_M_BFILL             0x1F  // ! Block-Fill
#define ASI_M_BYPASS            0x20   // ! Reference MMU bypass
#define ASI_M_FBMEM             0x29   // ! Graphics card frame buffer access 
#define ASI_M_VMEUS             0x2A   // ! VME user 16-bit access 
#define ASI_M_VMEPS             0x2B   // ! VME priv 16-bit access 
#define ASI_M_VMEUT             0x2C   // ! VME user 32-bit access 
#define ASI_M_VMEPT             0x2D   // ! VME priv 32-bit access 
#define ASI_M_SBUS              0x2E   // ! Direct SBus access 
#define ASI_M_CTL               0x2F   // ! Control Space (ECC and MXCC are here) 


//!SRMMU Internal Registers 
#define NUM_REGS           5

//!PTE Entry types
#define ET_INVALID  0
#define ET_PDE      1
#define ET_PTE      2
#define ET_RESERVED 3

//!Page Map Sizes
#define L1_REGION_MASK 0x00FFFFFF /*16MB*/
#define L2_REGION_MASK 0x0003FFFF /*256KB*/
#define L3_REGION_MASK 0x00000FFF /*4KB*/

//Region Mapped Sizes
#define L0_INDEX_MASK 0xFF000000
#define L1_INDEX_MASK 0xFC0000
#define L2_INDEX_MASK 0x3F000

//!Errors
#define INVALID_ADDRESS_ERROR      (1 << 2)
#define PROTECTION_ERROR           (2 << 2)
#define PRIVILEDGE_VIOLATION_ERROR (3 << 2)
#define TRANSLATION_ERROR          (4 << 2)
#define ACCESS_BUS_ERROR           (5 << 2)
#define INTERNAL_ERROR             (6 << 2)

#define L1_ERROR (1 << 8)
#define L2_ERROR (2 << 8)
#define L3_ERROR (3 << 8)

//!Bit masks
#define REFERENCED_MASK (1 << 5)
#define MODIFIED_MASK   (1 << 6)
#define NF_MASK 2
#define ET 3
#define ACC 0x1c
#define MMU_CTRL_MASK 0x007fff83

using tlm::tlm_transport_if;

/*isolates from ArchC namespace*/
namespace user
{
  class ac_tlm_srmmu : 
  public sc_module,
  public ac_tlm_transport_if
 {

   private:

     //!SRMMU Internal Registers
     uint32_t mmu_regs[NUM_REGS];

     //!ASI Holder
     unsigned int asi;  

     //!Functional PDC Cache
     //!FIXME: Pass this as argument to constructor
     //!FIXME: Create two pdc cache one for instruction and other for data
	  page_descriptor_cache* pdc_cache;   
	
     //!Acess Table
     static const int access_table[8][8];

     /**
     * TODO: Implement this
     * Performs a probe operation. On probe returns a page table entry from PDC or Page Table or an error
     * A probe is accomplished by executing a privileged load alternate instruction with the 
     * appropriate address (given by the VFPA field), type (given by the Type field), 
     * context (given by the Context register), and address space identifier (ASI)
     * It's rarely used.
     */
     void srmmu_probe(const uint32_t &);

     /**
     * TODO: Implement this
     * A flush operation removes from the PDC a PTE or PTD that matches the criteria
     * implied by the Type field. A flush is accomplished by executing a store alter-
     * nate instruction to the appropriate address (given by the VFPA field), with the
     * appropriate type (given by the Type field), with the appropriate context (given by
     * the Context register), and with the appropriate address space identifier (ASI),
     * which is implementation-dependent. The data supplied by the store alternate
     * instruction is ignored.
     * The kernel seems to always flush the entire PDC cache
     */
     void srmmu_flush(const uint32_t &);

     /**
     * Performs Translation of a virtual address to a physical address.
     * This method updates the FSR and FAR register on error and signal a 
     * trap to processor.
     * FIXME: Atualmente a trap é sinalizada como uma interrupção. O melhor seria
     * alterar e passar uma referencia para o objeto Trap Helper para setar a trap diretamente
     * @param virtual address to be translated
     * @param physical address where the translated address will be stored
     * @param a integer indicates the access type
     * @return zero if no error as occoured or one if a error as occorred and the
     * the address cannot be translated
     */
     int srmmu_translate(const uint32_t &, uint32_t &, int acc_type);

     /**
     * Performs a table walk on Page table o miss processing
     * @param virtual address to be translated
     * @param physical address where the translated address will be stored
     * @param a integer indicates the access type
     * @return a error code indicates if a error as occoured or zero if no errors
     */
     int srmmu_table_walk(const uint32_t &, uint32_t &, int acc_type);

     /**
     * Read SRMMU internal registers
     * @param address of register where data will be read
     * @param a variable where the readed data will be stored
     * @return 0 if operation is successifully or 1 if an error has occoured.
     * If an error occour the FSR and FAR register are updated
     */
     int read_internal_register(const uint32_t &, uint32_t &);
     /**
     * Write SRMMU internal registers
     * @param address of register where data will be write
     * @param data to be write
     * @return 0 if operation is successifully or 1 if an error has occoured.
     * If an error occour the FSR and FAR register are updated
     */
     int write_internal_register(const uint32_t &, const uint32_t &);
     
   public:
     //!FIXME:
     sc_export< ac_tlm_transport_if > target_export;

     //!TLM External Port
     ac_tlm_port ROUTER_port;

     //!FIXME: Remover a interface entre processador e memoria usando TLM
     //!Create a conection with CPUs
     sc_port< ac_tlm_transport_if > CPU_port;

     /*SRMMU read and write machethods*/
     ac_tlm_rsp_status srmmu_write(const uint32_t & , const uint32_t &);
     ac_tlm_rsp_status srmmu_read(const uint32_t & , uint32_t &);  

     /**
     * Write a data to a physical address pointed by address
     * using TLM protocol. Can be any storage element
     * @param address where the data will be write
     * @param data to be write
     * @return A response packet containing the write status
     */
     ac_tlm_rsp write_phys_addr(const uint32_t& addr, const uint32_t& data);

     /**
     * Read a physical address contents pointed by address
     * using TLM protocol. Can be any storage element
     * @param address to be read
     * @return A response packet contains the readed value pointed by addr
     */
     ac_tlm_rsp read_phys_addr(const uint32_t& addr);

     /**
     * Implementation of TLM transport method that
     * handle packets of the protocol doing apropriate actions.
     * This method must be implemented (required by SystemC TLM).
     * @param request is a received request packet
     * @return A response packet to be send
     */
     ac_tlm_rsp transport( const ac_tlm_req &request );

     /*
     * Default Construction
     * @param a object sc_module_name (string) contains the module name
     * of the module
     */
     ac_tlm_srmmu(sc_module_name module_name);
     /*
     * Default destructor 
     */
     ~ac_tlm_srmmu();
  };
};

#endif /*AC_TLM_SRMMU_H*/
