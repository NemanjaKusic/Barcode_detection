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
	    
	protected:
		typedef tlm::tlm_base_protocol_types::tlm_payload_type pl_t;
		typedef tlm::tlm_base_protocol_types::tlm_phase_type ph_t;
		
		pl_t pl;
		sc_core::sc_time offset;
		
		void soft();
		void write_bram(sc_dt::uint64 addr,unsigned char *val,int length);
		void read_bram(sc_dt::uint64 addr, unsigned char *val, int length);
};

#endif
