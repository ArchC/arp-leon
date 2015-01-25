/**
* \file      ac_tlm_signal_port.H
* \author    Rogerio Alves Cardoso
* 
* \author    The ArchC Team
*            http://www.archc.org/
*
*            Computer Systems Laboratory (LSC)
*            IC-UNICAMP
*            http://www.lsc.ic.unicamp.br/
* 
*
* \version   1.0
* 
* \brief     Defines the ArchC TLM signal port.
* 
* \attention Copyright (C) 2002-2014 --- The ArchC Team
* 
*/

#ifndef _AC_TLM_SIGNAL_PORT_H_
#define _AC_TLM_SIGNAL_PORT_H_

//!SystemC/C++ includes
#include <systemc.h>
#include <string>

//!ArchC includes
#include "ac_tlm_protocol.H"
#include "ac_tlm_dev_id.H"

// using statements
using std::string;

//!ArchC TLM signal port class.
class ac_tlm_signal_port : public sc_port<ac_tlm_transport_if>,
		    public ac_tlm_dev_id {
public:
  string name;
  uint32_t size;


  /** 
  * Default constructor.
  * 
  * \param name Port name.
  * 
  */
  explicit ac_tlm_signal_port(char const* name);

  /**
  * Default (virtual) destructor.
  * \return Nothing.
  */
  virtual ~ac_tlm_signal_port();

  /**
  * Send a signal to a device
  * \param dev_id device identifier (default: 0)
  * \param data signal value (default : 0)
  * \return a response status ERROR or SUCCESS
  */
  virtual ac_tlm_rsp_status signal(uint32_t dev_id = 0 , uint32_t data = 0);


  virtual string get_name() const;

};

#endif //_AC_TLM_SIGNAL_PORT_H_

