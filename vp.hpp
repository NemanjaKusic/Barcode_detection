#ifndef VP_HPP_
#define VP_HPP_

#include <systemc>
#include <iostream>

#include "soft.hpp"
#include "interconnect.hpp"
#include "ip_hard.hpp"
#include "bram.hpp"

class Vp :  public sc_core::sc_module
{
	public:
		Vp(sc_core::sc_module_name name);
		~Vp();

	protected:
		Soft soft;
		Interconnect interconnect;
		Ip_hard ip_hard;
		Bram bram;		
};


#endif
