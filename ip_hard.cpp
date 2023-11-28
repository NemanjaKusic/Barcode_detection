#include "ip_hard.hpp"

Ip_hard::Ip_hard(sc_core::sc_module_name name): sc_module(name), ready(1)
{
	interconnect_socket.register_b_transport(this, &Ip_hard::b_transport);
		
	SC_REPORT_INFO("IP_HARD", "Constructed.");
}

Ip_hard::~Ip_hard()
{
	SC_REPORT_INFO("IP_HARD", "Destroyed.");
}

void Ip_hard::b_transport(pl_t &pl, sc_core::sc_time &offset)
{
	cout << endl <<"flag recieved to/extracted from ip_hard" << endl;
	
	tlm::tlm_command cmd = pl.get_command();
 	sc_dt::uint64 addr = pl.get_address();
	unsigned int len = pl.get_data_length();
 	unsigned char *buf = pl.get_data_ptr();
 	pl.set_response_status( tlm::TLM_OK_RESPONSE );
 	
	switch(cmd)
 	{	
 	case tlm::TLM_WRITE_COMMAND:
      		switch(addr)
        	{
			case ADDR_START:
				start = toInt(buf);
				cout << endl << "start bit = " << start << endl;
				easy_function(offset);
				break;
			case ADDR_READY:
				ready = toInt(buf);
				cout << endl << "ready bit = " << ready << endl;
				break;
			default:
				pl.set_response_status( tlm::TLM_ADDRESS_ERROR_RESPONSE );
				cout << "Wrong address" << endl;
		}
      		
      		break;
	case tlm::TLM_READ_COMMAND:
		switch(addr)
		{
			case ADDR_READY:
				toUchar(buf, ready);
		  		break;
			case ADDR_START:
				toUchar(buf, start);
		  		break;
			default:
				pl.set_response_status( tlm::TLM_ADDRESS_ERROR_RESPONSE );
		}
		
		break;
		default:
		pl.set_response_status( tlm::TLM_COMMAND_ERROR_RESPONSE );
		cout << "Wrong command" << endl;
	}
	
	offset += sc_core::sc_time(DELAY, sc_core::SC_NS);
}

void Ip_hard::easy_function(sc_core::sc_time &){
	if(start == 1 && ready == 1)
	{
		cout << "easy function started" << endl;
		
		unsigned char myArray[10];
	
		read_bram(0, myArray, 10);
	
		for(int i=0; i < 10; i++ )
		{
			myArray[i] += 1;
		}
	
		
		
		write_bram(10, myArray, 10);
		ready = 0;
		
		cout << "easy function finished" << endl;
	}
}

void Ip_hard::write_bram(sc_dt::uint64 addr,unsigned char *val,int length)
{
	pl_t pl;
	
	offset += sc_core::sc_time(DELAY, sc_core::SC_NS);
	
	pl.set_data_length(length);
	pl.set_address(addr);
	pl.set_data_ptr(val);
	pl.set_command(tlm::TLM_WRITE_COMMAND);
	pl.set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);
	bram_socket->b_transport(pl,offset);
}

void Ip_hard::read_bram(sc_dt::uint64 addr, unsigned char *val, int length)
{
	offset += sc_core::sc_time(DELAY, sc_core::SC_NS);
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


