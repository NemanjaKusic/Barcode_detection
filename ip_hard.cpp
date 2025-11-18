#include "ip_hard.hpp"

using namespace sc_core;
using namespace sc_dt;
using namespace std;
using namespace tlm;

SC_HAS_PROCESS(Ip_hard);

Ip_hard::Ip_hard(sc_core::sc_module_name name): sc_module(name), ready(1)
{
	interconnect_socket.register_b_transport(this, &Ip_hard::b_transport);
		
	SC_REPORT_INFO("IP_HARD", "Constructed.");
}

Ip_hard::~Ip_hard()
{
	SC_REPORT_INFO("IP_HARD", "Destroyed.");
}

void Ip_hard::b_transport(pl_t &pl, sc_core::sc_time &offset)
{
	cout << endl <<"flag recieved to/extracted from ip_hard" << endl;
	
	tlm::tlm_command cmd = pl.get_command();
 	sc_dt::uint64 addr = pl.get_address();
	unsigned int len = pl.get_data_length();
 	unsigned char *buf = pl.get_data_ptr();
 	pl.set_response_status( tlm::TLM_OK_RESPONSE );
 	
	switch(cmd)
 	{	
 	case tlm::TLM_WRITE_COMMAND:
      		switch(addr)
        	{
			case ADDR_START:
				start = toInt(buf);
				cout << endl << "start bit = " << start << endl;
				sobel_function(offset);
				break;
			case ADDR_READY:
				ready = toInt(buf);
				cout << endl << "ready bit = " << ready << endl;
				break;
			case ADDR_X_Y:
				x_y = toInt(buf);
				cout << endl << "x_y bit = " << x_y << endl;
				break;
			default:
				pl.set_response_status( tlm::TLM_ADDRESS_ERROR_RESPONSE );
				cout << "Wrong address" << endl;
		}
      		
      		break;
	case tlm::TLM_READ_COMMAND:
		switch(addr)
		{
			case ADDR_READY:
				toUchar(buf, ready);
		  		break;
			case ADDR_START:
				toUchar(buf, start);
		  		break;
		  	case ADDR_X_Y:
				toUchar(buf, x_y);
		  		break;
			default:
				pl.set_response_status( tlm::TLM_ADDRESS_ERROR_RESPONSE );
		}
		
		break;
		default:
		pl.set_response_status( tlm::TLM_COMMAND_ERROR_RESPONSE );
		cout << "Wrong command" << endl;
	}
	
	offset += sc_core::sc_time(DELAY, sc_core::SC_NS);
}

void Ip_hard::sobel_function(sc_core::sc_time &offset){


	if(start == 1 && ready == 1)
	{
		ready = 0;
		offset += sc_core::sc_time(DELAY, sc_core::SC_NS);
	}
	else if(start == 0 && ready == 0)
	{
		cout << "sobel function started" << endl;
		
		
		unsigned char *BramArray = new unsigned char[STRIPE_ROWS * IMG_COLS];
		read_bram(0, BramArray, STRIPE_ROWS*IMG_COLS);
		
		//short *output = new short[STRIPE_ROWS * IMG_COLS];
		short output[1];
		
		int pos = 0;
		short sum = 0;

		short kernel1[9] = {-3, 0, 3, -10, 0, 10, -3, 0, 3};
		short kernel2[9] = {-3, -10, -3, 0, 0, 0, 3, 10, 3};
		
		short const1[9] = {-601, -600, -599, -1, 1, 599, 600, 601}; 
		
		unsigned char case_1 = 0;
		unsigned char case_2 = 0;
		
		//res4 will allways be 0 so it wont affect sum
		int addr = 0;
		short res = 0;
		//short ker0, ker1, ker2, ker3, ker5, ker6, ker7, ker8;
		short ker[8];
		unsigned char pixel = 0; 
		
		short ker_out;
		
		offset += sc_core::sc_time(DELAY, sc_core::SC_NS);
		               	
		for (int i = 0; i < STRIPE_ROWS * IMG_COLS; i++)//loop for rows// 
		{
		        	
		        	offset += sc_core::sc_time(DELAY, sc_core::SC_NS);     //for next 2 if in parallel
				if(i < 601)
				{
					//in parallel
					i += 2 * IMG_COLS;
					case_1 = 1;
				}
				if((i + 601) > (STRIPE_ROWS * IMG_COLS))
				{	
					//in parallel
					i -= 2 * IMG_COLS;
					case_2 = 1;
				}
				
				if(x_y)
				{
					//in parallel
					offset += sc_core::sc_time(DELAY, sc_core::SC_NS);
					ker[0] = kernel1[0];
					ker[1] = kernel1[1];
					ker[2] = kernel1[2];
					ker[3] = kernel1[3];
					ker[4] = kernel1[5];
					ker[5] = kernel1[6];
					ker[6] = kernel1[7];
					ker[7] = kernel1[8];
				}
				else
				{
					//in parallel
					offset += sc_core::sc_time(DELAY, sc_core::SC_NS);
					ker[0] = kernel2[0];
					ker[1] = kernel2[1];
					ker[2] = kernel2[2];
					ker[3] = kernel2[3];
					ker[4] = kernel2[5];
					ker[5] = kernel2[6];
					ker[6] = kernel2[7];
					ker[7] = kernel2[8];
				}
				
				unsigned char j=0;
				
				j_petlja: offset += sc_core::sc_time(DELAY, sc_core::SC_NS);
				if(j > 2)
				{
					sum += res;						
				}
				res = short(pixel) * ker_out;
				//res = short(pixel) * short(ker[j-2]);
				ker_out = ker[j-1];
				pixel = BramArray[addr];
				addr = i + const1[j];
				j++;
				
				if(j<10)
				{
					goto j_petlja;
				}
				else
				{
					goto kraj;
				}
				
				kraj: offset += sc_core::sc_time(DELAY, sc_core::SC_NS);
				sum += res;
	
				
				offset += sc_core::sc_time(DELAY, sc_core::SC_NS);	//for next 2 if in parallel
				if(case_1)
				{
					//in parallel
					i -= 2 * IMG_COLS;
					case_1 = 0;
				}					
				if(case_2)
				{
					//in parallel
					i += 2 * IMG_COLS;
					case_2 = 0;
				}
			
				//in parallel
				offset += sc_core::sc_time(DELAY, sc_core::SC_NS);
				output[0] = sum;

				write_bram2(i, output, 1);			
							
				sum = 0;
		}
		
		//delete[] output;
		delete[] BramArray;
		ready = 1;
		
			
		cout << "sobel function finished" << endl;
	}
}

void Ip_hard::write_bram(sc_dt::uint64 addr,short *val,int length)
{

	pl_t pl;
	
	//unsigned char *out = new unsigned char(2 * IMG_ROWS*IMG_COLS);
	unsigned char *out = static_cast<unsigned char*>(malloc(2 * STRIPE_ROWS * IMG_COLS));

	unsigned char buf[2];
	for(int i = 0; i < length; i++)
	{
		shortToUchar(buf,val[i]);
		out[2*i] = buf[0];
		out[2*i + 1] = buf[1];
	}
	
	pl.set_data_length(2*length);
	pl.set_address(addr);
	pl.set_data_ptr(out);
	pl.set_command(tlm::TLM_WRITE_COMMAND);
	pl.set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);
	bram_socket->b_transport(pl,offset);
	
	delete[] out;
}

void Ip_hard::read_bram(sc_dt::uint64 addr, unsigned char *val, int length)
{

	pl_t pl;
	
	//unsigned char buf[length];
	unsigned char *buf = new unsigned char[length];
	
	pl.set_address(addr);
	pl.set_data_length(length);
	pl.set_data_ptr(buf);
	pl.set_command(tlm::TLM_READ_COMMAND);
	pl.set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);
	bram_socket->b_transport(pl,offset);
	
	for(int i = 0; i < length; i++)
	{
		val[i] = buf[i];
	}
	
	delete[] buf;

}

void Ip_hard::write_bram2(sc_dt::uint64 addr,short *val,int length)
{
	pl_t pl;
	
	//unsigned char *out = new unsigned char(2 * IMG_ROWS*IMG_COLS);
	unsigned char *out = new unsigned char[2 * length];
	//unsigned char *out = static_cast<unsigned char*>(malloc(2 * length));

	unsigned char buf[2];
	for(int i = 0; i < length; i++)
	{
		shortToUchar(buf,val[i]);
		out[2*i] = buf[0];
		out[2*i + 1] = buf[1];
	}
	
	pl.set_data_length(2*length);
	pl.set_address(addr);
	pl.set_data_ptr(out);
	pl.set_command(tlm::TLM_WRITE_COMMAND);
	pl.set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);
	bram2_socket->b_transport(pl,offset);
	
	delete[] out;
	
}

