//conical
//https://www.google.ro/url?sa=t&rct=j&q=&esrc=s&source=web&cd=16&ved=0ahUKEwiMz_6TyevXAhWQpaQKHRHEDDA4ChAWCFEwBQ&url=https%3A%2F%2Fsuw.biblos.pk.edu.pl%2FdownloadResource%26mId%3D1930716&usg=AOvVaw1TTHRychrj7czjPqLfKaYo

#include "stdafx.h"
#include "ConeDistortion.h"

Mat resCone;

Mat getConeResult() {
	return resCone;
}

void cone_callback1(int value, void* userdata) //big
{
	Mat* image = (Mat*)userdata;

	const float width = (float)image->rows;
	const float height = (float)image->cols;

	Mat result(image->rows, image->cols, image->type());
	result.copyTo(resCone);

	for (int i = 0; i < image->rows; i++) {
		for (int j = 0; j < image->cols; j++) {
			float t;

			float x = (j / height) - 0.5f;
			float y = (i / width) - 0.5f;

			float angle = atan2f(y, x);
			float radius = sqrtf((x * x) + (y * y));

			float R;
			R = radius * value *100.0f;

			float xp, yp;
			xp = x;
			yp = y;
			float rp = sqrtf(xp*xp + yp*yp);

			float gamma = asin(yp / R);
			float phi = asin(xp / rp);


			t = ((R - rp)*tan(gamma)) / (2 * tan(2 * gamma - 3.14 / 2));

			float xr = width*(t + rp)*cosf(phi);
			float yr = height*(t + rp)*sinf(phi);

			int k = (int)std::min(width - 1, std::max(0.0f, xr));
			int m = (int)std::min(height - 1, std::max(0.0f, yr));

			uchar* src = image->ptr<uchar>(k, m);
			uchar* out = result.ptr<uchar>(i, j);

			out[0] = src[0];
			out[1] = src[1];
			out[2] = src[2];
		}
	}

	resCone = result;

	imshow("Result Image", result);
}


void cone_callback0(int value, void* userdata) //small
{
	Mat* image = (Mat*)userdata;

	const float width = (float)image->rows;
	const float height = (float)image->cols;

	Mat result(image->rows, image->cols, image->type());
	result.copyTo(resCone);

	for (int i = 0; i < image->rows; i++) {
		for (int j = 0; j < image->cols; j++) {
			float t;

			float x = (j / height) - 0.5f;
			float y = (i / width) - 0.5f;

			float angle = atan2f(y, x);
			float radius = sqrtf((x * x) + (y * y));

			float R;
			R = radius * value *100.0f;

			float xp, yp;
			xp = x;
			yp = y;
			float rp = sqrtf(xp*xp + yp*yp);

			float gamma = asin(yp / R);
			float phi = asin(xp / rp);


			t = ((R - rp)*tan(gamma)) / (2 * tan(2 * gamma - 3.14 / 2));

			float xr = width*(t + rp)*cosf(phi);
			float yr = height*(t + rp)*sinf(phi);

			int k = (int)std::min(width - 1, std::max(0.0f, xr));
			int m = (int)std::min(height - 1, std::max(0.0f, yr));

			uchar* src = image->ptr<uchar>(k, m);
			uchar* out = result.ptr<uchar>(i, j);

			out[0] = src[0];
			out[1] = src[1];
			out[2] = src[2];
		}
	}

	resCone = result;
	imshow("Roi", result);
}