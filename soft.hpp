#ifndef SOFT_HPP_ 
#define SOFT_HPP_

#include <iostream>
#include <systemc>

#include <tlm>
#include <tlm_utils/simple_initiator_socket.h>

class Soft : public sc_core::sc_module
{
	public:
	    Soft(sc_core::sc_module_name name);
	    ~Soft();
	    
	    tlm_utils::simple_initiator_socket<Soft> interconnect_socket;
};

#endif
