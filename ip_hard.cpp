#include "ip_hard.hpp"

Ip_hard::Ip_hard(sc_core::sc_module_name name): sc_module(name)
{
	SC_REPORT_INFO("IP_HARD", "Constructed.");
}

Ip_hard::~Ip_hard()
{
	SC_REPORT_INFO("IP_HARD", "Destroyed.");
}
