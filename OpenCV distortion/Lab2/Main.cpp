#include "stdafx.h"
#define _USE_MATH_DEFINES
#include "math.h"
#include <fstream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>  
#include <iostream> 
#include "TwirlDistortion.h"
#include "FishEyeDistortion.h"
#include "CylinderDistortion.h"
#include "ConeDistortion.h"
#include "Window.h"

using namespace cv;  
using namespace std;

Mat src, dst;
Mat map_x, map_y;

Mat small_window, small_dest, small_map_x, small_map_y;

int ind = 0, thres, imageVal, shape, width, height;

//Trackbar 1 for Voynich pages
const int max_Trackbar = 4;
//Trackbar 2 for changing Distortion device
const int max_Trackbar2 = 6;
//Trackbar 3 for changing parameters / threshold (?)
const int max_Trackbar3 = 5;
//Trackbar 4 for changing window shape
const int max_Trackbar4 = 1;
//Trackbar 5 for window width
const int max_Trackbar5 = 37;
//Trackbar 6 for window height
const int max_Trackbar6 = 37;

//Mouse cursor position
int posX, posY = 0;

int method;

/// Function Headers
void Distort(int, void*);
void ReadImages(int, void*);
void twirlEffect(Mat source);
void fishEyeEffect(Mat source, Mat dest);
void cylinder(Mat source);
void cone(Mat source);
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
	setMouseCallback("Source Image", CallBackMouseFunc, NULL);

	//resizeWindow("Result window", 800, 800);

	/// Create Trackbars
	createTrackbar("Image", "Source Image", &imageVal, max_Trackbar, Distort);
	createTrackbar("Method", "Source Image", &ind, max_Trackbar2, Distort);
	createTrackbar("Threshold", "Source Image", &thres, max_Trackbar3, Distort);
	createTrackbar("Shape", "Source Image", &shape, max_Trackbar4, Distort);
	createTrackbar("Width", "Source Image", &width, max_Trackbar5, Distort);
	createTrackbar("Height", "Source Image", &height, max_Trackbar6, Distort);

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
			twirlEffect(img_display);
			break;
		case 5:
			fishEyeEffect(img_display, dst);
			break;
		case 6:
			cylinder(img_display);
			break;
		//case 7:
			//cone(img_display);
	}

	//Set width and height of the window
	w.setNoCols(20 + 5 * width);
	w.setNoRows(20 + 5 * height);

	/// Display results
	imshow("Result window", dst);

	imshow("Source Image", img_display);

	waitKey(0);
	return;
}

void twirlEffect(Mat source)
{
	try
	{
		//const cv::String name_window = "Twirl Image";
		const cv::String name_trackbar = "Twirl";

		//namedWindow(name_window);

		//should be changed based on the small window's name
		createTrackbar(name_trackbar, "Source Image", NULL, 200, trackbar_callback, &source);
		setTrackbarPos(name_trackbar, "Source Image", 80);
		//imshow("Source Image", source);

		//dst = getResult();
		//waitKey(0);
		//destroyAllWindows();
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

//Should be changed
void cylinder(Mat source)
{
	try
	{
		//const cv::String name_window = "Twirl Image";
		const cv::String name_trackbar = "Cylinder";

		//namedWindow(name_window);

		//should be changed based on the small window's name
		createTrackbar(name_trackbar, "Source Image", NULL, 200, trackbar_callback_cylinder, &source);
		setTrackbarPos(name_trackbar, "Source Image", 80);
		//imshow("Source Image", source);

		//dst = getResult();
		//waitKey(0);
		//destroyAllWindows();
	}
	catch (cv::Exception& e)
	{
		const char* err_msg = e.what();
		std::cout << "exception caught: " << err_msg << std::endl;
	}
}

/*void cone(Mat source)
{
	try
	{
		//const cv::String name_window = "Twirl Image";
		const cv::String name_trackbar = "Twirl";

		//namedWindow(name_window);

		//should be changed based on the small window's name
		createTrackbar(name_trackbar, "Source Image", NULL, 200, trackbar_callback_cone, &source);
		setTrackbarPos(name_trackbar, "Source Image", 80);
		//imshow("Source Image", source);

		//dst = getResult();
		//waitKey(0);
		//destroyAllWindows();
	}
	catch (cv::Exception& e)
	{
		const char* err_msg = e.what();
		std::cout << "exception caught: " << err_msg << std::endl;
	}
}*/

void CallBackMouseFunc(int event, int x, int y, int flags, void* userdata)
{
	//twirl effect
	//if (ind == 4)
		//dst = imread("Result.jpg");

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
			//Not working well
			twirlEffect(small_window);
			small_dest = getResult();
			break;
		case 5:
			fishEyeEffect(small_window, small_dest);
			break;
		}

		namedWindow("Roi", CV_WINDOW_AUTOSIZE);
		imshow("Roi", small_dest);
	}
	
}