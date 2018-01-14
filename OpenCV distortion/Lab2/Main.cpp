#include "stdafx.h"
#define _USE_MATH_DEFINES
#include "math.h"
#include <fstream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream> 
#include "TwirlDistortion.h"
#include "FishEyeDistortion.h"
#include "CylinderDistortion.h"
#include "ConeDistortion.h"
#include "Scramble.h"
#include "Window.h"
#include "Template.h"

using namespace cv;  
using namespace std;

int imgInd=0;

Mat src, dst, temp;
Mat map_x, map_y;

Mat small_window, small_dest, small_map_x, small_map_y;

//Trackbar 1 for Voynich pages
const int max_Trackbar = 3;
//Trackbar 2 for changing Distortion device
const int max_Trackbar2 = 8;
//Trackbar 4 for changing window shape
const int max_Trackbar3 = 1;
//Trackbar 5 for window width
const int max_Trackbar4 = 100;
//Trackbar 6 for window height
const int max_Trackbar5 = 100;

int ind, imageVal, shape, width, height;

//Mouse cursor position
int posX, posY = 0;

int method;

/// Function Headers
void Distort(int, void*);
void ReadImages(int, void*);
void twirlEffect(Mat* source, Mat dest, int i);
void fishEyeEffect(Mat source, Mat dest);
void cylinderEffect(Mat* source, Mat dest, int i);
void coneEffect(Mat* source, Mat dest, int i);

void CallBackMouseFunc(int event, int x, int y, int flags, void* userdata);

Window w("rectangle", 100, 100);

int main(int argc, char** argv)
{
	/// Load first image
	src = imread("1.jpg");

	/// Create windows
	namedWindow("Source Image", CV_WINDOW_KEEPRATIO);
	//resizeWindow("Source Image", src.rows, src.cols);

	namedWindow("Result window", CV_WINDOW_KEEPRATIO);

	//resizeWindow("Result window", 800, 800);

	ind = 0; imageVal = 0; shape = 0; width = 0; height = 0;

	/// Create Trackbars
	createTrackbar("Image", "Source Image", &imageVal, max_Trackbar, Distort);
	createTrackbar("Method", "Source Image", &ind, max_Trackbar2, Distort);
	createTrackbar("Width", "Source Image", &width, max_Trackbar4, Distort);
	createTrackbar("Height", "Source Image", &height, max_Trackbar5, Distort);

	setMouseCallback("Source Image", CallBackMouseFunc, NULL);

	Distort(0, 0);
}

void ReadImages(int, void*)
{
	/// Load image (maybe better from cmd)
	string s = Str(imageVal + 1) + ".jpg";
	cout << s;
	src = imread(s);
}

void Distort(int, void*)
{
	ReadImages(0, 0);

	string n = "Templates/Temp4.jpg";
	temp = imread(n);

	/// Source image to display
	Mat img_display;
	src.copyTo(img_display);

	/// Create dst, map_x and map_y with the same size as src:
	dst.create(src.size(), src.type());
	map_x.create(src.size(), CV_32FC1);
	map_y.create(src.size(), CV_32FC1);

	//distortion cases
	switch (ind)
	{
		case 0:
			for (int i = 0; i < src.rows; i++)
			{
				for (int j = 0; j < src.cols; j++)
				{
					map_x.at<float>(i, j) = (float)j;
					map_y.at<float>(i, j) = (float)src.rows - i;
				}
			}
			remap(src, dst, map_x, map_y, CV_INTER_LINEAR, BORDER_CONSTANT, Scalar(0, 0, 0));
			break;
		case 1:
			for (int i = 0; i < src.rows; i++)
			{
				for (int j = 0; j < src.cols; j++)
				{
					map_x.at<float>(i, j) = (float)src.cols - j;
					map_y.at<float>(i, j) = (float)src.rows - i;
				}
			}
			remap(src, dst, map_x, map_y, CV_INTER_LINEAR, BORDER_CONSTANT, Scalar(0, 0, 0));
			break;
		case 2:
			for (int i = 0; i < src.rows; i++)
			{
				for (int j = 0; j < src.cols; j++)
				{
					map_x.at<float>(i, j) = (float)j;
					map_y.at<float>(i, j) = (float)src.rows - i;
				}
			}
			remap(src, dst, map_x, map_y, CV_INTER_LINEAR, BORDER_CONSTANT, Scalar(0, 0, 0));
			break;
		case 3:
			for (int i = 0; i < src.rows; i++)
			{
				for (int j = 0; j < src.cols; j++)
				{
					map_x.at<float>(i, j) = (float)src.cols - j;
					map_y.at<float>(i, j) = (float)i;
				}
			}
			remap(src, dst, map_x, map_y, CV_INTER_LINEAR, BORDER_CONSTANT, Scalar(0, 0, 0));
			break;
		case 4:
			twirlEffect(&img_display, dst, 1);
			break;
		case 5:
			fishEyeEffect(img_display, dst);
			break;		
		case 6:
			cylinderEffect(&img_display, dst, 1);
			break;
		case 7:
			coneEffect(&img_display, dst, 1);
			break;
		case 8:
			match(img_display, temp);
			scramble(img_display, dst, 200, 100);

			break;
	}

	//Set width and height of the window
	w.setNoCols(20 + 5 * width);
	w.setNoRows(20 + 5 * height);

	/// Display results
	imshow("Result window", dst);

	string name = "Results/Image" + to_string(imgInd) + ".jpg";
	imgInd++;

	imwrite(name, dst);

	imshow("Source Image", img_display);

	waitKey(0);
	return;
}

void twirlEffect(Mat* source, Mat dest, int i) //i=0 small, i=1 big
{
	try
	{
		//should be changed based on the small window's name
		if (i == 1)
		{
			createTrackbar("TwirlLarge", "Source Image", NULL, 200, trackbar_callback1, source);
			setTrackbarPos("TwirlLarge", "Source Image", 80);

			//dest = getResult();
			getResult().copyTo(dest);
		}
		else if (i == 0)
		{
			createTrackbar("TwirlSmall", "Source Image", NULL, 200, trackbar_callback0, source);
			setTrackbarPos("TwirlSmall", "Source Image", 80);

			//dest = getResult();
			getResult().copyTo(dest);
		}
	}
	catch (cv::Exception& e)
	{
		const char* err_msg = e.what();
		std::cout << "exception caught: " << err_msg << std::endl;
	}
}

void fishEyeEffect(Mat source, Mat dest)
{
	fishEye(source, dest, source.cols / 2, source.rows / 2, 0.0001);

}

void cylinderEffect(Mat* source, Mat dest, int i) //i=0 small, i=1 big
{
	try
	{
		//should be changed based on the small window's name
		if (i == 1)
		{
			createTrackbar("CylinderLarge", "Source Image", NULL, 200, cyl_callback1, source);
			setTrackbarPos("CylinderLarge", "Source Image", 80);

			//dest = getResult();
			getCylResult().copyTo(dest);
		}
		else if (i == 0)
		{
			createTrackbar("CylinderSmall", "Source Image", NULL, 200, cyl_callback0, source);
			setTrackbarPos("CylinderSmall", "Source Image", 80);

			//dest = getResult();
			getCylResult().copyTo(dest);
		}
	}
	catch (cv::Exception& e)
	{
		const char* err_msg = e.what();
		std::cout << "exception caught: " << err_msg << std::endl;
	}
}

void coneEffect(Mat* source, Mat dest, int i) //i=0 small, i=1 big
{
	try
	{
		//should be changed based on the small window's name
		if (i == 1)
		{
			createTrackbar("ConeLarge", "Source Image", NULL, 200, cone_callback1, source);
			setTrackbarPos("ConeLarge", "Source Image", 80);

			//dest = getResult();
			getConeResult().copyTo(dest);
		}
		else if (i == 0)
		{
			createTrackbar("ConeSmall", "Source Image", NULL, 200, cone_callback0, source);
			setTrackbarPos("ConeSmall", "Source Image", 80);

			//dest = getResult();
			getConeResult().copyTo(dest);
		}
	}
	catch (cv::Exception& e)
	{
		const char* err_msg = e.what();
		std::cout << "exception caught: " << err_msg << std::endl;
	}
}

void CallBackMouseFunc(int event, int x, int y, int flags, void* userdata)
{
	if (event == EVENT_LBUTTONDOWN)
	{
		cout << x << ", " << y << endl;
		posX = x;
		posY = y;


		switch (shape)
		{
		case 0:
			small_window = w.getWindowRect(src, posX, posY);
			break;
		case 1:
			small_window = w.getWindowCircle(src, posX, posY);
			break;
		}

		small_dest.create(small_window.size(), small_window.type());
		small_map_x.create(small_window.size(), CV_32FC1);
		small_map_y.create(small_window.size(), CV_32FC1);

		//distortion cases
		switch (ind)
		{
		case 0:
			for (int i = 0; i < small_window.rows; i++)
			{
				for (int j = 0; j < small_window.cols; j++)
				{
					small_map_x.at<float>(i, j) = (float)j;
					small_map_y.at<float>(i, j) = (float)small_window.rows - i;
				}
			}
			remap(small_window, small_dest, small_map_x, small_map_y, CV_INTER_LINEAR, BORDER_CONSTANT, Scalar(0, 0, 0));
			break;
		case 1:
			for (int i = 0; i < small_window.rows; i++)
			{
				for (int j = 0; j < small_window.cols; j++)
				{
					small_map_x.at<float>(i, j) = (float)small_window.cols - j;
					small_map_y.at<float>(i, j) = (float)small_window.rows - i;
				}
			}
			remap(small_window, small_dest, small_map_x, small_map_y, CV_INTER_LINEAR, BORDER_CONSTANT, Scalar(0, 0, 0));
			break;
		case 2:
			for (int i = 0; i < src.rows; i++)
			{
				for (int j = 0; j < src.cols; j++)
				{
					small_map_x.at<float>(i, j) = (float)j;
					small_map_y.at<float>(i, j) = (float)small_window.rows - i;
				}
			}
			remap(small_window, small_dest, small_map_x, small_map_y, CV_INTER_LINEAR, BORDER_CONSTANT, Scalar(0, 0, 0));
			break;
		case 3:
			for (int i = 0; i < small_window.rows; i++)
			{
				for (int j = 0; j < small_window.cols; j++)
				{
					small_map_x.at<float>(i, j) = (float)small_window.cols - j;
					small_map_y.at<float>(i, j) = (float)i;
				}
			}
			remap(small_window, small_dest, small_map_x, small_map_y, CV_INTER_LINEAR, BORDER_CONSTANT, Scalar(0, 0, 0));
			break;
		case 4:
			twirlEffect(&small_window, small_dest, 0);
			break;
		case 5:
			fishEyeEffect(small_window, small_dest);
			break;
		case 6:
			cylinderEffect(&small_window, small_dest, 0);
			break;
		case 7:
			coneEffect(&small_window, small_dest, 0);
			break;
		}

		namedWindow("Roi", CV_WINDOW_AUTOSIZE);
		imshow("Roi", small_dest);
	}

	//Matching

	
}