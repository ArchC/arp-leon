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

#include "ac_tlm_signal_port.H"

/** 
* Default constructor.
* \param port name.
*/
ac_tlm_signal_port::ac_tlm_signal_port(char const* nm) : name(nm) 
{ 
}

/**
* Send a signal to a device
* \param dev_id device identifier (default: 0)
* \param data signal value (default : 0)
* \return a response status ERROR or SUCCESS
*/
ac_tlm_rsp_status ac_tlm_signal_port::signal(uint32_t dev_id, uint32_t data)
{
  ac_tlm_req req;
  ac_tlm_rsp rsp;

  req.dev_id = dev_id; 
  req.type   = WRITE; //!signals are write only
  req.data   = data;

  rsp = (*this)->transport(req);
  
  return rsp.status;
}

string ac_tlm_signal_port::get_name() const {
  return name;
}

/**
* Default destructor.
*/
ac_tlm_signal_port::~ac_tlm_signal_port() {}
