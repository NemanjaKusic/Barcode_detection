#include "soft.hpp"
#include <chrono>

int argc;
char** data;
//char* data[];

#define TABLE_SIZE( X ) ( sizeof( (X) ) / sizeof( (X)[0] ) )
#define ROUNDINT( F ) static_cast<int>( 0.5 + (F) )

struct CompareContourAreas
{	//todo very inefficient
	static bool Asc(const std::vector< cv::Point>& contour1, const std::vector<cv::Point>& contour2) {
		double i = fabs(cv::contourArea(cv::Mat(contour1)));
		double j = fabs(cv::contourArea(cv::Mat(contour2)));
		return (i < j);
	}
	static bool Desc(const std::vector< cv::Point>& contour1, const std::vector<cv::Point>& contour2) {
		return Asc(contour2, contour1);
	}
};

SC_HAS_PROCESS(Soft);

Soft::Soft(sc_core::sc_module_name name, int argc, char* argv[]) : sc_module(name),  offset(sc_core::SC_ZERO_TIME)
{
	 SC_THREAD(soft);
	 SC_REPORT_INFO("Soft", "Constructed.");
	 argc = argc;
	 data = argv;
}

Soft::~Soft()
{
    SC_REPORT_INFO("Soft", "Destroyed.");
}

void Soft::soft()
{


	//load picture	
	Mat image = imread(data[1], IMREAD_COLOR);

	SC_REPORT_INFO("CPU", "Loaded image from file.");
    
	//turn to grayscale
	Mat gray;
	cvtColor(image, gray, COLOR_BGR2GRAY);
	
	// Resize the image to 600x452
	Mat resizedGray;
	resize(gray, resizedGray, cv::Size(600, 452));
	
	imshow("resizedGray", resizedGray);
	waitKey(0);
	
	

	int ready = 1;
	bool done = 0;
	bool x_y = 1; 
	int stripe = 0;
	
	short *response_img_1 = new short[IMG_ROWS * IMG_COLS];
	short *response_img_2 = new short[IMG_ROWS * IMG_COLS];
	
	unsigned char *response_stripe_1 = new unsigned char[2 * STRIPE_ROWS * IMG_COLS];
	unsigned char *response_stripe_2 = new unsigned char[2 * STRIPE_ROWS * IMG_COLS];	
	
	short *short_response_stripe_1 = new short[STRIPE_ROWS * IMG_COLS];
	short *short_response_stripe_2 = new short[STRIPE_ROWS * IMG_COLS];
	while(!done)
	{
		
		if(ready)
		{
			unsigned char *img = new unsigned char[STRIPE_ROWS * IMG_COLS];
		   				
			for (int j = 0; j < STRIPE_ROWS; j++)
			{
				for (int k = 0; k < IMG_COLS; k++)
				{
					img[j * IMG_COLS + k]= resizedGray.at<unsigned char>(j + stripe*STRIPE_ROWS, k);
				}
			}
			
			//Write image in bram 
			write_bram(VP_ADDR_BRAM_L, img, STRIPE_ROWS * IMG_COLS);
			
			delete[] img;
			
			
			//write_hard(ADDR_X_Y, 1);
			write_hard(ADDR_START, 1);
		}
			
		while(ready)
		{
		        ready = read_hard(ADDR_READY);
		        if (!ready)
		        {
		        	if(x_y)
		        	{
		        		write_hard(ADDR_X_Y, 1);
		        		write_hard(ADDR_START,0);
		        	}
		        	else if(!x_y)
		        	{
					write_hard(ADDR_X_Y, 0);
		        		write_hard(ADDR_START,0);		        		
		        	}
		        	
		        }
		}
		
		ready = read_hard(ADDR_READY);
		
		if(ready)
		{
			if(x_y)
			{
				x_y = !x_y;	
				
				read_bram2(0, short_response_stripe_1, STRIPE_ROWS * IMG_COLS);
				
				for(int i = 0; i < STRIPE_ROWS; i++)
				{
					for(int j = 0; j < STRIPE_COLS; j++)
					{
						response_img_1[stripe * STRIPE_ROWS * STRIPE_COLS + i * STRIPE_COLS + j] = short_response_stripe_1[i * STRIPE_COLS + j];
					}
				}
				
				
				
			}
			else if(!x_y)
			{
				x_y = !x_y;
			
				//read_bram2(STRIPE_ROWS * IMG_COLS, short_response_stripe_2, STRIPE_ROWS * IMG_COLS);
				read_bram2(0, short_response_stripe_2, STRIPE_ROWS * IMG_COLS);
				
				for(int i = 0; i < STRIPE_ROWS; i++)
				{
					for(int j = 0; j < STRIPE_COLS; j++)
					{
						response_img_2[stripe * STRIPE_ROWS * STRIPE_COLS + i * STRIPE_COLS + j] = short_response_stripe_2[i * STRIPE_COLS + j];
					}
				}
				
				cout << endl << "stripe: " << stripe + 1 << " done" << endl; 
				
				stripe++;
			}			
		}
		
		//cout << endl << "while(ready) loop exited" << endl;
			
		if(stripe == 4)
		{
			done = 1;
		}
		
	}

	delete[] response_stripe_1;
	delete[] response_stripe_2;
	
	delete[] short_response_stripe_1;
	delete[] short_response_stripe_2;
	
	
	cout << endl << "while(!done) loop exited" << endl;
	

	cout << endl << "offset is: " << offset << endl;
	
	double offset_in_seconds = offset.to_seconds();
	double framerate = 1 / offset_in_seconds;
	cout << endl << "framerate is: " << framerate << endl;
	


	Mat gradX(IMG_ROWS, IMG_COLS, CV_16S);
	
	for (int j = 0; j < IMG_ROWS; j++)
	{
		for (int k = 0; k < IMG_COLS; k++)
		{
			gradX.at<short>(j,k) = response_img_1[j * IMG_COLS + k];
			//std::cout << gradX.at<short>(j,k) << " ";  // Convert to int for proper printing
		}
	}
	
	imshow("gradX", gradX);
	waitKey(0);
	
        delete[] response_img_1;
             	
	Mat gradY(IMG_ROWS, IMG_COLS, CV_16S);
	
	for (int j = 0; j < IMG_ROWS; j++)
	{
		for (int k = 0; k < IMG_COLS; k++)
		{
			gradY.at<short>(j,k) = response_img_2[j * IMG_COLS + k];
		}
	}
	
	imshow("gradY", gradY);
	waitKey(0);
	
	//delete[] short_img_2;
        delete[] response_img_2;
        
       
        //subtract gradY form gradX
	Mat gradient(452, 600, CV_32F);
	subtract(gradY, gradX, gradient);

	imshow("gradient", gradient);
	waitKey(0);

	//8-bit brightness
	Mat grad;
	convertScaleAbs(gradient, grad);
	
	imshow("grad", grad);
	waitKey(0);

	//blur the image
	Mat blurred;
	blur(grad, blurred, Size(9, 9));
	
	imshow("blurred", blurred);
	waitKey(0);

	//brightness can be 0 or 255
	Mat thresh;
	threshold(blurred, thresh, 225, 255, THRESH_BINARY);

	imshow("thresh", thresh);
	waitKey(0);

	//to expand the white parts of the image
	Mat kernel;
	kernel = getStructuringElement(MORPH_RECT, Size(30, 7));
	Mat closed;
	morphologyEx(thresh, closed, MORPH_CLOSE, kernel);

	//to make the dots in the picture disappear
	kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
	erode(closed, closed, kernel, Point(-1, -1) , 4);
	dilate(closed, closed, kernel, Point(-1, -1), 4);

	//find contours (more precisely contour edges - because RETR_EXTERNAL)
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(closed, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_NONE);

	//draw contours
	
	std::sort(contours.begin(), contours.end(), CompareContourAreas::Desc);
	const  std::vector< cv::Point >& biggestContour = contours[0];

	//minAreaRect accepts only Point or Point2f, i.e. points of type CV_32S or CV_32F.
	cv::RotatedRect rect = cv::minAreaRect(biggestContour);

	//It seems cv::boxPoints() needs a cv::Mat as the OutputArray.
		//The rows are the 4 points and the two columns are x and y.
		//The function cv::boxPoints() finds the four vertices of a rotated rectangle. 
		//This function is useful to draw the rectangle. 
		//In C++, instead of using this function, you can directly use box.points() method. 
	cv::Point2f vertices[4];
	rect.points(vertices);

	std::vector< std::vector< cv::Point > > contours2;
	contours2.push_back(std::vector< cv::Point >());
	std::vector< cv::Point >& vcBoxpoints = contours2.back();
	for (int z = 0; z < TABLE_SIZE(vertices); z++)
	{
		const cv::Point2f& pf = vertices[z];
		vcBoxpoints.push_back(cv::Point(ROUNDINT(pf.x), ROUNDINT(pf.y)));
	} // for (;;)

	Mat image_copy = image.clone();
	cv::drawContours(image_copy, contours2, (-1), cv::Scalar(0, 255, 0), 3);

	

	
	imshow("Image", image);
	waitKey(0);

	imshow("image_copy", image_copy);
	waitKey(0);


        
}

void Soft::write_bram(sc_dt::uint64 addr,unsigned char *val,int length)
{
	

	pl_t pl;

	//10 * DELAY for each AXI4-FULL burst initial delay
	offset += sc_core::sc_time((10 * DELAY) * (length / 256) , sc_core::SC_NS);
	if(length % 256)
	{
		offset += sc_core::sc_time(10 * DELAY , sc_core::SC_NS);
	}
	
	int div_4 = 0;
	unsigned char buf1[BUS_WIDTH];
	
	while(length % BUS_WIDTH)
	{
		length++;
		div_4++;
	}

	unsigned char *buf = new unsigned char[length];
	
	for(int i = 0; i < length - div_4; i++)
	{
    		buf[i] = val[i];
   	}
  
    	for(int i = length - div_4; i < length; i++)
    	{
    		buf[i] = 0;
    	}


   	 pl.set_data_length(BUS_WIDTH);
    
    	for(int j = 0; j < length / BUS_WIDTH; j++)
    	{
    		int l = 0;
    	
    		for(int p = BUS_WIDTH * j; p < BUS_WIDTH + j * BUS_WIDTH; p++)
    		{
    			buf1[l] = buf[p];
    			l++;
    		}
  
		pl.set_address(VP_ADDR_BRAM_L + addr + j * BUS_WIDTH);
		pl.set_data_ptr(buf1);
		pl.set_command(tlm::TLM_WRITE_COMMAND);
		pl.set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);
		interconnect_socket->b_transport(pl,offset);
		
    	}
    	delete[] buf;
}    
    
void Soft::read_bram(sc_dt::uint64 addr, unsigned char *val, int length)
{

	pl_t pl;

	//10 * DELAY for each AXI4-FULL burst initial delay
	offset += sc_core::sc_time((10 * DELAY) * (length / 256) , sc_core::SC_NS);
	if(length % 256)
	{
		offset += sc_core::sc_time(10 * DELAY , sc_core::SC_NS);
	}
	
	unsigned char buf1[BUS_WIDTH];
	int div_4 = BUS_WIDTH;
	
	//unsigned char *buf = new unsigned char[length];
	
	while(length % BUS_WIDTH)
	{
		length++;
		div_4--;
	}
	
	pl.set_data_length(BUS_WIDTH);
	
	for(int i = 0; i < length / BUS_WIDTH; i++)
	{
		pl.set_address(VP_ADDR_BRAM_L + addr + i * BUS_WIDTH);
		pl.set_data_ptr(buf1);
		pl.set_command(tlm::TLM_READ_COMMAND);
		pl.set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);
		interconnect_socket->b_transport(pl,offset);
		
		int l = 0;
		
		if(i * BUS_WIDTH == (length - BUS_WIDTH))
		{
			for(int p = BUS_WIDTH * i; p < div_4 + i * BUS_WIDTH; p++)
    			{
    				val[p] = buf1[l];
    				l++;
    			}
		}
		else
		{
			for(int p = BUS_WIDTH * i; p < BUS_WIDTH + i * BUS_WIDTH; p++)
    			{
    				val[p] = buf1[l];
    				l++;
    			}
    			
		}
	}
	
		
}

void Soft::read_bram2(sc_dt::uint64 addr, short *val, int length)
{

	pl_t pl;
	
	//unsigned char buf1[32];
	
	int div_4 = BUS_WIDTH;
	int real_length = 2 * length;
	//unsigned char *buf = new unsigned char[length];
	
	while(real_length % BUS_WIDTH)
	{
		real_length++;
		div_4--;
	}
		
	int pos = 0;
	
	//10 * DELAY for each AXI4-FULL burst initial delay
	offset += sc_core::sc_time((10 * DELAY) * (real_length / 256) , sc_core::SC_NS);

	
	for(int i = 0; i < real_length / BUS_WIDTH; i++)
	{
		unsigned char *buf1 = new unsigned char[BUS_WIDTH];
		
		pl.set_data_length(BUS_WIDTH);
		pl.set_address(VP_ADDR_BRAM2_L + addr + i * (BUS_WIDTH/2));
		pl.set_data_ptr(buf1);
		pl.set_command(tlm::TLM_READ_COMMAND);
		pl.set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);
		interconnect_socket->b_transport(pl,offset);

		int limit = (i * BUS_WIDTH == real_length - BUS_WIDTH) ? div_4 : BUS_WIDTH;

		for (int l = 0; l < limit; l += 2) {
			val[pos++] = (static_cast<short>(buf1[l]) << 8) | buf1[l + 1];
		}
		delete[] buf1;
	}
	
}

void Soft::write_hard(sc_dt::uint64 addr,int val)
{
    	pl_t pl;
    	
    	//AXI4-LITE is 4 delays
    	offset += sc_core::sc_time(4 * DELAY , sc_core::SC_NS);
    	
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
    	
    	//AXI4-LITE is 4 delays
    	offset += sc_core::sc_time(4 * DELAY , sc_core::SC_NS);
    	
    	unsigned char buf[4];
   	pl.set_address(VP_ADDR_IP_HARD_L + addr);
    	pl.set_data_length(1);
    	pl.set_data_ptr(buf);
    	pl.set_command(tlm::TLM_READ_COMMAND);
    	pl.set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);
    	interconnect_socket->b_transport(pl,offset);
    	return toInt(buf);
}
