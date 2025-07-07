#ifndef _BRAM2_HPP_
#define _BRAM2_HPP_

#include <systemc>
#include <iostream>
#include <vector>

#include "addr.hpp"
#include "functions.hpp"

#include <tlm>
#include <tlm_utils/simple_target_socket.h>

class Bram2 : public sc_core::sc_module
{
	public:
		Bram2 (sc_core::sc_module_name name);
		~Bram2();
		
		tlm_utils::simple_target_socket<Bram2> bram2_socket_1;
		tlm_utils::simple_target_socket<Bram2> bram2_socket_2;
	protected:
		//typedef tlm::tlm_base_protocol_types::tlm_payload_type pl_t;
		//typedef tlm::tlm_base_protocol_types::tlm_phase_type ph_t;
	
		//pl_t pl;
		//sc_core::sc_time offset;
		void b_transport(pl_t &pl, sc_core::sc_time &);
		
		std::vector<short> mem;
};

#endif
