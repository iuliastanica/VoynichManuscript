#include "stdafx.h"
#include "Template.h"

Mat r, orig;

void match(Mat src, Mat templ)
{
/*	Mat img_display;
	src.copyTo(img_display);

	/// Create the result matrix (R from doc)
	int result_cols = src.cols - templ.cols + 1;
	int result_rows = src.rows - templ.rows + 1;

	r.create(result_rows, result_cols, CV_32FC1);

	//another result matrix that we'll modify
	Mat res(src.rows - templ.rows + 1, src.cols - templ.cols + 1, CV_32FC1);

	matchTemplate(src, templ, res, 2);
	normalize(res, res, 0, 1, NORM_MINMAX, -1, Mat());

	double val = 0.5;

	threshold(res, res, val, 1., CV_THRESH_TOZERO);

	//matrix copy for results
	matchTemplate(src, templ, r, 2);
	normalize(r, r, 0, 1, NORM_MINMAX, -1, Mat());

	//a threshold
	double minval, maxval, threshold;
	threshold = 0.2;

	Point minloc, maxloc;

	while (true)
	{
		minMaxLoc(r, &minval, &maxval, &minloc, &maxloc);
		minMaxLoc(res, &minval, &maxval, &minloc, &maxloc);

		if (maxval >= threshold)
		{
			rectangle(
				img_display,
				maxloc,
				Point(maxloc.x + templ.cols, maxloc.y + templ.rows),
				CV_RGB(0, 255, 0), 2
			);

			rectangle(
				r,
				maxloc,
				Point(maxloc.x + templ.cols, maxloc.y + templ.rows),
				CV_RGB(255, 255, 255), 2
			);

			floodFill(res, maxloc, cv::Scalar(0), 0, cv::Scalar(.1), cv::Scalar(1.));
		}
		else
			break;
	}
	namedWindow("Result", CV_WINDOW_KEEPRATIO);
	imshow("Result", img_display);*/


	Mat img_display;
	src.copyTo(img_display);

	/// Create the result matrix (R from doc)
	int result_cols = src.cols - templ.cols + 1;
	int result_rows = src.rows - templ.rows + 1;

	r.create(result_rows, result_cols, CV_32FC1);

	/// Do the Matching and Normalize
	matchTemplate(src, templ, r, 0);
	normalize(r, r, 0, 1, NORM_MINMAX, -1, Mat());

	/// Localizing the best match with minMaxLoc
	double minVal; double maxVal; Point minLoc; Point maxLoc;
	Point matchLoc;

	minMaxLoc(r, &minVal, &maxVal, &minLoc, &maxLoc, Mat());
	matchLoc = minLoc;

	/// Display rectangles inside pics
	rectangle(img_display, matchLoc, Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows), Scalar(0, 255, 0), 2, 8, 0);
	rectangle(r, matchLoc, Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows), Scalar::all(0), 2, 8, 0);

	imshow("Temp", templ);
	namedWindow("Result", CV_WINDOW_KEEPRATIO);
	imshow("Result", img_display);


	return;
}