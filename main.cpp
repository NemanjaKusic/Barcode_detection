#include "vp.hpp"

using namespace sc_core;

int sc_main(int argc, char* argv[])
{
    Vp vp("Virtual_Platform", argc, argv);
    sc_start(1000, SC_NS);
    
    return 0;
}  
