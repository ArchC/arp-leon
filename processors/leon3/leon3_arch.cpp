/******************************************************
 * ArchC Resources Implementation file.               *
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
 

#include "leon3_arch.H"

leon3_arch::leon3_arch() :
  ac_arch_dec_if<leon3_parms::ac_word, leon3_parms::ac_Hword>(leon3_parms::AC_MAX_BUFFER),
  trap_selector(),
  ac_pc("ac_pc", 0),
  MEM_port("MEM_port", 33554432U),
  MEM(*this, MEM_port),
  intr_reg("intr_reg", 0),
  RB("RB"),
  REGS("REGS"),
  PSR("PSR", 0),
  FSR("FSR", 0),
  TBR("TBR", 0),
  WIM("WIM", 0),
  Y("Y", 0),
  ASR("ASR"),
  npc("npc", 0) {

  ac_mt_endian = leon3_parms::AC_MATCH_ENDIAN;
  ac_tgt_endian = leon3_parms::AC_PROC_ENDIAN;

  IM = &MEM;
  APP_MEM = &MEM;

}

