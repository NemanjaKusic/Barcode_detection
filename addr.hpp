#ifndef ADDR_HPP
#define ADDR_HPP

#include <systemc>
#include <tlm>

#define DELAY 10

#define IMG_COLS 600
#define IMG_ROWS 450

//bram size is 1MB for now 
#define BRAM_SIZE 0xF4240 

//registers in ip_hard
#define ADDR_START 0x00
#define ADDR_READY 0x01

// locations for memory and ip 
#define VP_ADDR_BRAM_L 0x00000000
#define VP_ADDR_BRAM_H 0x00000000 + BRAM_SIZE
#define VP_ADDR_IP_HARD_L 0x40000000
#define VP_ADDR_IP_HARD_H 0x40000002


#endif // ADDR_HPP
