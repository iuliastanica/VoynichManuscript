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
#include "Window.h"

using namespace cv;  
using namespace std;

Mat src, dst;
Mat map_x, map_y;

int ind = 0, thres, imageVal, shape;

//Trackbar 1 for Voynich pages
const int max_Trackbar = 4;
//Trackbar 2 for changing Distortion device
const int max_Trackbar2 = 5;
//Trackbar 3 for changing parameters / threshold (?)
const int max_Trackbar3 = 5;
//Trackbar 4 for changing window shape
const int max_Trackbar4 = 4;

//Mouse cursor position
int posX, posY = 0;

int method;

/// Function Headers
void Distort(int, void*);
void ReadImages(int, void*);
void twirlEffect();
void fishEyeEffect();
void CallBackMouseFunc(int event, int x, int y, int flags, void* userdata);

Window w("rectangle", 100, 100, 50, 50);

int main(int argc, char** argv)
{
	/// Load first image
	src = imread("1.jpg");

	/// Create windows
	namedWindow("Source Image", CV_WINDOW_KEEPRATIO);
	//resizeWindow("Source Image", src.rows, src.cols);

	namedWindow("Result window", CV_WINDOW_KEEPRATIO);
	setMouseCallback("Result window", CallBackMouseFunc, NULL);

	//resizeWindow("Result window", 800, 800);

	/// Create Trackbars
	createTrackbar("Image", "Source Image", &imageVal, max_Trackbar, Distort);
	createTrackbar("Method", "Source Image", &ind, max_Trackbar2, Distort);
	createTrackbar("Threshold", "Source Image", &thres, max_Trackbar3, Distort);
	createTrackbar("Shape", "Source Image", &shape, max_Trackbar4, Distort);


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
	for (int i = 0; i < src.rows; i++)
	{
		for (int j = 0; j < src.cols; j++)
		{
			switch (ind)
			{
			case 0:
				map_x.at<float>(i, j) = j;
				map_y.at<float>(i, j) = src.rows - i;
				break;
			case 1:
				map_x.at<float>(i, j) = src.cols - j;
				map_y.at<float>(i, j) = src.rows - i;
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
				twirlEffect();
				break;
			case 5:
				//barrelDistortion(src, dst);
				fishEyeEffect(); 
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

void twirlEffect()
{
	try
	{
		//const cv::String name_window = "Twirl Image";
		const cv::String name_trackbar = "Twirl";

		//namedWindow(name_window);
		createTrackbar(name_trackbar, "Source Image", NULL, 200, trackbar_callback, &src);
		setTrackbarPos(name_trackbar, "Source Image", 80);
		//imshow(name_window, src);

		dst = getResult();
		waitKey(0);
		destroyAllWindows();
	}
	catch (cv::Exception& e)
	{
		const char* err_msg = e.what();
		std::cout << "exception caught: " << err_msg << std::endl;
	}

}

void fishEyeEffect()
{
	fishEye(src, dst, src.cols / 2, src.rows / 2, 0.0001);

}

void CallBackMouseFunc(int event, int x, int y, int flags, void* userdata)
{
	//twirl effect
	if (ind == 4)
		dst = imread("Result.jpg");

	if (event == EVENT_LBUTTONDOWN)
	{
		cout << x << ", " << y << endl;
		posX = x;
		posY = y;

		switch (shape)
		{
		case 0:
			w.getWindowRect(dst, posX, posY);
			break;
		}
	}
}










// Unsuccessful attemp for Barrel distortion
/*void sampleImage(const Mat* arr, float idx0, float idx1, CvScalar& res)
{
	if (idx0<0 || idx1<0 || idx0>((*arr).rows - 1) || idx1>((*arr).cols - 1)) {
		res.val[0] = 0;
		res.val[1] = 0;
		res.val[2] = 0;
		res.val[3] = 0;
		return;
	}
	float idx0_fl = floor(idx0);
	float idx0_cl = ceil(idx0);
	float idx1_fl = floor(idx1);
	float idx1_cl = ceil(idx1);

	CvScalar s1 = cvGet2D(arr, (int)idx0_fl, (int)idx1_fl);
	CvScalar s2 = cvGet2D(arr, (int)idx0_fl, (int)idx1_cl);
	CvScalar s3 = cvGet2D(arr, (int)idx0_cl, (int)idx1_cl);
	CvScalar s4 = cvGet2D(arr, (int)idx0_cl, (int)idx1_fl);
	float x = idx0 - idx0_fl;
	float y = idx1 - idx1_fl;
	res.val[0] = s1.val[0] * (1 - x)*(1 - y) + s2.val[0] * (1 - x)*y + s3.val[0] * x*y + s4.val[0] * x*(1 - y);
	res.val[1] = s1.val[1] * (1 - x)*(1 - y) + s2.val[1] * (1 - x)*y + s3.val[1] * x*y + s4.val[1] * x*(1 - y);
	res.val[2] = s1.val[2] * (1 - x)*(1 - y) + s2.val[2] * (1 - x)*y + s3.val[2] * x*y + s4.val[2] * x*(1 - y);
	res.val[3] = s1.val[3] * (1 - x)*(1 - y) + s2.val[3] * (1 - x)*y + s3.val[3] * x*y + s4.val[3] * x*(1 - y);
}

float xscale;
float yscale;
float xshift;
float yshift;

float getRadialX(float x, float y, float cx, float cy, float k) {
	x = (x*xscale + xshift);
	y = (y*yscale + yshift);
	float res = x + ((x - cx)*k*((x - cx)*(x - cx) + (y - cy)*(y - cy)));
	return res;
}

float getRadialY(float x, float y, float cx, float cy, float k) {
	x = (x*xscale + xshift);
	y = (y*yscale + yshift);
	float res = y + ((y - cy)*k*((x - cx)*(x - cx) + (y - cy)*(y - cy)));
	return res;
}

float thresh = 1;
float calc_shift(float x1, float x2, float cx, float k) {
	float x3 = x1 + (x2 - x1)*0.5;
	float res1 = x1 + ((x1 - cx)*k*((x1 - cx)*(x1 - cx)));
	float res3 = x3 + ((x3 - cx)*k*((x3 - cx)*(x3 - cx)));

	//  std::cerr<<"x1: "<<x1<<" - "<<res1<<" x3: "<<x3<<" - "<<res3<<std::endl;

	if (res1>-thresh && res1 < thresh)
		return x1;
	if (res3<0) {
		return calc_shift(x3, x2, cx, k);
	}
	else {
		return calc_shift(x1, x3, cx, k);
	}
}

void barrelDistortion(Mat src, Mat dst)

{
	//IplImage* src = cvLoadImage(argv[1], 1);
	//IplImage* dst = cvCreateImage(cvGetSize(src), src->depth, src->nChannels);
	//IplImage* dst2 = cvCreateImage(cvGetSize(src), src->depth, src->nChannels);

	float K = 0.001f;
	float centerX = src.cols/2;
	float centerY = src.rows/2;
	int width = src.cols;
	int height = src.rows;

	xshift = calc_shift(0, centerX - 1, centerX, K);
	float newcenterX = width - centerX;
	float xshift_2 = calc_shift(0, newcenterX - 1, newcenterX, K);

	yshift = calc_shift(0, centerY - 1, centerY, K);
	float newcenterY = height - centerY;
	float yshift_2 = calc_shift(0, newcenterY - 1, newcenterY, K);
	//  scale = (centerX-xshift)/centerX;
	xscale = (width - xshift - xshift_2) / width;
	yscale = (height - yshift - yshift_2) / height;

	std::cerr << xshift << " " << yshift << " " << xscale << " " << yscale << std::endl;
	std::cerr << height << std::endl;
	std::cerr << width << std::endl;
	IplImage temp = dst;

	for (int j = 0; j < dst.rows; j++) {
		for (int i = 0; i < dst.cols; i++) {
			CvScalar s;
			float x = getRadialX((float)i, (float)j, centerX, centerY, K);
			float y = getRadialY((float)i, (float)j, centerX, centerY, K);
			sampleImage(&src, y, x, s);
			cvSet2D(&temp, j, i, s);
		}
	}

	//not used
	Mat dest = cvarrToMat(&temp);
	cvNamedWindow("Source1", 1);
	cvShowImage("Source1", &temp);
	cvWaitKey(0);

	//cvSaveImage(argv[2], dst, 0);

/*	for (int j = 0; j < dst.rows; j++) {
		for (int i = 0; i < i < dst.cols; i++) {
			CvScalar s;
			sampleImage(&src, j + 0.25, i + 0.25, s);
			cvSet2D(&temp, j, i, s);
		}
	}

	cvNamedWindow("Source1", 1);
	cvShowImage("Source1", src);
	cvWaitKey(0);*/

