/******************************************************
 * ArchC Resources implementation file.               *
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
 

#include  "leon3_arch.H"
#include  "leon3_arch_ref.H"

//!/Default constructor.
leon3_arch_ref::leon3_arch_ref(leon3_arch& arch) : ac_arch_ref<leon3_parms::ac_word, leon3_parms::ac_Hword>(arch),
                                                   trap_selector(arch.trap_selector),
                                                   ack_intp(arch.ack_intp),
                                                   cpu_id(arch.cpu_id),
                                                   ac_pc(arch.ac_pc),
                                                   MEM(arch.MEM),   
                                                   intr_reg(arch.intr_reg),   
                                                   RB(arch.RB),   
                                                   REGS(arch.REGS),   
                                                   PSR(arch.PSR),   
                                                   FSR(arch.FSR),   
                                                   TBR(arch.TBR),   
                                                   WIM(arch.WIM),   
                                                   Y(arch.Y),   
                                                   ASR(arch.ASR),   
                                                   npc(arch.npc) { }

