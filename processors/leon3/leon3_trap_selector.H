/**
 * \file      leon3_trap_selector.H
 * \author    Rogerio Alves Cardoso
 * 
 * \brief     Implements LEON 3 Trap Selector
 *            The ArchC Team
 *            http://www.archc.org/
 *
 *            Computer Systems Laboratory (LSC)
 *            IC-UNICAMP
 *            http://www.lsc.ic.unicamp.br/
 *
 * \brief     Implements a sparc trap helper
 *            
 *
 * \attention  
 *   Copyright (C) 2002-2014 --- The ArchC Team
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


#ifndef _LEON_3_TRAP_SELECTOR_
#define _LEON_3_TRAP_SELECTOR_

#define RESET                             0x00
#define DATA_STORE_ERROR                  0x2b
#define INSTRUCTION_ACCESS_MMU_MISS       0x3c
#define INSTRUCTION_ACCESS_ERROR          0x21
#define R_REGISTER_ACCESS_ERROR           0x20
#define INSTRUCTION_ACCESS_EXCEPTION      0x01
#define PRIVILEGED_INSTRUCTION            0x03
#define ILLEGAL_INSTRUCTION               0x02
#define FP_DISABLED                       0x04
#define CP_DISABLED                       0x24
#define UNIMPLEMENTED_FLUSH               0x25
#define WATCHPOINT_DETECTED               0x0b
#define WINDOW_OVERFLOW                   0x05
#define WINDOW_UNDERFLOW                  0x06
#define MEM_ADDRESS_NOT_ALIGNED           0x07
#define FP_EXCEPTION                      0x08
#define CP_EXCEPTION                      0x28
#define DATA_ACCESS_ERROR                 0x29
#define DATA_ACCESS_MMU_MISS              0x2c
#define DATA_ACCESS_EXCEPTION             0x09
#define TAG_OVERFLOW                      0x0a
#define DIVISION_BY_ZERO                  0x2a
#define TRAP_INSTRUCTION                  0x80  // 0x80 - 0xFF
//!Interrupt Level                        0x11 - 0x1F
//!Implementation Dependent Exception     0x60 - 0x7F

#include <stdio.h>

class leon3_trap_selector{

private:
  //!trap mode flag
  bool trap_mode;

  bool reset_trap;
  bool data_store_error;
  bool instruction_access_error;
  bool r_register_access_error;
  bool instruction_access_exception;
  bool privileged_instruction;
  bool illegal_instruction;
  bool fp_disabled;
  bool cp_disabled;
  bool unimplemented_FLUSH;
  bool window_overflow;
  bool window_underflow;
  bool mem_address_not_aligned;
  bool fp_exception;
  bool cp_exception;
  bool data_access_error;
  bool data_access_exception;
  bool tag_overflow;
  bool division_by_zero;
  unsigned char trap_instruction;
  unsigned char interrupt_level;

public:

  leon3_trap_selector();
  ~leon3_trap_selector();

  void clean ();

  void dump_trap_table();

  void set_reset_trap (bool b);
  void set_data_store_error (bool b);
  void set_instruction_access_error (bool b);
  void set_r_register_access_error (bool b);
  void set_instruction_access_exception (bool b);
  void set_privileged_instruction (bool b);
  void set_illegal_instruction (bool b);
  void set_fp_disabled (bool b);
  void set_cp_disabled (bool b);
  void set_unimplemented_FLUSH (bool b);
  void set_window_overflow (bool b);
  void set_window_underflow (bool b);
  void set_mem_address_not_aligned (bool b);
  void set_fp_exception (bool b);
  void set_cp_exception (bool b);
  void set_data_access_error (bool b);
  void set_data_access_exception (bool b);
  void set_tag_overflow (bool b);
  void set_division_by_zero (bool b);
  void set_trap_instruction (unsigned char n);
  void set_interrupt_level (unsigned char n);

  unsigned char get_tt ();
  bool is_trap_mode();
};
#endif
