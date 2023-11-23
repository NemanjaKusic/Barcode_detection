#ifndef _IP_HARD_H_
#define _IP_HARD_H_

#include <iostream>
#include <systemc>

#include <tlm>
#include <tlm_utils/simple_target_socket.h>
#include <tlm_utils/simple_initiator_socket.h>

class Ip_hard : public sc_core::sc_module
{
	public:
		Ip_hard(sc_core::sc_module_name);
		~Ip_hard();
	
		tlm_utils::simple_target_socket<Ip_hard> interconnect_socket;
		tlm_utils::simple_initiator_socket<Ip_hard> bram_socket;
};

#endif 
