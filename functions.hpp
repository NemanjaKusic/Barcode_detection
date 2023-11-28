#ifndef _FUNCTIONS_HPP
#define _FUNCTIONS_HPP

#define SC_INCLUDE_FX

#include <systemc>
#include <sysc/datatypes/fx/sc_fixed.h>
#include <iostream>
#include <string.h>
#include <bitset>
#include <math.h>
#include <string>
#include <systemc>
#include <tlm>

using namespace std;

int toInt(unsigned char *buf);

void toUchar(unsigned char *buf,int val);

#endif
