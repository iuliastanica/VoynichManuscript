#include "stdafx.h"
#include "CylinderDistortion.h"

void trackbar_callback_cylinder(int value, void* userdata)
{
	Mat* image = (Mat*)userdata;

	const float width = (float)image->rows;
	const float height = (float)image->cols;

	Mat result(image->rows, image->cols, image->type());

	//cylindrical anamorphosis
	//https://suw.biblos.pk.edu.pl/downloadResource&mId=1626326

	for (int i = 0; i < image->rows; i++) {
		for (int j = 0; j < image->cols; j++) {

			float x = (j / height) - 0.5f;
			float y = (i / width) - 0.5f;

			float angle = atan2f(y, x);
			float radius = sqrtf((x * x) + (y * y));

			float R;//radius of the cylinder of revolution
			R = radius / value *300.0f; //300f -> value found by trying

										//centrul (bazei) cilindrului
										//TODO: get coordonates by clicking
			float xp, yp;
			xp = x;
			yp = y;
			float gamma = asin(yp / R);

			float xr = width*((R * cosf(gamma)) + (R * cosf(gamma) - xp)*cosf(2 * gamma));
			float yr = height*(yp + (R*cos(gamma) - xp)*sinf(2 * gamma));

			int k = (int)std::min(width - 1, std::max(0.0f, xr));
			int m = (int)std::min(height - 1, std::max(0.0f, yr));

			uchar* src = image->ptr<uchar>(k, m);
			uchar* out = result.ptr<uchar>(i, j);

			out[0] = src[0];
			out[1] = src[1];
			out[2] = src[2];
		}
	}

	imshow("Result window", result);
}