#include "stdafx.h"
#include "Perspective.h"

Mat resultPersp, originalPersp, *auxDestPersp;
int indPersp = 0;
int px1 = 0, py1 = 0, px2 = 0, py2 = 0, px3 = 0, py3 = 0, px4 = 0, py4 = 0;

void refreshPersp()
{
	Point2f inputQuad[4];
	Point2f outputQuad[4];

	Mat lambda(2, 4, CV_32FC1);
	lambda = Mat::zeros(originalPersp.rows, originalPersp.cols, originalPersp.type());

	inputQuad[0] = Point2f(0, 0);
	inputQuad[1] = Point2f(originalPersp.cols, 0);
	inputQuad[2] = Point2f(originalPersp.cols, originalPersp.rows);
	inputQuad[3] = Point2f(0, originalPersp.rows);

	outputQuad[0] = Point2f(px1, py1);
	outputQuad[1] = Point2f(originalPersp.cols - px2, py2);
	outputQuad[2] = Point2f(originalPersp.cols - px3, originalPersp.rows - py3);
	outputQuad[3] = Point2f(px4, originalPersp.rows - py4);

	lambda = getPerspectiveTransform(inputQuad, outputQuad);
	warpPerspective(originalPersp, resultPersp, lambda, resultPersp.size());

	resultPersp.copyTo(*auxDestPersp);

	string name = "Results/Skew" + to_string(indPersp) + ".jpg";
	indPersp++;
	cv::imwrite(name, resultPersp);
	cv::imshow("Result window", resultPersp);
}

void trackbar_perspx1(int value, void* userdata)
{
	px1 = value;
	refreshPersp();
}

void trackbar_perspy1(int value, void* userdata)
{
	py1 = value;
	refreshPersp();
}

void trackbar_perspx2(int value, void* userdata)
{
	px2 = value;
	refreshPersp();
}

void trackbar_perspy2(int value, void* userdata)
{
	py2 = value;
	refreshPersp();
}

void trackbar_perspx3(int value, void* userdata)
{
	px3 = value;
	refreshPersp();
}

void trackbar_perspy3(int value, void* userdata)
{
	py3 = value;
	refreshPersp();
}

void trackbar_perspx4(int value, void* userdata)
{
	px4 = value;
	refreshPersp();
}

void trackbar_perspy4(int value, void* userdata)
{
	py4 = value;
	refreshPersp();
}

void perspective(Mat _src, Mat &_dst)
{
	_src.copyTo(resultPersp);
	_src.copyTo(originalPersp);
	resultPersp.copyTo(_dst);
	auxDestPersp = &_dst;

	try
	{
		createTrackbar("x1", "Source Image", NULL, originalPersp.cols / 2, trackbar_perspx1, &resultPersp);
		setTrackbarPos("x1", "Source Image", 0);
		createTrackbar("y1", "Source Image", NULL, originalPersp.rows / 2, trackbar_perspy1, &resultPersp);
		setTrackbarPos("y1", "Source Image", 0);
		createTrackbar("x2", "Source Image", NULL, originalPersp.cols / 2, trackbar_perspx2, &resultPersp);
		setTrackbarPos("x2", "Source Image", 0);
		createTrackbar("y2", "Source Image", NULL, originalPersp.rows / 2, trackbar_perspy2, &resultPersp);
		setTrackbarPos("y2", "Source Image", 0);
		createTrackbar("x3", "Source Image", NULL, originalPersp.cols / 2, trackbar_perspx3, &resultPersp);
		setTrackbarPos("x3", "Source Image", 0);
		createTrackbar("y3", "Source Image", NULL, originalPersp.rows / 2, trackbar_perspy3, &resultPersp);
		setTrackbarPos("y3", "Source Image", 0);
		createTrackbar("x4", "Source Image", NULL, originalPersp.cols / 2, trackbar_perspx4, &resultPersp);
		setTrackbarPos("x4", "Source Image", 0);
		createTrackbar("y4", "Source Image", NULL, originalPersp.rows / 2, trackbar_perspy4, &resultPersp);
		setTrackbarPos("y4", "Source Image", 0);
	}
	catch (cv::Exception& e)
	{
		const char* err_msg = e.what();
		std::cout << "exception caught: " << err_msg << std::endl;
	}
}