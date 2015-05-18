/**
 * \file      srmmu.cpp
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
 *
 * \brief     Implements Sparc Memory Management Unit (SRMMU)
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

#include "srmmu.h"

namespace sparc {

const int srmmu::access_table[8][8] = {
    { 0, 0, 0, 0, 8, 0, 12, 12 },
    { 0, 0, 0, 0, 8, 0, 0, 0 },
    { 8, 8, 0, 0, 0, 8, 12, 12 },
    { 8, 8, 0, 0, 0, 8, 0, 0 },
    { 8, 0, 8, 0, 8, 8, 12, 12 },
    { 8, 0, 8, 0, 8, 0, 8, 0 },
    { 8, 8, 8, 0, 8, 8, 12, 12 },
    { 8, 8, 8, 0, 8, 8, 8, 0 }
  };

//!Default Constructor
srmmu::srmmu( sc_module_name module_name ):
  sc_module( module_name ),
  ROUTER_port("router",0U),
  target_export("iport")
{

  asi = 0x9;
  mmu_regs[0] = 0x0;
  mmu_regs[1] = 0x0;
  mmu_regs[2] = 0x0;
  mmu_regs[3] = 0x0;
  mmu_regs[4] = 0x0;
  /*Exposed port*/
  target_export( *this );

   pdc_cache = new page_descriptor_cache("PDC");
}

//!Default Destructor
srmmu::~srmmu(){}

//!SRMMU Tranlate methos
int srmmu::translate(const uint32_t &vaddr, uint32_t &paddr, int acc_type){
  //!Check if MMU is disabled
  if(!(mmu_regs[0] & 0x1)){
    paddr = vaddr; 
    return 0; 
  }
  int error_code = table_walk(vaddr, paddr, acc_type);
  if(error_code) {
    mmu_regs[4] = vaddr; //!Write fault address

    if(mmu_regs[3]) //!Check if overflow
      mmu_regs[3] |= 1;

    mmu_regs[3] |= (acc_type << 5) | error_code | 2; //!Write fault status
 
    //printf("Page Fault at 0x%x FSR-> 0x%x\n", mmu_regs[4], mmu_regs[3]);
    int no_fault = ((mmu_regs[0] & NF_MASK) >> 1); //!Check if is in No Fault Mode
    int issu = (asi == 0x9); //!Check if is in Supervisor Mode

    //!Generates a fault to processor only if is not in no fault mode or if is supervisor access
    if(!no_fault || issu) {
      ac_tlm_req request;
      ac_tlm_rsp response;
        switch(acc_type) {
          case 2:
          case 3:
          case 6:
          case 7:
            dbg_printf("Instruction access exception at 0x%x\n", mmu_regs[4]);
            request.data = 0x60; //Instruction access exception
            request.type = WRITE;
            response = CPU_port->transport(request); 
            break;
           case 0:
           case 1:
           case 4:
           case 5:
            dbg_printf("Data access exception at 0x%x\n", mmu_regs[4]);
            request.data = 0x7F;//Data access exception
            request.type = WRITE;
            response = CPU_port->transport(request);
            break;
         }
       } else {
          dbg_printf("NO FAULT MODE at 0x%x\n", asi, mmu_regs[4]);
          return 0;
       }
    return 1;
  }

  //!Insert the translated address to PDC Cache
  //pdc_cache->fill(mmu_regs[2], vaddr, paddr);
  return 0;
}

//!Write Physical Address Method
ac_tlm_rsp srmmu::write_phys_addr(const uint32_t& addr, const uint32_t& data) {
  ac_tlm_req req;
  req.addr = addr;
  req.data = data;
  req.type = WRITE;
  return ROUTER_port->transport(req);
}

//!Read Physical Address Method
ac_tlm_rsp srmmu::read_phys_addr(const uint32_t& addr) {
  ac_tlm_req req;
  req.addr = addr;
  req.type = READ;
  return ROUTER_port->transport(req);
}

//!External TLM Port Transport Method
ac_tlm_rsp srmmu::transport( const ac_tlm_req &request ) {
  ac_tlm_rsp response;
  //!FIXME: This is ugly, i pass the ASI number using a non mapped address
  if(request.addr == 0x90000000) {
    unsigned recv_data = request.data;
    endian_swap32(recv_data);
    asi = recv_data;
    response.status = SUCCESS;
    return response;
  }

  switch( request.type ) {
    case WRITE:               
      response.status = srmmu_write(request.addr, request.data);
      break;
    case READ:
      response.status = srmmu_read(request.addr, response.data);
      break;
    default:
      response.status = ERROR;
      break; 
  }
  return response;
}

int srmmu::table_walk(const uint32_t &vaddr, uint32_t &paddr, int acc_type)
{
  uint32_t pde;
  uint32_t pde_ptr;
  ac_tlm_rsp rsp;
  
  //!Check if PDC has a translation
  //if(pdc_cache->find(mmu_regs[2], vaddr, paddr) == PDC_HIT)
  //  return 0;

  //!Calculates Root Table Pointer
  pde_ptr = (mmu_regs[1] << 4) + (mmu_regs[2] << 2);
  rsp = read_phys_addr(pde_ptr);

  if(rsp.status == ERROR)
    return ACCESS_BUS_ERROR;
  pde = rsp.data;
  endian_swap32(pde);

  switch (pde & ET) {
    default: 
    case ET_INVALID:
         return INVALID_ADDRESS_ERROR;       
    case ET_PTE:
    case ET_RESERVED: 
           return TRANSLATION_ERROR;
    case ET_PDE: //L0 PDE
           pde_ptr = ((pde & ~ET) << 4) + ((vaddr & L0_INDEX_MASK) >> 22); 
          rsp = read_phys_addr(pde_ptr);
           if(rsp.status == ERROR)
              return ACCESS_BUS_ERROR;
           pde = rsp.data;
           endian_swap32(pde);
   
        switch (pde & ET) {
          default:
          case ET_INVALID:  //!Invalid
              return L1_ERROR | INVALID_ADDRESS_ERROR;
          case ET_RESERVED: //!Reserved
              return L1_ERROR | TRANSLATION_ERROR;
          case ET_PDE:      //!L1 PDE
              pde_ptr = ((pde & ~ET) << 4) + ((vaddr & L1_INDEX_MASK) >> 16);
                  rsp = read_phys_addr(pde_ptr);
                   if(rsp.status == ERROR)
                      return ACCESS_BUS_ERROR;
                   pde = rsp.data;
                   endian_swap32(pde);

              switch (pde & ET){
                default:
                case ET_INVALID:  //!Invalid
                    return L2_ERROR | INVALID_ADDRESS_ERROR;
                case ET_RESERVED: //!Reserved
                    return L2_ERROR | TRANSLATION_ERROR;
                case ET_PDE:      //!L2 PDE   
                    pde_ptr = ((pde & ~ET) << 4) + ((vaddr & L2_INDEX_MASK) >> 10); 
                          rsp = read_phys_addr(pde_ptr);
                           pde = rsp.data;
                           if(rsp.status == ERROR)
                              return ACCESS_BUS_ERROR;
                           endian_swap32(pde);

                    switch (pde & ET){
                      case ET_INVALID:  //!Invalid
                          return L3_ERROR | INVALID_ADDRESS_ERROR;
                      case ET_PDE:     //!L3 PDE 
                      case ET_RESERVED: //!Reserved
                          return L3_ERROR | TRANSLATION_ERROR;
                      case ET_PTE:      //!L3 PTE
                          paddr = (((pde >> 8) << 12) & ~(L3_REGION_MASK)) + (vaddr & L3_REGION_MASK);
              } 
              break;
          case ET_PTE: //!L2 PTE
              paddr = (((pde >> 8) << 12) & ~(L2_REGION_MASK)) + (vaddr & L2_REGION_MASK);
          } 
          break;
           case ET_PTE: //!L1 PTE    
               paddr = (((pde >> 8) << 12) & ~(L1_REGION_MASK)) + (vaddr & L1_REGION_MASK);
      }
         break;
  }

  int access_perms; 
  int error_code = 0;
  //Check if MMU is in No Fault Mode
  int no_fault = ((mmu_regs[0] & NF_MASK) >> 1);
  //Check if is Supervisor Mode 
  int issu = (asi == 0x9);
  //Get the ACC bits from PDE
  access_perms = (pde & ACC) >> 2;

  error_code = access_table[acc_type][access_perms];

  // Check page access permissions
  // returns a error only if NF (Non Fault Mode) bit is 0 
  // or if is in Supervisor mode
  if(!no_fault || issu) {
    if(error_code) {
      //dbg_printf("Protection violation error at 0x%x\n", vaddr);
      return error_code;
    }
  }
  //!Check if PDE needs to update !
  int is_dirty = (acc_type & 1) && !(pde & MODIFIED_MASK);

  if(!(pde & REFERENCED_MASK) || is_dirty) {
    pde |= REFERENCED_MASK;
    if(is_dirty) {
      pde |= MODIFIED_MASK;
    }
    endian_swap32(pde);
    rsp = write_phys_addr(pde_ptr, pde);

    if(rsp.status == ERROR)
      return ACCESS_BUS_ERROR; 
  }
  return error_code;
}

int srmmu::read_register(const uint32_t &addr, uint32_t &data){

  int rn = (addr >> 8);

  if(rn >= NUM_REGS) {
    printf("MMU ERROR: Invalid Register Access\n");
    mmu_regs[4] = addr; //!Write fault address
    if(mmu_regs[3])     //!Check if overflow
       mmu_regs[3] |= 1;

    mmu_regs[3] |= INTERNAL_ERROR | 2; //!Write Internal Error Status

    return 1;
  }

  data = mmu_regs[rn];
  if(rn == 3)
    mmu_regs[3]  = 0x0;

  return 0;
}

int srmmu::write_register(const uint32_t &addr, const uint32_t &data){

  int rn = (addr >> 8);
  uint32_t d = data;

  if(rn >= NUM_REGS) {
    printf("MMU ERROR: Invalid Register Access\n");
    
    mmu_regs[4] = addr; //!Write fault address
    if(mmu_regs[3])     //!Check if overflow 
       mmu_regs[3] |= 1;
      
    mmu_regs[3] |= INTERNAL_ERROR | 2; //!Write Internal Error Status

    return 1;
  }
  //!Don't write the reserved bits of Configuration Register
  if(rn == 0) {
    mmu_regs[0] = (d & MMU_CTRL_MASK);
    return 0;
  }

  if(rn != 3) //!Writes to Fault Status Register are ignored
    mmu_regs[rn] = d;
  return 0;
}

void srmmu::probe(const uint32_t& addr){
  //TODO:Implement this
}

void srmmu::flush(const uint32_t& addr) {
  //!FIXME: kernel always flush entire cache ?
  //pdc_cache->flush(4);
}

ac_tlm_rsp_status srmmu::srmmu_write(const uint32_t &addr , const uint32_t &data){ 

  uint32_t paddr = 0x0;
  int acc_type = 0;
  uint32_t data_;

  ac_tlm_req request;
  ac_tlm_rsp response;

  switch(asi) {
    case ASI_LEON_MMUFLUSH:
    case ASI_M_FLUSH_PROBE:
      dbg_printf("MMU Flush %x\n",addr >> 8);
      flush(addr);
      return SUCCESS;
      break;
    case ASI_LEON_MMUREGS:
    case ASI_M_MMUREGS:
      data_ = data;
      //!Data came from processor with reversed endian so we need to correct the endian before write
      endian_swap32(data_);
      dbg_printf("MMU Write Register: %d <- 0x%x\n",(addr >> 8), data);
      if(write_register(addr, data_) == 0)
        return SUCCESS;
      else
        return ERROR;
      break;
    case ASI_LEON_BYPASS:
      case ASI_M_BYPASS:
      dbg_printf("MMU bypassed at 0x%.8x <- 0x%x\n",addr, data);
      response = write_phys_addr(addr, data);
      return SUCCESS;
    case ASI_LEON_IFLUSH:      //! 0x10 Unsuported 
      dbg_printf("Instruction Cache Flush <- 0x%.8x\n", addr);
      return SUCCESS;   
    case ASI_LEON_DFLUSH:      //! 0x11 Unsuported
      dbg_printf("Data Cache Flush <- 0x%.8x\n", addr);
      return SUCCESS;
    case ASI_LEON_DCACHE_MISS: //! 0x01 
      dbg_printf("Forced Cache Miss <- 0x%.8x\n", addr);
      response = write_phys_addr(addr, data);
      return SUCCESS;
    case ASI_M_IODIAG:         //! 0x07 
    case ASI_LEON_DIAG_ACCESS: //! 0x1D
      dbg_printf("MMU Diagnostic access <- 0x%.8x\n", addr);
      return SUCCESS;
    case ASI_M_TXTC_TAG:       //! 0x0C
      dbg_printf("MMU Instruction Cache Tag <- 0x%.8x\n", addr);
      return SUCCESS;
    case ASI_M_TXTC_DATA:      //! 0x0D
      dbg_printf("MMU Instruction Cache Data <- 0x%.8x\n", addr);
      return SUCCESS;
    case ASI_M_DATAC_TAG:      //! 0x0E
      dbg_printf("MMU Data Cache Tag <-  0x%.8x\n", addr);
      return SUCCESS;
    case ASI_M_DATAC_DATA:     //! 0x0F
      dbg_printf("MMU Data Cache Data <- 0x%.8x\n", addr, asi);
      return SUCCESS;
    case ASI_LEON_CACHEREGS: //!0x2
      switch(addr){
        default:
        case 0x0:
        case 0xC://read only
        case 0x8://read only
         dbg_printf("Write CACHE Control Registers\n");
        break;
      }
      return SUCCESS;
    case ASI_M_USERTXT:
      acc_type = 6;
      if(translate(addr , paddr, acc_type) == 0){
        response = write_phys_addr(paddr, data);
        return response.status;
      } else {
        return ERROR;
      } 
    case ASI_M_KERNELTXT:
      acc_type = 7;
      if(translate(addr , paddr, acc_type) == 0){
        response = write_phys_addr(paddr, data);
        return response.status;
      }else {
        return ERROR;
      }
    case ASI_M_USERDATA:
      acc_type = 4;     
      if(translate(addr , paddr, acc_type) == 0){
        response = write_phys_addr(paddr, data);
        return response.status;
      }else {
        return ERROR;
      } 
    case ASI_M_KERNELDATA:
      acc_type = 5;     
      if(translate(addr , paddr, acc_type) == 0){
        response = write_phys_addr(paddr, data);
        return response.status;
      }else {
        return ERROR;
      }
    default:
      fprintf(stderr, "MMU ERROR: Can't handle ASI: 0x%x\n",asi);
      mmu_regs[4] = addr;
      if(mmu_regs[3]) //!Check if overflow
        mmu_regs[3] |= 1;
        mmu_regs[3] |= INTERNAL_ERROR | 2;  //!Write internal error

      return ERROR;
  }
}

ac_tlm_rsp_status srmmu::srmmu_read(const uint32_t &addr , uint32_t &data){

  uint32_t paddr = 0x0;
  int acc_type = 0;
  ac_tlm_req request;
  ac_tlm_rsp response;

  switch(asi){
    case ASI_LEON_MMUFLUSH: //! 0x18
    case ASI_M_FLUSH_PROBE: //! 0x03
    dbg_printf("MMU Probe %x\n",addr >> 8);
    //probe(addr);
    response.data = 0x0;
    return SUCCESS;   
    case ASI_LEON_MMUREGS:
      if(read_register(addr, data) == 0) {
         dbg_printf("MMU Read Register: %d -> 0x%x\n",(addr >> 8), data);
         //!Now we need to reverse endian because processor reverse it
         endian_swap32(data);
         return SUCCESS;
      } else {
         return ERROR;
      }
    case ASI_LEON_BYPASS: //! 0x1C
    case ASI_M_BYPASS:    //! 0x20
      response = read_phys_addr(addr);
      data = response.data;
      dbg_printf("MMU bypassed at 0x%.8x -> 0x%x\n",addr, data);
      return SUCCESS;
    case ASI_LEON_IFLUSH:      //! 0x10 Unsuported 
      dbg_printf("Instruction Cache Flush -> 0x%.8x\n", addr);
      return SUCCESS;   
    case ASI_LEON_DFLUSH:      //! 0x11 Unsuported
      dbg_printf("Data Cache Flush -> 0x%.8x\n", addr);
      return SUCCESS;
    case ASI_LEON_DCACHE_MISS: //! 0x01 
      dbg_printf("Forced Cache Miss -> 0x%.8x\n", addr);
      response = read_phys_addr(addr);
      data = response.data;
      return SUCCESS;
    case ASI_M_IODIAG:         //! 0x07 
    case ASI_LEON_DIAG_ACCESS: //! 0x1D
      dbg_printf("MMU Diagnostic access  -> 0x%.8x\n", addr);
      return SUCCESS;
    case ASI_M_TXTC_TAG:       //! 0x0C
      dbg_printf("MMU Instruction Cache Tag -> 0x%.8x\n", addr);
      return SUCCESS;
    case ASI_M_TXTC_DATA:      //! 0x0D
      dbg_printf("MMU Instruction Cache Data -> 0x%.8x\n", addr);
      return SUCCESS;
    case ASI_M_DATAC_TAG:      //! 0x0E
      dbg_printf("MMU Data Cache Tag -> 0x%.8x\n", addr);
      return SUCCESS;
    case ASI_M_DATAC_DATA:     //! 0x0F
      dbg_printf("MMU Data Cache Data -> 0x%.8x asi=%x\n", addr, asi);
      return SUCCESS;
    case ASI_LEON_CACHEREGS://!0x2 Cache Control Registers
      switch(addr){
        //!TODO: Implement this if you use some cache
        default:
        case 0x0:
        case 0xC:
        case 0x8:
          dbg_printf("Read CACHE Control Registers\n");
          data = 0x0;
          break;
      }
      return SUCCESS;
    case ASI_M_USERTXT:      //! 0x8
      acc_type = 2;
      if(translate(addr , paddr, acc_type) == 0){
         response = read_phys_addr(paddr);
         data = response.data;
         return response.status;
      }else{
        return ERROR;
      } 
    case ASI_M_KERNELTXT:    //! 0x9
      acc_type = 3;
      if(translate(addr , paddr, acc_type) == 0){
        response = read_phys_addr(paddr);
        data = response.data;
        return response.status;
      }else{
        return ERROR;
      } 
    case ASI_M_USERDATA:     //! 0xA
      acc_type = 0;
      if(translate(addr , paddr, acc_type) == 0){
        response = read_phys_addr(paddr);
        data = response.data;
        return response.status;
      }else{
        return ERROR;
      } 
    case ASI_M_KERNELDATA :  //! 0xB
      acc_type = 1;
      if(translate(addr , paddr, acc_type) == 0){
         response = read_phys_addr(paddr);
         data = response.data;
         return response.status;
      }else{
        return ERROR;
      } 
    default:
      fprintf(stderr, "MMU ERROR: Can't handle ASI: 0x%x\n",asi);
      mmu_regs[4] = addr;
      if(mmu_regs[3]) //!Check if overflow
        mmu_regs[3] |= 1;
        mmu_regs[3] |= INTERNAL_ERROR | 2;  //!Write internal error

      return ERROR;
  }
} 

} //namespace sparc