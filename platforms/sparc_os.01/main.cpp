/******************************************************
 * This is the main file for the mips1 ArchC model    *
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
 

const char *project_name="leon3";
const char *project_file="leon3.ac";
const char *archc_version="2.2";
const char *archc_options="-ndc ";

#define WITH_SRMMU
//#define AC_DEBUG
#include  <systemc.h>
#include <stdio.h>
#include  "leon3.H"
#include  "ac_tlm_mem.h"
#include  "ac_tlm_router.h"
#include  "ac_tlm_apbuart.h"
#include  "ac_tlm_gptimer.h"
#include  "ac_tlm_irqmp.h"
#include  "pnp.h" //plug and play config
#include  "loader.h"//kernel loader
#include  "ac_stats_base.H"
#ifdef WITH_SRMMU
#include  "ac_tlm_srmmu.h"
#endif

//#define ENABLE_GDB

using user::ac_tlm_mem;
using user::ac_tlm_router;
using user::ac_tlm_apbuart;
using user::ac_tlm_gptimer;
using user::ac_tlm_irqmp;
#ifdef WITH_SRMMU
using user::ac_tlm_srmmu;
#endif

int sc_main(int ac, char *av[])
{
  sc_report_handler::set_actions("/IEEE_Std_1666/deprecated", SC_DO_NOTHING);
  char prog[256];

  /*Clock for peripherical 50MHZ*/
  sc_clock p_clock("p_clock", 4, SC_NS);
  cout <<"\t\t\tGENERATING LEON3 (SPARC V8) PLATFORM" << endl;
  cout <<"\t\t\t\t(with SRMMU Support)" << endl;
  cout << endl;
  //!  ISA simulator
  cout << "*Creating SPARC V8 Leon3 Processor" << endl;
  cout << endl;
  leon3 leon3_cpu0("leon3_cpu0");

  //! Platform IP's
  cout << "*Creating IP Devices:" << endl;

  cout << "Creating Router" << endl;
  ac_tlm_router router("router");

  cout << "Creating RAM Memory" << endl;
  ac_tlm_mem memory("memory",33554432U);//32MB

  cout << "Creating Grlib APB ROM" << endl;
  ac_tlm_mem apbrom("apbrom",4096U);//4KB

  cout << "Creating Grlib AHB ROM" << endl;
  ac_tlm_mem ahbrom("ahbrom",4096U);//4KB

  cout << "Creating Grlib PROM" << endl;
  ac_tlm_mem prom("prom",1048576U);//1MB

  cout << "Creating Grlib GPTIMER" << endl;
  ac_tlm_gptimer gptimer("gptimer",50);

  cout << "Creating Grlib IRQMP" << endl;
  ac_tlm_irqmp irqmp("irqmp");
#ifdef WITH_SRMMU
  cout << "Creating Grlib SRMMU" << endl;
  ac_tlm_srmmu srmmu("srmmu");
#endif

  cout << "Creating APBUART" << endl;
  cout << endl;
  ac_tlm_apbuart uart("uart");
  cout << endl;
  /*Clock connections with peripherials*/
  gptimer.clk(p_clock);
  irqmp.clk(p_clock);
 
  /*CPU interruption port*/
  irqmp.CPU_port[0](leon3_cpu0.intp);
  /*Connection between peripherials and IRQ Controller*/
  gptimer.IRQ_port(irqmp.target_export);
  uart.IRQ_port(irqmp.target_export);

#ifdef AC_DEBUG
  ac_trace("leon3_cpu0.trace");
#endif 

  /*Bind*/
#ifdef WITH_SRMMU
  leon3_cpu0.MEM_port(srmmu.target_export);
  srmmu.ROUTER_port(router.target_export);
  srmmu.CPU_port(leon3_cpu0.intp);
#else
  leon3_cpu0.MEM_port(router.target_export);
#endif
  router.MEM_port(memory.target_export);
  router.UART_port(uart.target_export);
  router.GPTIMER_port(gptimer.target_export);
  router.IRQMP_port(irqmp.target_export);
  router.AHBROM_port(ahbrom.target_export);
  router.APBROM_port(apbrom.target_export);
  router.PROM_port(prom.target_export);

  /*Configure Plug & Play and ROMS*/
  cout << endl;
  set_pnp(ahbrom, apbrom);
  /*Load kernel image on RAM memory*/
  cout << endl;

  cout << "program load > " << endl;
  scanf("%s", prog);
  load_elf(leon3_cpu0, memory, prog ,RAM_BASEADDR,33554432U);
  leon3_cpu0.set_instr_batch_size(1);
  leon3_cpu0.init();
  cerr << endl;

#ifdef ENABLE_GDB
  leon3_cpu0.enable_gdb();
#endif

  sc_start();

  leon3_cpu0.PrintStat();
  cerr << endl;

#ifdef AC_STATS
  //ac_stats_base::print_all_stats(std::cerr);
#endif 

#ifdef AC_DEBUG
  ac_close_trace();
#endif 

  return leon3_cpu0.ac_exit_status;
}