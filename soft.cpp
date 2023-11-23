#include "soft.hpp"

SC_HAS_PROCESS(Soft);

Soft::Soft(sc_core::sc_module_name name) : sc_module(name),  offset(sc_core::SC_ZERO_TIME)
{
	 SC_THREAD(soft);
	 SC_REPORT_INFO("Soft", "Constructed.");
}

Soft::~Soft()
{
    SC_REPORT_INFO("Soft", "Destroyed.");
}

void Soft::soft()
{
	unsigned char example[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

	write_bram(0, example, 10);
	
	unsigned char response[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	
	read_bram(0, response, 10);
	
	std::cout << std::endl;
	
	for (int i = 0; i < 10; ++i) 
	{
        	std::cout << static_cast<int>(response[i]) << " ";  // Convert to int for proper printing
	}

    	std::cout << std::endl;
	

 
}

void Soft::write_bram(sc_dt::uint64 addr,unsigned char *val,int length)
{
	pl_t pl;
	
	offset += sc_core::sc_time(10, sc_core::SC_NS);
	
	pl.set_data_length(length);
	pl.set_address(addr);
	pl.set_data_ptr(val);
	pl.set_command(tlm::TLM_WRITE_COMMAND);
	pl.set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);
	interconnect_socket->b_transport(pl,offset);
}

void Soft::read_bram(sc_dt::uint64 addr, unsigned char *val, int length)
{
	offset += sc_core::sc_time(10, sc_core::SC_NS);
	pl_t pl;
	
	unsigned char buf[length];
	
	pl.set_address(addr);
	pl.set_data_length(length);
	pl.set_data_ptr(buf);
	pl.set_command(tlm::TLM_READ_COMMAND);
	pl.set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);
	interconnect_socket->b_transport(pl,offset);
	
	for(int i = 0; i < length; i++)
	{
		val[i] = buf[i];
	}
}
