/**
 * \file      leon3_trap_selector.cpp
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


#include "leon3_trap_selector.H"

//!Sparc Traps default constructor
leon3_trap_selector::leon3_trap_selector() : 
    trap_mode(0),
    reset_trap(0),
    data_store_error(0),
    instruction_access_error(0),
    r_register_access_error(0),
    instruction_access_exception(0),
    privileged_instruction(0),
    illegal_instruction(0),
    fp_disabled(0),
    cp_disabled(0),
    unimplemented_FLUSH(0),
    window_overflow(0),
    window_underflow(0),
    mem_address_not_aligned(0),
    fp_exception(0),
    cp_exception(0),
    data_access_error(0),
    data_access_exception(0),
    tag_overflow(0),
    division_by_zero(0),
    trap_instruction(0),
    interrupt_level(0) { }

//!Sparc traps default destructor
leon3_trap_selector::~leon3_trap_selector() { }

void leon3_trap_selector::clean () {
    trap_mode                    = 0;
    reset_trap                   = 0;
    data_store_error             = 0;
    instruction_access_error     = 0;
    r_register_access_error      = 0;
    instruction_access_exception = 0;
    privileged_instruction       = 0;
    illegal_instruction          = 0;
    fp_disabled                  = 0;
    cp_disabled                  = 0;
    unimplemented_FLUSH          = 0;
    window_overflow              = 0;
    window_underflow             = 0;
    mem_address_not_aligned      = 0;
    fp_exception                 = 0;
    cp_exception                 = 0;
    data_access_error            = 0;
    data_access_exception        = 0;
    tag_overflow                 = 0;
    division_by_zero             = 0;
    trap_instruction             = 0;
    interrupt_level              = 0;
}

void leon3_trap_selector::set_reset_trap(bool b)
{ 
    reset_trap = b; 
    trap_mode  = 1;
}

void leon3_trap_selector::set_data_store_error(bool b)
{ 
    data_store_error = b; 
    trap_mode        = 1;
}

void leon3_trap_selector::set_instruction_access_error(bool b)  
{ 
    instruction_access_error = b; 
    trap_mode                = 1;
}

void leon3_trap_selector::set_r_register_access_error(bool b)   
{ 
    r_register_access_error = b; 
    trap_mode               = 1;
}

void leon3_trap_selector::set_instruction_access_exception(bool b)  
{ 
    instruction_access_exception = b; 
    trap_mode                    = 1;
}

void leon3_trap_selector::set_privileged_instruction(bool b)        
{ 
    privileged_instruction = b;
    trap_mode              = 1; 
}

void leon3_trap_selector::set_illegal_instruction(bool b)       
{ 
    illegal_instruction = b; 
    trap_mode           = 1;
}

void leon3_trap_selector::set_fp_disabled(bool b)           
{ 
    fp_disabled = b; 
    trap_mode = 1;
}

void leon3_trap_selector::set_cp_disabled(bool b)           
{ 
    cp_disabled = b; 
    trap_mode   = 1;
}

void leon3_trap_selector::set_unimplemented_FLUSH(bool b)       
{
    unimplemented_FLUSH = b; 
    trap_mode           = 1;
}

void leon3_trap_selector::set_window_overflow(bool b)           
{ 
    window_overflow = b;
    trap_mode       = 1; 
}

void leon3_trap_selector::set_window_underflow(bool b)      
{ 
    window_underflow = b;
    trap_mode        = 1; 
}

void leon3_trap_selector::set_mem_address_not_aligned(bool b)       
{ 
    mem_address_not_aligned = b; 
    trap_mode               = 1;
}

void leon3_trap_selector::set_fp_exception(bool b)          
{ 
    fp_exception = b; 
    trap_mode    = 1;
}

void leon3_trap_selector::set_cp_exception(bool b)          
{ 
    cp_exception = b;
    trap_mode    = 1; 
}

void leon3_trap_selector::set_data_access_error(bool b)     
{ 
    data_access_error = b; 
    trap_mode         = 1;
}

void leon3_trap_selector::set_data_access_exception(bool b)     
{ 
    data_access_exception = b;
    trap_mode             = 1; 
}

void leon3_trap_selector::set_tag_overflow(bool b)          
{ 
    tag_overflow = b; 
    trap_mode    = 1;
}

void leon3_trap_selector::set_division_by_zero(bool b)      
{ 
    division_by_zero = b; 
    trap_mode        = 1;
}

void leon3_trap_selector::set_trap_instruction(unsigned char n)     
{ 
    trap_instruction = n;
    trap_mode        = 1; 
}

void leon3_trap_selector::set_interrupt_level(unsigned char n)          
{ 
    interrupt_level = n; 
    trap_mode       = 1;
}

unsigned char leon3_trap_selector::get_tt() 
{
    unsigned char tt;

    if (leon3_trap_selector::reset_trap)    tt = RESET;
    else if (data_store_error)              tt = DATA_STORE_ERROR;
    else if (instruction_access_error)      tt = INSTRUCTION_ACCESS_ERROR;
    else if (r_register_access_error)       tt = R_REGISTER_ACCESS_ERROR;
    else if (instruction_access_exception)  tt = INSTRUCTION_ACCESS_EXCEPTION;
    else if (privileged_instruction)        tt = PRIVILEGED_INSTRUCTION;
    else if (illegal_instruction)           tt = ILLEGAL_INSTRUCTION;
    else if (fp_disabled)                   tt = FP_DISABLED;
    else if (cp_disabled)                   tt = CP_DISABLED;
    else if (unimplemented_FLUSH)           tt = UNIMPLEMENTED_FLUSH;
    else if (window_overflow)               tt = WINDOW_OVERFLOW;
    else if (window_underflow)              tt = WINDOW_UNDERFLOW;
    else if (mem_address_not_aligned)       tt = MEM_ADDRESS_NOT_ALIGNED;
    else if (fp_exception)                  tt = FP_EXCEPTION;
    else if (cp_exception)                  tt = CP_EXCEPTION;
    else if (data_access_error)             tt = DATA_ACCESS_ERROR;
    else if (data_access_exception)         tt = DATA_ACCESS_EXCEPTION;
    else if (tag_overflow)                  tt = TAG_OVERFLOW;
    else if (division_by_zero)              tt = DIVISION_BY_ZERO;
    else if (trap_instruction)              tt = TRAP_INSTRUCTION + (trap_instruction); 
    else if (interrupt_level)               tt = 0x10 + (interrupt_level);

    return (tt);
}

//!Check if processor is in trap mode
bool leon3_trap_selector::is_trap_mode()
{ 
    return trap_mode;
}

//TODO
void leon3_trap_selector::dump_trap_table(){ }
