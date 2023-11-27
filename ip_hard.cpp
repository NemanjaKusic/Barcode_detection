#include "ip_hard.hpp"

Ip_hard::Ip_hard(sc_core::sc_module_name name): sc_module(name)
{
	SC_REPORT_INFO("IP_HARD", "Constructed.");
}

Ip_hard::~Ip_hard()
{
	SC_REPORT_INFO("IP_HARD", "Destroyed.");
}

void Ip_hard::easy_function(){
	unsigned char myArray[10];
	
	read_bram(0, myArray, 10);
	
	for(int i=0; i < 10; i++ )
	{
		myArray[i] += 1;
	}
	
	write_bram(10, myArray, 10);
}

void Ip_hard::write_bram(sc_dt::uint64 addr,unsigned char *val,int length)
{
	pl_t pl;
	
	offset += sc_core::sc_time(10, sc_core::SC_NS);
	
	pl.set_data_length(length);
	pl.set_address(addr);
	pl.set_data_ptr(val);
	pl.set_command(tlm::TLM_WRITE_COMMAND);
	pl.set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);
	bram_socket->b_transport(pl,offset);
}

void Ip_hard::read_bram(sc_dt::uint64 addr, unsigned char *val, int length)
{
	offset += sc_core::sc_time(10, sc_core::SC_NS);
	pl_t pl;
	
	unsigned char buf[length];
	
	pl.set_address(addr);
	pl.set_data_length(length);
	pl.set_data_ptr(buf);
	pl.set_command(tlm::TLM_READ_COMMAND);
	pl.set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);
	bram_socket->b_transport(pl,offset);
	
	for(int i = 0; i < length; i++)
	{
		val[i] = buf[i];
	}
}


