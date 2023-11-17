#include "vp.hpp"

Vp::Vp(sc_module_name name) :
	sc_module (name),
	soft("Soft"),
	interconnect("Interconnect"),
	ip_hard("Ip_hard"),
	bram("Bram")
{
	SC_REPORT_INFO("Virtual Platform", "Constructed.");
}

Vp::~Vp()
{
 	SC_REPORT_INFO("Virtual Platform", "Destroyed.");
}

