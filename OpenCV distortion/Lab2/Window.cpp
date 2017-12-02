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

Window::~Window()
{
}

void Window::getWindowRect(Mat src, int pos_x, int pos_y)
{
	// resulting window   
	Rect windows(pos_x - no_rows/2, pos_y - no_cols/2, no_rows, no_cols);

	Mat result = src.clone();
	rectangle(result, windows, Scalar(255), 1, 8, 0);

	Mat Roi = src(windows);
	nr++;
	string s = "test" + Str(nr) + ".jpg";

	namedWindow(s, CV_WINDOW_KEEPRATIO);
	imwrite(s, Roi);

	imshow(s, Roi);
	
	
}
