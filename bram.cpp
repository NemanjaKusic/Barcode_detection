#include "bram.hpp"

Bram::Bram(sc_core::sc_module_name name) : sc_module(name)
{
	SC_REPORT_INFO("BRAM", "Constructed.");
}

Bram::~Bram()
{
	SC_REPORT_INFO("BRAM", "Destroyed.");
}
