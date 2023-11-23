#include "interconnect.hpp"

Interconnect::Interconnect(sc_core::sc_module_name name)
  : sc_module(name),  offset(sc_core::SC_ZERO_TIME)
{
	soft_socket.register_b_transport(this, &Interconnect::b_transport);
	
	SC_REPORT_INFO("Interconnect", "Constructed.");
}

Interconnect::~Interconnect()
{
  SC_REPORT_INFO("Interconnect", "Destroyed.");
}

void Interconnect::b_transport(pl_t &pl, sc_core::sc_time &offset)
{
	sc_dt::uint64 addr = pl.get_address(); 
	
	if (addr == 0)
	{
		offset += sc_core::sc_time(10, sc_core::SC_NS);
		SC_REPORT_INFO("Interconnect", "Active.");
		bram_socket->b_transport(pl, offset);
		



	}
	else
	{
		SC_REPORT_ERROR("Interconnect", "Wrong address.");
		pl.set_response_status ( tlm::TLM_ADDRESS_ERROR_RESPONSE );
		
	}
	
}
