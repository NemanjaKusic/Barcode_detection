#ifndef SOFT_HPP_ 
#define SOFT_HPP_

#include <iostream>
#include <systemc>
 
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp> 
#include <opencv2/highgui.hpp> 

#include "addr.hpp"
#include "functions.hpp"

#include <tlm>
#include <tlm_utils/simple_initiator_socket.h>

using namespace cv;
using namespace std;

class Soft : public sc_core::sc_module
{
	public:
		Soft(sc_core::sc_module_name name, int argc, char** argv);
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
		void read_bram2(sc_dt::uint64 addr, short *val, int length);
		int read_hard(sc_dt::uint64 addr);
	    	void write_hard(sc_dt::uint64 addr,int val);
};

#endif
