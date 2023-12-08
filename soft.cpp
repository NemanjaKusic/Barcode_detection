#include "soft.hpp"



SC_HAS_PROCESS(Soft);

Soft::Soft(sc_core::sc_module_name name) : sc_module(name),  offset(sc_core::SC_ZERO_TIME)
{
	 SC_THREAD(soft);
	 SC_REPORT_INFO("Soft", "Constructed.");
}

Soft::~Soft()
{
    SC_REPORT_INFO("Soft", "Destroyed.");
}

void Soft::soft()
{
/*
	unsigned char example[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

	write_bram(0, example, 10);
	
	unsigned char response[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	
	read_bram(0, response, 10);
	
	std::cout << std::endl;
	
	for (int i = 0; i < 10; ++i) 
	{
        	std::cout << static_cast<int>(response[i]) << " ";  // Convert to int for proper printing
	}

    	std::cout << std::endl;
	
	write_hard(ADDR_START,1);
	
	int start = 0;
	
	start = read_hard(ADDR_START);
	
	cout << "start bit is " << start << endl;
	
	unsigned char response2[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	
	read_bram(10, response2, 10);
	
	for (int i = 0; i < 10; ++i) 
	{
        	std::cout << static_cast<int>(response2[i]) << " ";  // Convert to int for proper printing
	}
	
*/
/*
	int ready = 1;
	bool done = 0;
	
	unsigned char example[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	
	while(!done)
	{
		if(ready)
		{
			write_bram(0, example, 10);
			
			std::cout << std::endl;
	
			for (int i = 0; i < 10; ++i) 
			{
        			std::cout << static_cast<int>(example[i]) << " ";  // Convert to int for proper printing
			}

    			std::cout << std::endl;
			
			write_hard(ADDR_START,1);
		}
		
		while(ready)
		{
		        ready = read_hard(ADDR_READY);
		        if (!ready)
		        	write_hard(ADDR_START,0);
		}
		cout << endl << "while(ready) loop exited" << endl;
		

			
		done = 1;

	}
	
	cout << endl << "while(!done) loop exited" << endl;
	
	unsigned char response[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	
	read_bram(10, response, 10);
	
	for (int i = 0; i < 10; ++i) 
	{
        	std::cout << static_cast<int>(response[i]) << " ";  // Convert to int for proper printing
	}
*/

	//load picture
	Mat image = imread("../slike/barkod_3b.jpg");	
	//Mat image = imread(argv[i], IMREAD_COLOR);

	SC_REPORT_INFO("CPU", "Loaded image from file.");
    
	//turn to grayscale
	Mat gray;
	cvtColor(image, gray, COLOR_BGR2GRAY);
	
	// Resize the image to 600x450
	Mat resizedGray;
	resize(gray, resizedGray, cv::Size(600, 450));
	/*
	unsigned char *img = new unsigned char[IMG_ROWS * IMG_COLS];
		   				
			for (int j = 0; j < IMG_ROWS; j++)
			{
				for (int k = 0; k < IMG_COLS; k++)
				{
					img[j * IMG_COLS + k]= resizedGray.at<unsigned char>(j,k);
				}
			}
			
			//Write image in bram 
			write_bram(VP_ADDR_BRAM_L, img, IMG_ROWS * IMG_COLS);
			
			cout << "array of pixels of an image:" << endl;
			
			for (int i = 0; i < IMG_ROWS * IMG_COLS; ++i) 
			{
        			std::cout << static_cast<int>(img[i]) << " ";  // Convert to int for proper printing
			}
			delete[] img;
	*/
	/*
	unsigned char *img = new unsigned char[IMG_ROWS * IMG_COLS];
		   				
			for (int j = 0; j < IMG_ROWS; j++)
			{
				for (int k = 0; k < IMG_COLS; k++)
				{
					img[j * IMG_COLS + k]= resizedGray.at<unsigned char>(j,k);
				}
			}
			
			//Write image in bram 
			write_bram(VP_ADDR_BRAM_L, img, IMG_ROWS * IMG_COLS);
			
			cout << "array of pixels of an image:" << endl;
			
			for (int i = 0; i < IMG_ROWS * IMG_COLS; ++i) 
			{
        			std::cout << static_cast<int>(img[i]) << " ";  // Convert to int for proper printing
			}
			delete[] img;
			
			cout << endl;
			write_hard(ADDR_START,1);
			
			
			unsigned char *response_img = new unsigned char[IMG_ROWS * IMG_COLS];
			
			
			//cout << endl << "ovde se pojavi segmentation fault" << endl;
			read_bram(IMG_ROWS * IMG_COLS, response_img, IMG_ROWS * IMG_COLS);
	
			cout << "array of pixels of response_img:" << endl;
			for (int i = 0; i < IMG_ROWS * IMG_COLS; ++i) 
			{
        			std::cout << static_cast<int>(response_img[i]) << " ";  // Convert to int for proper printing
			}
	
			delete[] response_img;
	*/

	int ready = 1;
	bool done = 0;
	
	while(!done)
	{
		
		if(ready)
		{
			unsigned char *img = new unsigned char[IMG_ROWS * IMG_COLS];
		   				
			for (int j = 0; j < IMG_ROWS; j++)
			{
				for (int k = 0; k < IMG_COLS; k++)
				{
					img[j * IMG_COLS + k]= resizedGray.at<unsigned char>(j,k);
				}
			}
			
			//Write image in bram 
			write_bram(VP_ADDR_BRAM_L, img, IMG_ROWS * IMG_COLS);
			
			cout << "array of pixels of an image:" << endl;
			
			for (int i = 0; i < IMG_ROWS * IMG_COLS; ++i) 
			{
        			std::cout << static_cast<int>(img[i]) << " ";  // Convert to int for proper printing
			}
			delete[] img;
			
			write_hard(ADDR_START, 1);
		}
			
		while(ready)
		{
		        ready = read_hard(ADDR_READY);
		        if (!ready)
		        	write_hard(ADDR_START,0);
		}
		
		cout << endl << "while(ready) loop exited" << endl;
			
		done = 1;
		
	}

	cout << endl << "while(!done) loop exited" << endl;
	
	unsigned char *response_img = new unsigned char[IMG_ROWS * IMG_COLS];	
	//unsigned char response_img[IMG_ROWS * IMG_COLS];
	
	read_bram(IMG_ROWS * IMG_COLS, response_img, IMG_ROWS * IMG_COLS);
	
	cout << "array of pixels of response_img:" << endl;
	for (int i = 0; i < IMG_ROWS * IMG_COLS; ++i) 
	{
        	std::cout << static_cast<int>(response_img[i]) << " ";  // Convert to int for proper printing
	}
	
	delete[] response_img;
        
}

void Soft::write_bram(sc_dt::uint64 addr,unsigned char *val,int length)
{
	pl_t pl;
	
	offset += sc_core::sc_time(DELAY, sc_core::SC_NS);
	
	pl.set_data_length(length);
	pl.set_address(addr);
	pl.set_data_ptr(val);
	pl.set_command(tlm::TLM_WRITE_COMMAND);
	pl.set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);
	interconnect_socket->b_transport(pl,offset);
}

void Soft::read_bram(sc_dt::uint64 addr, unsigned char *val, int length)
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
	interconnect_socket->b_transport(pl,offset);
	
	for(int i = 0; i < length; i++)
	{
		val[i] = buf[i];
	}
	
	delete[] buf;
}

void Soft::write_hard(sc_dt::uint64 addr,int val)
{
    	pl_t pl;
    	unsigned char buf[4];
    	toUchar(buf,val); 	
    	pl.set_address(VP_ADDR_IP_HARD_L + addr);
    	pl.set_data_length(1);
    	pl.set_data_ptr(buf);
    	pl.set_command(tlm::TLM_WRITE_COMMAND);
    	pl.set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);
    	interconnect_socket->b_transport(pl,offset);
}

int Soft::read_hard(sc_dt::uint64 addr)
{
    	pl_t pl;
    	unsigned char buf[4];
   	pl.set_address(VP_ADDR_IP_HARD_L + addr);
    	pl.set_data_length(1);
    	pl.set_data_ptr(buf);
    	pl.set_command(tlm::TLM_READ_COMMAND);
    	pl.set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);
    	sc_core::sc_time offset = sc_core::SC_ZERO_TIME;
    	interconnect_socket->b_transport(pl,offset);
    	return toInt(buf);
}
