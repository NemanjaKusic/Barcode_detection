#include "bram.hpp"

Bram::Bram(sc_core::sc_module_name name) : sc_module(name)
{
	bram_socket_1.register_b_transport(this, &Bram::b_transport);
	SC_REPORT_INFO("BRAM", "Constructed.");
	
	mem.reserve(10);
}

Bram::~Bram()
{
	SC_REPORT_INFO("BRAM", "Destroyed.");
}

void Bram::b_transport(pl_t &pl, sc_core::sc_time &offset)
{
	tlm::tlm_command cmd = pl.get_command();
	sc_dt::uint64 addr = pl.get_address();
	unsigned int len = pl.get_data_length();
	unsigned char *buf = pl.get_data_ptr();
	
	switch(cmd)
	{
		case tlm::TLM_WRITE_COMMAND:
			for (unsigned int i = 0; i < len; ++i)
			{
		  		mem[addr++] = buf[i];
			}
			pl.set_response_status( tlm::TLM_OK_RESPONSE );
			
			offset += sc_core::sc_time(10, sc_core::SC_NS);
			
			SC_REPORT_INFO("BRAM", "Array stored in memory.");
			break;
	
		case tlm::TLM_READ_COMMAND:
			for (unsigned int i = 0; i < len; ++i)
			{
		  		buf[i] = mem[addr++];
			}
			pl.set_response_status( tlm::TLM_OK_RESPONSE );
			
			offset += sc_core::sc_time(10, sc_core::SC_NS);
			
			SC_REPORT_INFO("BRAM", "Array extracted from memory.");
			break;
	
		default:
			pl.set_response_status( tlm::TLM_COMMAND_ERROR_RESPONSE );
			offset += sc_core::sc_time(10, sc_core::SC_NS);
	}

}
