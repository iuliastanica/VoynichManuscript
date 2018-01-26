#include "stdafx.h"
#include "Template.h"
#include "Window.h"

string n = "Templates/Temp1.jpg";
Mat r, orig, img_display;
Mat templ = imread(n);
int imagVal = 0;
int thres = 0;
void match(int, void*);

void matching(Mat src)
{
	src.copyTo(orig);
	match(0, 0);

	try
	{
		createTrackbar("Template", "Source Image", &imagVal, 3, match);
		createTrackbar("Threshold", "Source Image", &thres, 5, match);
		setTrackbarPos("Template", "Source Image", 0);
		setTrackbarPos("Threshold", "Source Image", 2);
	}
	catch (cv::Exception& e)
	{
		const char* err_msg = e.what();
		std::cout << "exception caught: " << err_msg << std::endl;
	}
}


void ReadTemplate(int, void*)
{
	n = "Templates/Temp" + Str(imagVal + 1) + ".jpg";
	templ = imread(n);

}

void match(int, void*)
{
	ReadTemplate(0, 0);
	orig.copyTo(img_display);

	/// Create the result matrix (R from doc)
	int result_cols = img_display.cols - templ.cols + 1;
	int result_rows = img_display.rows - templ.rows + 1;

	r.create(result_rows, result_cols, CV_32FC1);

	//matrix copy for results
	matchTemplate(img_display, templ, r, CV_TM_CCOEFF_NORMED);
	normalize(r, r, 0, 1, NORM_MINMAX, -1, Mat());

	//a threshold
	double minval, maxval, threshold;
	threshold = 1 - (thres + 1) * 0.1;

	Point minloc, maxloc, matchloc;

	while (true)
	{
		minMaxLoc(r, &minval, &maxval, &minloc, &maxloc);

		if (maxval >= threshold)
		{
			matchloc = maxloc;

			rectangle(
				img_display,
				matchloc,
				Point(matchloc.x + templ.cols, matchloc.y + templ.rows),
				CV_RGB(0, 255, 0), 2
			);

			rectangle(
				r,
				matchloc,
				Point(matchloc.x + templ.cols, matchloc.y + templ.rows),
				CV_RGB(255, 255, 255), 2
			);

			floodFill(r, maxloc, cv::Scalar(0), 0, cv::Scalar(.1), cv::Scalar(1.));
		}
		else
			break;
	}

	imshow("Temp", templ);
	namedWindow("Result", CV_WINDOW_KEEPRATIO);
	imshow("Result", img_display);
}