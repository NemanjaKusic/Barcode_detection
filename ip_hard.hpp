#ifndef _IP_HARD_H_
#define _IP_HARD_H_

#include <iostream>
#include <systemc>

class Ip_hard : public sc_core::sc_module
{
public:
	Ip_hard(sc_core::sc_module_name);
	~Ip_hard();
};

#endif 
