#ifndef INTERCONNECT_HPP_
#define INTERCONNECT_HPP_

#include <iostream>
#include <systemc>
#include <string>

#include "addr.hpp"

#include <tlm>
#include <tlm_utils/simple_target_socket.h>
#include <tlm_utils/simple_initiator_socket.h>

using namespace sc_core;

class Interconnect : public sc_core::sc_module
{
	public:
		Interconnect(sc_core::sc_module_name name);
		~Interconnect();
		
		tlm_utils::simple_initiator_socket<Interconnect> bram_socket;
		tlm_utils::simple_initiator_socket<Interconnect> bram2_socket;
		tlm_utils::simple_initiator_socket<Interconnect> ip_hard_socket;
		tlm_utils::simple_target_socket<Interconnect> soft_socket;
		
	protected:
		typedef tlm::tlm_base_protocol_types::tlm_payload_type pl_t;
		typedef tlm::tlm_base_protocol_types::tlm_phase_type ph_t;
	
		pl_t pl;
		sc_core::sc_time offset;
		void b_transport(pl_t &pl, sc_core::sc_time &offset);
};

#endif
