#ifndef ADDR_HPP
#define ADDR_HPP

#include <systemc>
#include <tlm>

typedef tlm::tlm_base_protocol_types::tlm_payload_type pl_t;
typedef tlm::tlm_base_protocol_types::tlm_phase_type ph_t;

#define DELAY 6

#define IMG_COLS 600
#define IMG_ROWS 452
#define STRIPE_COLS 600
#define STRIPE_ROWS 113 

//bram size is 1.5MB for now 
//#define BRAM_SIZE 0x16E360 

//bram1 size is STRIPE_COLS * STRIPE_ROWS + BUS_WIDTH = 67832KB
//bram2 size is bram1 * 2 = 135664KB
#define BRAM_SIZE 0x108F8

#define BUS_WIDTH 32

//registers in ip_hard
#define ADDR_START 0x00
#define ADDR_READY 0x01
#define ADDR_X_Y 0x02

// locations for memory and ip 
#define VP_ADDR_BRAM_L 0x00000000
#define VP_ADDR_BRAM_H 0x00000000 + BRAM_SIZE
#define VP_ADDR_BRAM2_L 0x20000000
#define VP_ADDR_BRAM2_H 0x20000000 + BRAM_SIZE
#define VP_ADDR_IP_HARD_L 0x40000000
#define VP_ADDR_IP_HARD_H 0x40000002


#endif // ADDR_HPP
