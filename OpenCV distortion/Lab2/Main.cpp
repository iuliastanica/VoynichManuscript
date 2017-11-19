#include "stdafx.h"
#define _USE_MATH_DEFINES
#include "math.h"
#include <fstream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>  
#include <iostream> 

using namespace cv;  
using namespace std;

Mat src, dst;
Mat map_x, map_y;

int ind = 0, thres, imageVal;

const int max_Trackbar = 10;
int max_Trackbar2 = 4;
int max_Trackbar3 = 5;

int method;

/// Function Headers
void Distort(int, void*);
void ReadImages(int, void*);

int main(int argc, char** argv)
{
	/// Load first image
	src = imread("1.png");

	/// Create windows
	namedWindow("Source Image", CV_WINDOW_AUTOSIZE);
	//resizeWindow("Source Image", src.rows, src.cols);

	namedWindow("Result window", CV_WINDOW_AUTOSIZE);
	//resizeWindow("Result window", 800, 800);

	/// Create Trackbars
	//Trackbar 1 for Voynich pages
	//Trackbar 2 for changing Distortion device
	//Trackbar 3 for changing parameters / threshold (?)
	createTrackbar("Image", "Source Image", &imageVal, max_Trackbar, Distort);
	createTrackbar("Method", "Source Image", &ind, max_Trackbar2, Distort);
	createTrackbar("Threshold", "Source Image", &thres, max_Trackbar3, Distort);

	Distort(0, 0);
}

void ReadImages(int, void*)
{
	/// Load image (maybe better from cmd)
	if (imageVal == 0)
	{
		src = imread("1.png");
	}
	else
	{
		src = imread("2.png");
	}
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

	for (int i = 0; i < src.rows; i++)
	{
		for (int j = 0; j < src.cols; j++)
		{
			switch (ind)
			{
			case 0:
				dst = src;
			case 1:
				if (j > src.cols*0.25 && j < src.cols*0.75 && i > src.rows*0.25 && i < src.rows*0.75)
				{
					map_x.at<float>(i, j) = 2 * (j - src.cols*0.25) + 0.5;
					map_y.at<float>(i, j) = 2 * (i - src.rows*0.25) + 0.5;
				}
				else
				{
					map_x.at<float>(i, j) = 0;
					map_y.at<float>(i, j) = 0;
				}
				break;
			case 2:
				map_x.at<float>(i, j) = j;
				map_y.at<float>(i, j) = src.rows - i;
				break;
			case 3:
				map_x.at<float>(i, j) = src.cols - j;
				map_y.at<float>(i, j) = i;
				break;
			case 4:
				map_x.at<float>(i, j) = src.cols - j;
				map_y.at<float>(i, j) = src.rows - i;
				break;
			} 
		}
	}

	remap(src, dst, map_x, map_y, CV_INTER_LINEAR, BORDER_CONSTANT, Scalar(0, 0, 0));

	/// Display results
	imshow("Result window", dst);

	imshow("Source Image", img_display);

	waitKey(0);
	return;
}