/******************************************************
 * Architecture Module header file.                   *
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
 

#ifndef  _LEON3_H
#define  _LEON3_H

#include "systemc.h"
#include "ac_module.H"
#include "ac_utils.H"
#include "leon3_parms.H"
#include "leon3_arch.H"
#include "leon3_isa.H"
#include "ac_tlm_intr_port.H"
#include "leon3_intr_handlers.H"


class leon3: public ac_module, public leon3_arch {
private:
  typedef cache_item<leon3_parms::AC_DEC_FIELD_NUMBER> cache_item_t;
  typedef ac_instr<leon3_parms::AC_DEC_FIELD_NUMBER> ac_instr_t;
public:

  unsigned bhv_pc;
 

  bool has_delayed_load;
  char* delayed_load_program;

  leon3_parms::leon3_isa ISA;
  leon3_intp_handler intp_hnd;
  ac_tlm_intr_port intp;

  unsigned id;

  bool start_up;
  unsigned* instr_dec;
  ac_instr_t* instr_vec;

  //!Behavior execution method.
  void behavior();

 
  SC_HAS_PROCESS( leon3 );

  //!Constructor.
  leon3( sc_module_name name_, uint32_t cpu_id_ ): ac_module(name_), leon3_arch(), ISA(*this), intp_hnd(*this), intp("intp", intp_hnd) {

    SC_THREAD( behavior );
    bhv_pc = 0; 
    has_delayed_load = false;
    cpu_id = cpu_id_; //!CPU identifier
    start_up=1;
    id = 1;

  }

  unsigned get_ac_pc();

  void set_ac_pc( unsigned int value );

  virtual void PrintStat();

  void init(int ac, char* av[]);

  void init();

  void load(char* program);

  void delayed_load(char* program);

  void stop(int status = 0);

  virtual ~leon3() {};

};
#endif  //_LEON3_H

