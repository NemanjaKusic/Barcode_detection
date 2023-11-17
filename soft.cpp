#include "soft.hpp"

Soft::Soft(sc_core::sc_module_name name) : sc_module(name)
{
	 SC_REPORT_INFO("Soft", "Constructed.");
}

Soft::~Soft()
{
    SC_REPORT_INFO("Soft", "Destroyed.");
}
