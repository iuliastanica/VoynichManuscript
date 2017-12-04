//cylindrical anamorphosis
//https://suw.biblos.pk.edu.pl/downloadResource&mId=1626326

#include "stdafx.h"
#include "CylinderDistortion.h"

Mat resCyl;

Mat getCylResult()
{
	return resCyl;
}


void cyl_callback1(int value, void* userdata) //big
{
	Mat* image = (Mat*)userdata;

	const float width = (float)image->rows;
	const float height = (float)image->cols;

	Mat result(image->rows, image->cols, image->type());
	result.copyTo(resCyl);


	for (int i = 0; i < image->rows; i++) {
		for (int j = 0; j < image->cols; j++) {

			float x = (j / height) - 0.5f;
			float y = (i / width) - 0.5f;

			float angle = atan2f(y, x);
			float radius = sqrtf((x * x) + (y * y));

			float R;//radius of the cylinder of revolution
			//R = radius / 100.0f * (value / 10.0f);// / (width * 100) / value;  ///(300.0f*width); //// 300f -> value found by testing
			R = radius * 300 * value;
			float xp, yp;
			xp = x;
			yp = y;
			float gamma = asin(yp / R);

			float xr = x*((R * cosf(gamma)) + (R * cosf(gamma) - xp)*cosf(2 * gamma));
			float yr = y*(yp + (R*cos(gamma) - xp)*sinf(2 * gamma));

			int k = (int)std::min(width - 1, std::max(0.0f, xr));
			int m = (int)std::min(height - 1, std::max(0.0f, yr));

			uchar* src = image->ptr<uchar>(k, m);
			uchar* out = result.ptr<uchar>(i, j);

			out[0] = src[0];
			out[1] = src[1];
			out[2] = src[2];
		}
	}
	resCyl = result;
	imshow("Result window", result);
}


void cyl_callback0(int value, void* userdata) //small
{
	Mat* image = (Mat*)userdata;

	const float width = (float)image->rows;
	const float height = (float)image->cols;

	Mat result(image->rows, image->cols, image->type());
	result.copyTo(resCyl);


	for (int i = 0; i < image->rows; i++) {
		for (int j = 0; j < image->cols; j++) {

			float x = (j / height) - 0.5f;
			float y = (i / width) - 0.5f;

			float angle = atan2f(y, x);
			float radius = sqrtf((x * x) + (y * y));

			float R;//radius of the cylinder of revolution
			//R = radius/(300.0f*width);
			R = radius * 200.0f*value;
			//R = radius/100.0f*(value / 10.0f);// / (width * 100) / value;

			float xp, yp;
			xp = x;
			yp = y;
			float gamma = asin(yp / R);

			float xr = x*((R * cosf(gamma)) + (R * cosf(gamma) - xp)*cosf(2 * gamma));
			float yr = y*(yp + (R*cos(gamma) - xp)*sinf(2 * gamma));

			int k = (int)std::min(width - 1, std::max(0.0f, xr));
			int m = (int)std::min(height - 1, std::max(0.0f, yr));

			uchar* src = image->ptr<uchar>(k, m);
			uchar* out = result.ptr<uchar>(i, j);

			out[0] = src[0];
			out[1] = src[1];
			out[2] = src[2];
		}
	}
	resCyl = result;
	imshow("Roi", result);
}