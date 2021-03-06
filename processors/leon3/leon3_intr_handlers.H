/******************************************************
 * Interrupt Handlers header file.                    *
 * This file is automatically generated by ArchC      *
 * WITHOUT WARRANTY OF ANY KIND, either express       *
 * or implied.                                        *
 * For more information on ArchC, please visit:       *
 * http://www.archc.org                               *
 *                                                    *
 * The ArchC Team                                     *
 * Computer Systems Laboratory (LSC)                  *
 * IC-UNICAMP                                         *
 * http://www.lsc.ic.unicamp.br                       *
 ******************************************************/
 

#ifndef _LEON3_INTR_HANDLERS_H
#define _LEON3_INTR_HANDLERS_H

#include "ac_intr_handler.H"
#include "leon3_parms.H"
#include "leon3_arch.H"
#include "leon3_arch_ref.H"

class leon3_intp_handler :
  public ac_intr_handler,
  public leon3_arch_ref
{
 public:

  explicit leon3_intp_handler(leon3_arch& ref) : leon3_arch_ref(ref) {}

  void handle(uint32_t value);
  void execute_trap();

};


#endif // _LEON3_INTR_HANDLERS_H
