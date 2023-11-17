#include "interconnect.hpp"

Interconnect::Interconnect(sc_core::sc_module_name name)
  : sc_module(name)
{
	SC_REPORT_INFO("Interconnect", "Constructed.");
}

Interconnect::~Interconnect()
{
  SC_REPORT_INFO("Interconnect", "Destroyed.");
}
