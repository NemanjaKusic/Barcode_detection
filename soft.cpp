#include "soft.hpp"

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
	
	imshow("resizedGray", resizedGray);
	waitKey(0);
	
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
			
			write_hard(ADDR_X_Y, 1);
			write_hard(ADDR_START, 1);
		}
			
		while(ready)
		{
		        ready = read_hard(ADDR_READY);
		        if (!ready)
		        	write_hard(ADDR_START,0);
		}
		
		ready = 1;
		write_hard(ADDR_READY, 1);
		
		if(ready)
		{
			
			write_hard(ADDR_X_Y, 0);
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
	
	unsigned char *response_img_1 = new unsigned char[2 * IMG_ROWS * IMG_COLS];	

	read_bram(IMG_ROWS * IMG_COLS, response_img_1, 2 * IMG_ROWS * IMG_COLS);
	
	cout << "array of pixels of response_img_1:" << endl;
	for (int i = 0; i < 2 * IMG_ROWS * IMG_COLS; ++i) 
	{
        	std::cout << static_cast<int>(response_img_1[i]) << " ";  // Convert to int for proper printing
	}
	
	short *short_img_1 = new short[IMG_ROWS * IMG_COLS];
	
	for(int i = 0; i < IMG_ROWS * IMG_COLS; i++)
	{
		short_img_1[i] = (static_cast<signed short>(response_img_1[2*i]) << 8) | response_img_1[2*i + 1];
	}
	
	
	
	Mat gradX(IMG_ROWS, IMG_COLS, CV_16S);
	
	for (int j = 0; j < IMG_ROWS; j++)
	{
		for (int k = 0; k < IMG_COLS; k++)
		{
			gradX.at<short>(j,k) = short_img_1[j * IMG_COLS + k];
		}
	}
	
	imshow("gradX", gradX);
	waitKey(0);
	
	delete[] short_img_1;
        delete[] response_img_1;
        
        
        
        
        unsigned char *response_img_2 = new unsigned char[2 * IMG_ROWS * IMG_COLS];	

	read_bram(3 * IMG_ROWS * IMG_COLS, response_img_2, 2 * IMG_ROWS * IMG_COLS);
	
	cout << "array of pixels of response_img_2:" << endl;
	for (int i = 0; i < 2 * IMG_ROWS * IMG_COLS; ++i) 
	{
        	std::cout << static_cast<int>(response_img_2[i]) << " ";  // Convert to int for proper printing
	}
	
	short *short_img_2 = new short[IMG_ROWS * IMG_COLS];
	
	for(int i = 0; i < IMG_ROWS * IMG_COLS; i++)
	{
		short_img_2[i] = (static_cast<signed short>(response_img_2[2*i]) << 8) | response_img_2[2*i + 1];
	}
	
	
	
	Mat gradY(IMG_ROWS, IMG_COLS, CV_16S);
	
	for (int j = 0; j < IMG_ROWS; j++)
	{
		for (int k = 0; k < IMG_COLS; k++)
		{
			gradY.at<short>(j,k) = short_img_2[j * IMG_COLS + k];
		}
	}
	
	imshow("gradY", gradY);
	waitKey(0);
	
	delete[] short_img_2;
        delete[] response_img_2;
        
        
        //oduzmi y_gradijent od x
	Mat gradient(450, 600, CV_32F);
	subtract(gradY, gradX, gradient);

	//8-bitna osvetljenost
	Mat grad;
	convertScaleAbs(gradient, grad);

	//zamutiti sliku
	Mat blurred;
	blur(grad, blurred, Size(9, 9));

	//osvetljenost u 0 ili 255
	Mat thresh;
	threshold(blurred, thresh, 225, 255, THRESH_BINARY);

	//da se prosire beli delovi slike
	Mat kernel;
	kernel = getStructuringElement(MORPH_RECT, Size(30, 7));
	Mat closed;
	morphologyEx(thresh, closed, MORPH_CLOSE, kernel);

	//da nestanu tackice na slici i sl.
	kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
	erode(closed, closed, kernel, Point(-1, -1) , 4);
	dilate(closed, closed, kernel, Point(-1, -1), 4);

	//naci konture(tacnije ivice kontura-jer RETR_EXTERNAL)
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(closed, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_NONE);
	//contours = moja_funkcija(closed, hierarchy);
	/*
	//naci najvecu konturu
	sort(contours.begin(), contours.end(), Contour_Area);
	vector<Point> c = contours[0];

	RotatedRect rect;
	rect = minAreaRect(c);

	Mat box;
	boxPoints(rect, box);
	*/
	/*
	vector<Point> box;
	boxPoints(rect, box);
	
	vector<Point2f> box(4);
	rect.points(box.data());
	*/
	//nacrtati konture

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

	


	/*
	Mat image_copy = image.clone();
	drawContours(image_copy, contours, -1, Scalar(0, 255, 0), 3);
	*/

	
	imshow("Image", image);
	waitKey(0);
/*
	imshow("grad", grad);
	waitKey(0);
	imshow("thresh", thresh);
	waitKey(0);
	imshow("closed", closed);
	waitKey(0);
*/
	imshow("image_copy", image_copy);
	waitKey(0);
	
        
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
