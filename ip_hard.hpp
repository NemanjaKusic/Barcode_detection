#ifndef _IP_HARD_H_
#define _IP_HARD_H_

#include <iostream>
#include <systemc>

#include "addr.hpp"
#include "functions.hpp"

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
		

		
	protected:
		typedef tlm::tlm_base_protocol_types::tlm_payload_type pl_t;
		typedef tlm::tlm_base_protocol_types::tlm_phase_type ph_t;
	
		pl_t pl;
		sc_core::sc_time offset;
		
		sc_dt::sc_uint<1> start;
		sc_dt::sc_uint<1> ready;
		sc_dt::sc_uint<1> x_y;
		
		void b_transport(pl_t&, sc_core::sc_time&);	
		void sobel_function(sc_core::sc_time &);
	
		void write_bram(sc_dt::uint64 addr,short *val,int length);
		void read_bram(sc_dt::uint64 addr, unsigned char *val, int length);
	
};

#endif 
