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
	
	write_hard(ADDR_START,1);
	
	int start = 0;
	
	start = read_hard(ADDR_START);
	
	cout << "start bit is " << start << endl;
	
	unsigned char response2[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	
	read_bram(10, response2, 10);
	
	for (int i = 0; i < 10; ++i) 
	{
        	std::cout << static_cast<int>(response2[i]) << " ";  // Convert to int for proper printing
	}
	

 
}

void Soft::write_bram(sc_dt::uint64 addr,unsigned char *val,int length)
{
	pl_t pl;
	
	offset += sc_core::sc_time(DELAY, sc_core::SC_NS);
	
	pl.set_data_length(length);
	pl.set_address(addr);
	pl.set_data_ptr(val);
	pl.set_command(tlm::TLM_WRITE_COMMAND);
	pl.set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);
	interconnect_socket->b_transport(pl,offset);
}

void Soft::read_bram(sc_dt::uint64 addr, unsigned char *val, int length)
{
	offset += sc_core::sc_time(DELAY, sc_core::SC_NS);
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

void Soft::write_hard(sc_dt::uint64 addr,int val)
{
    	pl_t pl;
    	unsigned char buf[4];
    	toUchar(buf,val); 	
    	pl.set_address(VP_ADDR_IP_HARD_L + addr);
    	pl.set_data_length(1);
    	pl.set_data_ptr(buf);
    	pl.set_command(tlm::TLM_WRITE_COMMAND);
    	pl.set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);
    	interconnect_socket->b_transport(pl,offset);
}

int Soft::read_hard(sc_dt::uint64 addr)
{
    	pl_t pl;
    	unsigned char buf[4];
   	pl.set_address(VP_ADDR_IP_HARD_L + addr);
    	pl.set_data_length(1);
    	pl.set_data_ptr(buf);
    	pl.set_command(tlm::TLM_READ_COMMAND);
    	pl.set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);
    	sc_core::sc_time offset = sc_core::SC_ZERO_TIME;
    	interconnect_socket->b_transport(pl,offset);
    	return toInt(buf);
}
