#ifndef _BRAM_HPP_
#define _BRAM_HPP_

#include <systemc>
#include <iostream>

class Bram : public sc_core::sc_module
{
	public:
		Bram (sc_core::sc_module_name name);
		~Bram();
};

#endif
