#include "stdafx.h"
#include "TwirlDistortion.h"

Mat res;

Mat getResult()
{
	return res;
}

void trackbar_callback1(int value, void* userdata) //big
{
	Mat* image = (Mat*)userdata;

	const float width = (float)image->rows;
	const float height = (float)image->cols;

	Mat result(image->rows, image->cols, image->type());
	result.copyTo(res);

	for (int i = 0; i < image->rows; i++) {
		for (int j = 0; j < image->cols; j++) {

			float x = (j / height) - 0.5f;
			float y = (i / width) - 0.5f;

			float angle = atan2f(y, x);
			float radius = sqrtf((x * x) + (y * y));

			angle += radius * (value / 10.0f);

			float xr = ((radius * sinf(angle)) + 0.5f) * width;
			float yr = ((radius * cosf(angle)) + 0.5f) * height;

			int k = (int)std::min(width - 1, std::max(0.0f, xr));
			int m = (int)std::min(height - 1, std::max(0.0f, yr));

			uchar* src = image->ptr<uchar>(k, m);
			uchar* out = result.ptr<uchar>(i, j);

			out[0] = src[0];
			out[1] = src[1];
			out[2] = src[2];
		}
	}
	//namedWindow("Result image", CV_WINDOW_KEEPRATIO);
	res = result;
	//imwrite("Result.jpg", res);
	imshow("Result window", result);
}

void trackbar_callback0(int value, void* userdata) //small
{
	Mat* image = (Mat*)userdata;

	const float width = (float)image->rows;
	const float height = (float)image->cols;

	Mat result(image->rows, image->cols, image->type());
	result.copyTo(res);

	for (int i = 0; i < image->rows; i++) {
		for (int j = 0; j < image->cols; j++) {

			float x = (j / height) - 0.5f;
			float y = (i / width) - 0.5f;

			float angle = atan2f(y, x);
			float radius = sqrtf((x * x) + (y * y));

			angle += radius * (value / 10.0f);

			float xr = ((radius * sinf(angle)) + 0.5f) * width;
			float yr = ((radius * cosf(angle)) + 0.5f) * height;

			int k = (int)std::min(width - 1, std::max(0.0f, xr));
			int m = (int)std::min(height - 1, std::max(0.0f, yr));

			uchar* src = image->ptr<uchar>(k, m);
			uchar* out = result.ptr<uchar>(i, j);

			out[0] = src[0];
			out[1] = src[1];
			out[2] = src[2];
		}
	}
	//namedWindow("Result image", CV_WINDOW_KEEPRATIO);
	res = result;
	//imwrite("Result.jpg", res);
	imshow("Roi", result);
}