#ifndef SOFT_HPP_ 
#define SOFT_HPP_

#include <iostream>
#include <systemc>

class Soft : public sc_core::sc_module
{
	public:
	    Soft(sc_core::sc_module_name name);
	    ~Soft();
};

#endif
