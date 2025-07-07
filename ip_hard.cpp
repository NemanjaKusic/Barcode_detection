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
		
		
		/*
		unsigned char *myArray = new unsigned char[IMG_ROWS * IMG_COLS];
		//unsigned char myArray[IMG_ROWS*IMG_COLS];
	
		read_bram(0, myArray, IMG_ROWS*IMG_COLS);
	
		for(int i=0; i < IMG_ROWS*IMG_COLS; i++ )
		{
			myArray[i] += 1;
		}
	
		
		
		write_bram(IMG_ROWS*IMG_COLS, myArray, IMG_ROWS*IMG_COLS);
		delete[] myArray;
		*/
		
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

		               	
		for (int i = 0; i < STRIPE_ROWS * IMG_COLS; i++)//loop for rows// 
		{
		        	offset += sc_core::sc_time(DELAY, sc_core::SC_NS);     //for i++
		        	
		        	offset += sc_core::sc_time(DELAY, sc_core::SC_NS);     //for next 2 if in parallel
				if(i < 601)
				{
					//in parallel
					offset += sc_core::sc_time(DELAY, sc_core::SC_NS);
					i += 2 * IMG_COLS;
					case_1 = 1;
				}
				if((i + 601) > (STRIPE_ROWS * IMG_COLS))
				{	
					//in parallel
					offset += sc_core::sc_time(DELAY, sc_core::SC_NS);
					i -= 2 * IMG_COLS;
					case_2 = 1;
				}
				
				offset += sc_core::sc_time(DELAY, sc_core::SC_NS);
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
				res = short(pixel) * short(ker[j-2]);
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
/*				

				for(unsigned char j=0; j<10; j++)
				{
					offset += sc_core::sc_time(DELAY, sc_core::SC_NS);
					if(j > 2)
					{
						sum += res;						
					}
					res = short(pixel) * short(ker[j-2]);
					pixel = BramArray[addr];
					addr = i + const1[j];
				}
				
				offset += sc_core::sc_time(DELAY, sc_core::SC_NS);
				sum += res;
*/
/*
					//in parallel
					offset += sc_core::sc_time(DELAY, sc_core::SC_NS);
					addr = i + const1[0];
					
					//in parallel
					//read from bram is 2 clk cycles
					offset += sc_core::sc_time(2 * DELAY, sc_core::SC_NS);
					pixel = BramArray[addr];
					addr = i + const1[1];
					
					//in parallel
					offset += sc_core::sc_time(DELAY, sc_core::SC_NS);
					res = short(pixel) * short(ker0); 
					pixel = BramArray[addr];
					addr = i + const1[2];
					
					//in parallel
					offset += sc_core::sc_time(DELAY, sc_core::SC_NS);
					sum += res;
					res = short(pixel) * short(ker1); 
					pixel = BramArray[addr];
					addr = i + const1[3];
					
					//in parallel
					offset += sc_core::sc_time(DELAY, sc_core::SC_NS);
					sum += res;
					res = short(pixel) * short(ker2); 
					pixel = BramArray[addr];
					addr = i + const1[5];
					
					//in parallel
					offset += sc_core::sc_time(DELAY, sc_core::SC_NS);
					sum += res;
					res = short(pixel) * short(ker3); 
					pixel = BramArray[addr];
					addr = i + const1[6];
					
					//in parallel
					offset += sc_core::sc_time(DELAY, sc_core::SC_NS);
					sum += res;
					res = short(pixel) * short(ker5); 
					pixel = BramArray[addr];
					addr = i + const1[7];
					
					//in parallel
					offset += sc_core::sc_time(DELAY, sc_core::SC_NS);
					sum += res;
					res = short(pixel) * short(ker6); 
					pixel = BramArray[addr];
					addr = i + const1[8];
					
					//in parallel
					offset += sc_core::sc_time(DELAY, sc_core::SC_NS);
					sum += res;
					res = short(pixel) * short(ker7); 
					pixel = BramArray[addr];
					//addr = i + const1[8];
					
					//in parallel
					offset += sc_core::sc_time(DELAY, sc_core::SC_NS);
					sum += res;
					res = short(pixel) * short(ker8); 
					//pixel = myArray[addr];
					//addr = i + const1[8];
					
					//in parallel
					offset += sc_core::sc_time(DELAY, sc_core::SC_NS);
					sum += res;
*/					
				/*	
					addr2 = i + const1[1];
					addr3 = i + const1[2];
					addr4 = i + const1[3];
					addr5 = i + const1[4];
					addr6 = i + const1[5];
					addr7 = i + const1[6];
					addr8 = i + const1[7];
					addr9 = i + const1[8];
				if(x_y)
				{
				
					//in parallel
					res1 = ((short)myArray[addr1] * (short)kernel1[0]);
					res2 = ((short)myArray[addr2] * (short)kernel1[1]);
					res3 = ((short)myArray[addr3] * (short)kernel1[2]);
					res4 = ((short)myArray[addr4] * (short)kernel1[3]);
					//res5 = ((short)myArray[addr5] * (short)kernel1[4]);
					res6 = ((short)myArray[addr6] * (short)kernel1[5]);
					res7 = ((short)myArray[addr7] * (short)kernel1[6]);
					res8 = ((short)myArray[addr8] * (short)kernel1[7]);
					res9 = ((short)myArray[addr9] * (short)kernel1[8]);
	
				}
				else
				{	
				
					//in parallel
					res1 = ((short)myArray[addr1] * (short)kernel2[0]);
					res2 = ((short)myArray[addr2] * (short)kernel2[1]);
					res3 = ((short)myArray[addr3] * (short)kernel2[2]);
					res4 = ((short)myArray[addr4] * (short)kernel2[3]);
					//res5 = ((short)myArray[addr5] * (short)kernel2[4]);
					res6 = ((short)myArray[addr6] * (short)kernel2[5]);
					res7 = ((short)myArray[addr7] * (short)kernel2[6]);
					res8 = ((short)myArray[addr8] * (short)kernel2[7]);
					res9 = ((short)myArray[addr9] * (short)kernel2[8]);

				}
				
					sum = ((res1+res2)+(res3+res4))+((res6+res7)+(res8+res9));	//res5 = 0
				*/	
				
				offset += sc_core::sc_time(DELAY, sc_core::SC_NS);	//for next 2 if in parallel
				if(case_1)
				{
					//in parallel
					offset += sc_core::sc_time(DELAY, sc_core::SC_NS);
					i -= 2 * IMG_COLS;
					case_1 = 0;
				}					
				if(case_2)
				{
					//in parallel
					offset += sc_core::sc_time(DELAY, sc_core::SC_NS);
					i += 2 * IMG_COLS;
					case_2 = 0;
				}
			
				//in parallel
				offset += sc_core::sc_time(DELAY, sc_core::SC_NS);
				output[0] = sum;
				if(x_y)
				{
					offset += sc_core::sc_time(DELAY, sc_core::SC_NS);	//one delay for addr, two for writing in b_transport
					//write_bram(STRIPE_ROWS*IMG_COLS + 2*i, output, 1);
					write_bram2(i, output, 1);
					
				}
				else
				{	
					offset += sc_core::sc_time(DELAY, sc_core::SC_NS);	//one delay for addr, two for writing in b_transport
					//write_bram(3 * STRIPE_ROWS*IMG_COLS + 2*i, output, 1);
					write_bram2(STRIPE_ROWS*IMG_COLS + i, output, 1);		
				}
				
				offset += sc_core::sc_time(DELAY, sc_core::SC_NS);
				sum = 0;
		}
/*	
		
		if(x_y)
		{
			write_bram(STRIPE_ROWS*IMG_COLS, output, STRIPE_ROWS*IMG_COLS);
		}
		else
		{	
			write_bram(3 * STRIPE_ROWS*IMG_COLS, output, STRIPE_ROWS*IMG_COLS);
		}
*/		
		//delete[] output;
		delete[] BramArray;
		ready = 1;
		
			
		cout << "sobel function finished" << endl;
	}
}

void Ip_hard::write_bram(sc_dt::uint64 addr,short *val,int length)
{
/*
	pl_t pl;
	//offset += sc_core::sc_time(DELAY, sc_core::SC_NS);
	offset += sc_core::sc_time(11 * DELAY , sc_core::SC_NS);
	
	int div_four = 0;
	unsigned char buf1[4];
	
	while((2*length) % 4)
	{
		length++;
		div_four += 2;
	}
	
	//unsigned char *out = new unsigned char(2 * IMG_ROWS*IMG_COLS);
	unsigned char *out = static_cast<unsigned char*>(malloc(2 * length));

	unsigned char temp[2];
	for(int i = 0; i < (2*length) - div_four; i+=2)
	{
		shortToUchar(temp,val[i/2]);
		out[i] = temp[0];
		out[i + 1] = temp[1];
	}
	


	
  
    	for(int i = 2*length - div_four; i < 2*length; i++)
    	{
    		out[i] = 0;
    	}


   	pl.set_data_length(BUS_WIDTH);
    
    	for(int j = 0; j < ((2*length) / 4); j++)
    	{
    		int l = 0;
    	
    		for(int p = 4 * j; p < 4 + j * 4; p++)
    		{
    			buf1[l] = out[p];
    			l++;
    		}
  
		pl.set_address(VP_ADDR_BRAM_L + addr + j * 4);
		pl.set_data_ptr(buf1);
		pl.set_command(tlm::TLM_WRITE_COMMAND);
		pl.set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);
		bram_socket->b_transport(pl,offset);
		
    	}

	delete[] out;
*/
	pl_t pl;
	offset += sc_core::sc_time(DELAY, sc_core::SC_NS);
	
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

	offset += sc_core::sc_time(DELAY, sc_core::SC_NS);
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

/*
	pl_t pl;

	offset += sc_core::sc_time(11 * DELAY , sc_core::SC_NS);
	
	unsigned char buf1[4];
	int div_four = 4;
	
	//unsigned char *buf = new unsigned char[length];
	
	while(length % 4)
	{
		length++;
		div_four--;
	}
	
	pl.set_data_length(BUS_WIDTH);
	
	for(int i = 0; i < length / 4; i++)
	{
		pl.set_address(VP_ADDR_BRAM_L + addr + i * 4);
		pl.set_data_ptr(buf1);
		pl.set_command(tlm::TLM_READ_COMMAND);
		pl.set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);
		bram_socket->b_transport(pl,offset);
		
		int l = 0;
		
		if(i * 4 == (length - 4))
		{
			for(int p = 4 * i; p < div_four + i * 4; p++)
    			{
    				val[p] = buf1[l];
    				l++;
    			}
		}
		else
		{
			for(int p = 4 * i; p < 4 + i * 4; p++)
    			{
    				val[p] = buf1[l];
    				l++;
    			}
    			
		}
	}
*/
}

void Ip_hard::write_bram2(sc_dt::uint64 addr,short *val,int length)
{
	pl_t pl;
	offset += sc_core::sc_time(DELAY, sc_core::SC_NS);
	
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

