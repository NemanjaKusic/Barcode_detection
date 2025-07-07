#ifndef VP_HPP_
#define VP_HPP_

#include <systemc>
#include <iostream>

#include "soft.hpp"
#include "interconnect.hpp"
#include "ip_hard.hpp"
#include "bram.hpp"
#include "bram2.hpp"

class Vp :  public sc_core::sc_module
{
	public:
		Vp(sc_core::sc_module_name name, int argc, char** argv);
		~Vp();

	protected:
		Soft soft;
		Interconnect interconnect;
		Ip_hard ip_hard;
		Bram bram;
		Bram2 bram2;		
};


#endif
