#pragma once
#include <string>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

template <typename T>
string Str(const T & t) {
	ostringstream os;
	os << t;
	return os.str();
}

class Window
{
private:
	string shape;
	int no_rows;
	int no_cols;
	int stepRows;
	int stepCols;

public:
	int nr;
	Window();
	Window(string shape, int no_rows, int no_cols, int stepRows, int stepCols);
	Window(string shape, int no_rows, int no_cols);

	Mat getWindowRect(Mat src, int pos_x, int pos_y);
	Mat getWindowCircle(Mat src, int pos_x, int pos_y);
	void setName(string name);
	void setNoRows(int rows);
	void setNoCols(int cols);

	~Window();
};

