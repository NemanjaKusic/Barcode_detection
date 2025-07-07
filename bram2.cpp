#include "bram2.hpp"

Bram2::Bram2(sc_core::sc_module_name name) : sc_module(name)
{
	bram2_socket_1.register_b_transport(this, &Bram2::b_transport);
	bram2_socket_2.register_b_transport(this, &Bram2::b_transport);
	SC_REPORT_INFO("BRAM2", "Constructed.");
	
	mem.reserve(BRAM_SIZE);
}

Bram2::~Bram2()
{
	SC_REPORT_INFO("BRAM2", "Destroyed.");
}

void Bram2::b_transport(pl_t &pl, sc_core::sc_time &offset)
{
	tlm::tlm_command cmd = pl.get_command();
	sc_dt::uint64 addr = pl.get_address();
	unsigned int len = pl.get_data_length();
	unsigned char *buf = pl.get_data_ptr();
	
	switch(cmd)
	{
		case tlm::TLM_WRITE_COMMAND:
			for (unsigned int i = 0; i < len; i+=2)
			{
		  		//mem[addr++] = buf[i];
		  		mem[addr++] = (static_cast<signed short>(buf[2*i]) << 8) | buf[2*i + 1];
			}
			pl.set_response_status( tlm::TLM_OK_RESPONSE );
			
			offset += sc_core::sc_time(DELAY, sc_core::SC_NS);
			
			//SC_REPORT_INFO("BRAM", "Array stored in memory.");
			break;
	
		case tlm::TLM_READ_COMMAND:
			unsigned char buf2[2];
			for (unsigned int i = 0; i < len; i+=2)
			{
				shortToUchar(buf2,mem[addr++]);
				buf[i] = buf2[0];
				buf[i+1] = buf2[1];
		  		//buf[i] = mem[addr++];
			}
			pl.set_response_status( tlm::TLM_OK_RESPONSE );
			
			offset += sc_core::sc_time(DELAY, sc_core::SC_NS);
			
			//SC_REPORT_INFO("BRAM", "Array extracted from memory.");
			break;
	
		default:
			pl.set_response_status( tlm::TLM_COMMAND_ERROR_RESPONSE );
			offset += sc_core::sc_time(DELAY, sc_core::SC_NS);
	}

}
