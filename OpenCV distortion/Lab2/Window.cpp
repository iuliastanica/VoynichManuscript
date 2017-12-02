#include "stdafx.h"
#include "Window.h"

Window::Window()
{
}

Window::Window(string shape, int no_rows, int no_cols, int stepRows, int stepCols)
{
	nr = 0;
	this -> shape = shape;
	this -> no_rows = no_rows;
	this -> no_cols = no_cols;
	this -> stepRows = stepRows;
	this-> stepCols = stepCols;
}

Window::Window(string shape, int no_rows, int no_cols)
{
	nr = 0;
	this->shape = shape;
	this->no_rows = no_rows;
	this->no_cols = no_cols;
}

Window::~Window()
{
}

void Window::setName(string name)
{
	this->shape = name;
}

void Window::setNoRows(int rows)
{
	this->no_rows = rows;
}

void Window::setNoCols(int cols)
{
	this->no_cols = cols;
}

Mat Window::getWindowRect(Mat src, int pos_x, int pos_y)
{
	// resulting window   
	Rect windows(pos_x - no_cols /2, pos_y - no_rows/2, no_cols, no_rows);

	Mat result = src.clone();
	rectangle(result, windows, Scalar(255), 1, 8, 0);

	Mat roi = src(windows);
	nr++;
	string s = "test" + Str(nr) + ".jpg";

	return roi;

	//namedWindow(s, CV_WINDOW_KEEPRATIO);
	//imwrite(s, Roi);
	//imshow(s, Roi);
	
	
}

Mat Window::getWindowCircle(Mat src, int pos_x, int pos_y)
{
	// resulting window   
	Mat roi;

	Mat result = src.clone();
	Mat mask = cv::Mat::zeros(no_rows, no_cols, CV_32FC1);

	circle(mask, Point(pos_x, pos_y), no_rows, Scalar(255), -1, 8, 0);
	//ellipse(mask, RotatedRect(Point2f(pos_x - no_cols / 2, pos_y - no_rows / 2), Size2f(no_cols, no_rows), 60.0f), Scalar(255), 1, 8);

	result.copyTo(roi, mask);
	nr++;
	string s = "test" + Str(nr) + ".jpg";

	return roi;

	//namedWindow(s, CV_WINDOW_KEEPRATIO);
	//imwrite(s, Roi);

	//imshow(s, Roi);


}
