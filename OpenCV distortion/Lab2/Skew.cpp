#include "stdafx.h"
#include "Skew.h"

Mat resultSkew, originalSkew, *auxDestSkew;
int indSkew = 0;
int sx1 = 0, sy1 = 0, sx2 = 0, sy2 = 0, sx3 = 0, sy3;

void refreshSkew()
{
	Point2f srcTri[3];
	Point2f dstTri[3];

	Mat rot_mat(2, 3, CV_32FC1);
	Mat warp_mat(2, 3, CV_32FC1);
	Mat warp_dst;

	//original.copyTo(result);

	/// Set the dst image the same type and size as src
	warp_dst = Mat::zeros(originalSkew.rows, originalSkew.cols, originalSkew.type());

	/// Set your 3 points to calculate the  Affine Transform
	srcTri[0] = Point2f(0, 0);
	srcTri[1] = Point2f(originalSkew.cols - 1, 0);
	srcTri[2] = Point2f(0, originalSkew.rows - 1);

	dstTri[0] = Point2f(originalSkew.cols * (sx1 - 50) / 100.0, originalSkew.rows * (sy1 - 50) / 100.0); //originalSkew.rows*0.33
	dstTri[1] = srcTri[1]; // Point2f(originalSkew.cols*0.85, originalSkew.rows*0.25);
	dstTri[2] = srcTri[2]; // Point2f(originalSkew.cols*0.15, originalSkew.rows*0.7);

						   // Get the Affine Transform
	warp_mat = getAffineTransform(srcTri, dstTri);

	/// Apply the Affine Transform just found to the src image
	warpAffine(originalSkew, warp_dst, warp_mat, warp_dst.size());

	/* Rotating the image after Warp

	/// Compute a rotation matrix with respect to the center of the image
	Point center = Point(warp_dst.cols / 2, warp_dst.rows / 2);
	double angle = -50.0;
	double scale = 0.6;

	/// Get the rotation matrix with the specifications above
	rot_mat = getRotationMatrix2D(center, angle, scale);

	/// Rotate the warped image
	warpAffine(warp_dst, resultSkew, rot_mat, warp_dst.size());*/

	warp_dst.copyTo(resultSkew);

	resultSkew.copyTo(*auxDestSkew);

	string name = "Results/Skew" + to_string(indSkew) + ".jpg";
	indSkew++;
	cv::imwrite(name, resultSkew);

	cv::imshow("Result window", resultSkew);
}

void trackbar_callback_skewx1(int value, void* userdata)
{
	sx1 = value;
	refreshSkew();
}

void trackbar_callback_skewy1(int value, void* userdata)
{
	sy1 = value;
	refreshSkew();
}
/*
void trackbar_callback_skewx2(int value, void* userdata)
{
	sx2 = value;
	refreshSkew();
}

void trackbar_callback_skewy2(int value, void* userdata)
{
	sy2 = value;
	refreshSkew();
}

void trackbar_callback_skewx3(int value, void* userdata)
{
	sx3 = value;
	refreshSkew();
}

void trackbar_callback_skewy3(int value, void* userdata)
{
	sy3 = value;
	refreshSkew();
}
*/
void skew(Mat _src, Mat &_dst)
{
	_src.copyTo(resultSkew);
	_src.copyTo(originalSkew);
	resultSkew.copyTo(_dst);
	auxDestSkew = &_dst;

	try
	{
		createTrackbar("x1", "Source Image", NULL, 100, trackbar_callback_skewx1, &resultSkew);
		setTrackbarPos("x1", "Source Image", 50);
		createTrackbar("y1", "Source Image", NULL, 100, trackbar_callback_skewy1, &resultSkew);
		setTrackbarPos("y1", "Source Image", 50);
		/*
		createTrackbar("x2", "Source Image", NULL, 50, trackbar_callback_skewx2, &resultSkew);
		setTrackbarPos("x2", "Source Image", 0);
		createTrackbar("y2", "Source Image", NULL, 50, trackbar_callback_skewy2, &resultSkew);
		setTrackbarPos("y2", "Source Image", 0);

		createTrackbar("x3", "Source Image", NULL, 50, trackbar_callback_skewx3, &resultSkew);
		setTrackbarPos("x3", "Source Image", 0);
		createTrackbar("y3", "Source Image", NULL, 50, trackbar_callback_skewy3, &resultSkew);
		setTrackbarPos("y3", "Source Image", 0);
		*/
	}
	catch (cv::Exception& e)
	{
		const char* err_msg = e.what();
		std::cout << "exception caught: " << err_msg << std::endl;
	}
}