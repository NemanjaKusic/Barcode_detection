#ifndef INTERCONNECT_HPP_
#define INTERCONNECT_HPP_

#include <iostream>
#include <systemc>

class Interconnect : public sc_core::sc_module
{
	public:
		Interconnect(sc_core::sc_module_name name);
		~Interconnect();
};

#endif
