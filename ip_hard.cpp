#include "ip_hard.hpp"

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

void Ip_hard::sobel_function(sc_core::sc_time &){

	if(start == 1 && ready == 1)
	{
		ready = 0;
		offset += sc_core::sc_time(DELAY, sc_core::SC_NS);
	}
	else if(start == 0 && ready == 0)
	{
		cout << "easy function started" << endl;
		
		
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
		
		unsigned char *myArray = new unsigned char[STRIPE_ROWS * IMG_COLS];
		read_bram(0, myArray, STRIPE_ROWS*IMG_COLS);
		
		short *output = new short[STRIPE_ROWS * IMG_COLS];
		
		int pos = 0;
		short sum = 0;
/*
		char kernel1[3][3] = {
		{-3, 0, 3},
		{-10, 0 ,10},
		{-3, 0, 3}
		};

		char kernel2[3][3] = {
		{-3, -10 ,-3},
		{0, 0, 0},
		{3, 10, 3}
		};
*/
		short kernel1[9] = {-3, 0, 3, -10, 0, 10, -3, 0, 3};
		short kernel2[9] = {-3, -10, -3, 0, 0, 0, 3, 10, 3};
		
		short const1[9] = {-601, -600, -599, -1, 0, 1, 599, 600, 601}; 
		
		unsigned char slucaj_1 = 0;
		unsigned char slucaj_2 = 0;
		unsigned char slucaj_3 = 0;
		unsigned char slucaj_4 = 0;


		
/*
		for (int i = 0; i < STRIPE_ROWS; i++)//loop for rows// 
		{
		        for (int j = 0; j < IMG_COLS; j++)//loop for columns// 
		        {
				for(int h = 0; h < 3; h++)
				{
					for(int k = 0; k < 3; k++)
					{
						if((i+h-1) < 0)
						{
							i += 2*IMG_COLS;
							slucaj_1 = 1;
						}
						if((j+k-1) < 0)
						{
							j += 2;
							slucaj_2 = 1;
						}
						if((i+h-1) >= STRIPE_ROWS)
						{
							i -= 2*IMG_COLS;
							slucaj_3 = 1;
						}
						if((j+k-1) >= IMG_COLS)
						{
							j -= 2;
							slucaj_4 = 1;
						}
						if(x_y){
							pos = (h-1) * IMG_COLS;
							sum += ((short)myArray[(i*IMG_COLS+j) + pos + (k-1)] * (short)kernel1[h][k]);
							//sum += ((short)slika.at<uchar>(i+h-1,j+k-1) * (short)kernel_1[h][k]);
						}
						else
						{	
							pos = (h-1) * IMG_COLS;
							sum += ((short)myArray[(i*IMG_COLS+j) + pos + (k-1)] * (short)kernel2[h][k]);
							//sum += ((short)slika.at<uchar>(i+h-1,j+k-1) * (short)kernel_2[h][k]);
						}
						if(slucaj_1)
						{
							i -= 2*IMG_COLS;
							slucaj_1 = 0;
						}					
						if(slucaj_2)
						{
							j -= 2;
							slucaj_2 = 0;
						}
						if(slucaj_3)
		                                {
		                                        i += 2*IMG_COLS;
		                                        slucaj_3 = 0;
		                                }
						if(slucaj_4)
		                                {
		                                        j += 2;
		                                        slucaj_4 = 0;
		                                }

					}
				}
		               	//x = (float)slika_pikseli.at<float>(i, j);//storing value of (i,j) pixel in variable//
		               	//cout << "Value of pixel" << "(" << i << "," << j << ")" << "=" << x << endl;//showing the values in console window//	

*/		               	
		for (int i = 0; i < STRIPE_ROWS * IMG_COLS; i++)//loop for rows// 
		{
		        for(int k = 0; k < 9; k++)
		        {
				if(i < 601)
				{
					i += 2 * IMG_COLS;
					slucaj_1 = 1;
				}
				if((i + 601) > (STRIPE_ROWS * IMG_COLS))
				{
					i -= 2 * IMG_COLS;
					slucaj_2 = 1;
				}
				if(x_y)
				{
					sum += ((short)myArray[i + const1[k]] * (short)kernel1[k]);
				}
				else
				{	
					sum += ((short)myArray[i + const1[k]] * (short)kernel2[k]);
				}
				if(slucaj_1)
				{
					i -= 2 * IMG_COLS;
					slucaj_1 = 0;
				}					
				if(slucaj_2)
				{
					i += 2 * IMG_COLS;
					slucaj_2 = 0;
				}
			}
			
			output[i] = sum;
			sum = 0;
		}
			
		delete[] myArray;

		if(x_y)
		{
			write_bram(STRIPE_ROWS*IMG_COLS, output, STRIPE_ROWS*IMG_COLS);
		}
		else
		{	
			write_bram(3 * STRIPE_ROWS*IMG_COLS, output, STRIPE_ROWS*IMG_COLS);
		}
		delete[] output;
		ready = 1;

			
		cout << "sobel function finished" << endl;
	}
}

void Ip_hard::write_bram(sc_dt::uint64 addr,short *val,int length)
{
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
}


