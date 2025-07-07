#include "vp.hpp"

Vp::Vp(sc_core::sc_module_name name, int argc, char** argv) :
	sc_module (name),
	soft("Soft", argc, argv),
	interconnect("Interconnect"),
	ip_hard("Ip_hard"),
	bram("Bram"),
	bram2("Bram2")
{
	soft.interconnect_socket.bind(interconnect.soft_socket);
	interconnect.bram_socket.bind(bram.bram_socket_1);
	interconnect.bram2_socket.bind(bram2.bram2_socket_1);
	interconnect.ip_hard_socket.bind(ip_hard.interconnect_socket);
	ip_hard.bram_socket.bind(bram.bram_socket_2);
	ip_hard.bram2_socket.bind(bram2.bram2_socket_2);
	
	SC_REPORT_INFO("Virtual Platform", "Constructed.");
}

Vp::~Vp()
{
 	SC_REPORT_INFO("Virtual Platform", "Destroyed.");
}

