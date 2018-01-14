#include "stdafx.h"
#include "Template.h"

void match(InputArray _src, InputArray _temp, OutputArray _dst)
{
	Mat src = _src.getMat();
	cvtColor(src, src, CV_BGR2GRAY);

	Mat temp = _temp.getMat();
	cvtColor(temp, temp, CV_BGR2GRAY);

	Mat result, resRec;
	src.copyTo(result);

	Mat img_display;
	src.copyTo(img_display);

	/// Create the result matrix
	int result_cols = src.cols - temp.cols + 1;
	int result_rows = src.rows - temp.rows + 1;

	result.create(result_rows, result_cols, CV_32FC1);

	/// Do the Matching and Normalize
	matchTemplate(src, temp, result, 1);
	normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());

	/// Localizing the best match with minMaxLoc
	double minVal; double maxVal; Point minLoc; Point maxLoc;
	Point matchLoc;

	minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());
	matchLoc = minLoc;

	/// Display rectangles inside pics
	rectangle(img_display, matchLoc, Point(matchLoc.x + temp.cols, matchLoc.y + temp.rows), Scalar(0, 255, 0), 2, 8, 0);
	rectangle(result, matchLoc, Point(matchLoc.x + temp.cols, matchLoc.y + temp.rows), Scalar::all(0), 2, 8, 0);

	imshow("Source Image", img_display);
	imshow("Result window", result);

	return;
}