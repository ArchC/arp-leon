#ifndef AC_TLM_IRQMP_H
#define AC_TLM_IRQMP_H

#include <systemc.h>
#include <stdio.h>
#include "ac_tlm_protocol.H"
#include "ac_tlm_port.H"

//!FIXME: Pass this value as constructor parameter
#define NUM_CPUS  1

#define INTERRUPT_LEVEL_OFFSET     0x0
#define INTERRUPT_PENDING_OFFSET   0x4
#define INTERRUPT_FORCE_OFFSET     0x8
#define INTERRUPT_CLEAR_OFFSET     0xC
#define INTERRUPT_MPSTATUS_OFFSET  0x10
#define INTERRUPT_BROADCAST_OFFSET 0x14
#define INTERRUPT_MASK_OFFSET      0x40
#define LEON_HARD_INT(x)           (1 << (x))
#define MAX_INTERRUPT 15
#define IRQMP_MAX_CPU 16

//#define DEBUG_MODEL

#ifdef DEBUG_MODEL
#  define dbg_message(args...) fprintf (stderr, "IRQMP DEBUG: " args)
#  define err_message(args...) fprintf (stderr, "IRQMP ERROR: " args)
#  define _d_(arg) arg
#else
#  define dbg_message(args...)
#  define err_message(args...) fprintf (stderr, "IRQMP ERROR: " args)
#  define _d_(arg)
#endif

//!Leon3 IRQ Controller Registers
typedef struct{

  unsigned int level;              //!Interrupt Level Register 
  unsigned int pending;            //!Interrupt Pending Register
  unsigned int force;              //!Interrupt Force
  unsigned int clear;              //!Interrupt Clear Register
  unsigned int status;             //!Processor Status
  unsigned int not_used[12];       //!Not Used
  unsigned int mask[IRQMP_MAX_CPU];//!Interrupt Mask Register

}LEON3_irqmp_registers;

using tlm::tlm_transport_if;

namespace user
{
  class ac_tlm_irqmp : 
  public sc_module,
  public ac_tlm_transport_if
 {
  private:
   //!IRQMP Registers
   LEON3_irqmp_registers *irqmp;
  
   /**
   * Interrupt Prioritization method
   * Select a interruption to be forward to processor based
   * on the interruption level (1 or 0) and the interruption priority (15 to 1)
   * @param unsigned value of the pending register
   * @param unsigned value of the level register 
   * @return the selected interruption level that will be forward to processor
   **/
   inline uint8_t intr_prioritization(unsigned &, unsigned &);

   /**
   * Interrupt send method
   * Send a selected interruption to processor n
   **/
   void irqmp_interrupt_send();

  public:

   //!IRQMP interface
   sc_export< ac_tlm_transport_if > target_export;
  
   //!CPU port interface
   sc_port< ac_tlm_transport_if > CPU_port[NUM_CPUS];

   sc_in<bool> clk;
   //sc_in<uint8_t> ack;

   /**
   * Default constructor
   * @param a string with the module name
   **/
   ac_tlm_irqmp(sc_module_name module_name);

   /**
   * Default destructor
   **/
   ~ac_tlm_irqmp();
   
   /**
   * IRQMP interrupt clear method
   * When the processor acknownledge a interruption, a signal is sent to IRQMP
   * and this method is activated cleaning the pending interruption
	* @param  a unsigned value with the acknoledge interruption to be cleaned
   **/
   void irqmp_interrupt_clear(uint32_t irl);

   /**
   * IRQMP write method
	* @param   a const unsigned value correponding to the register address
	* @param   a const unsigned value correponding to the data to be write
	* @return  a TLM response status SUCCESS or ERROR
   **/
   ac_tlm_rsp_status irqmp_write(const uint32_t & , const uint32_t &);

   /**
   * IRQMP read method
	* @param   a const unsigned value correponding to the register address
	* @param   a const unsigned value correponding to the readed data
	* @return  a TLM response status SUCCESS or ERROR
   **/
   ac_tlm_rsp_status irqmp_read(const uint32_t & , uint32_t &); 

   /**
   * Implementation of TLM transport method that
   * handle packets of the protocol doing apropriate actions.
   * This method must be implemented (required by SystemC TLM).
   * @param request is a received request packet
   * @return A response packet to be send
   */
   ac_tlm_rsp transport( const ac_tlm_req &request );  
 };
};
#endif /*AC_TLM_IRQMP_H*/
