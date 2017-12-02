#include "stdafx.h"
#include "FishEyeDistortion.h"

float calc_shift(float x1, float x2, float cx, float k)
{
	float thresh = 1;
	float x3 = x1 + (x2 - x1)*0.5;
	float res1 = x1 + ((x1 - cx)*k*((x1 - cx)*(x1 - cx)));
	float res3 = x3 + ((x3 - cx)*k*((x3 - cx)*(x3 - cx)));

	//    std::cerr<<"x1: "<<x1<<" - "<<res1<<" x3: "<<x3<<" - "<<res3<<std::endl;

	if (res1>-thresh && res1 < thresh)
		return x1;
	if (res3<0) {
		return calc_shift(x3, x2, cx, k);
	}
	else {
		return calc_shift(x1, x3, cx, k);
	}
}

float getRadialX(float x, float y, float cx, float cy, float k, bool scale, Vec4f props)
{
	float result;
	if (scale)
	{
		float xshift = props[0];
		float yshift = props[1];
		float xscale = props[2];
		float yscale = props[3];

		x = (x*xscale + xshift);
		y = (y*yscale + yshift);
		result = x + ((x - cx)*k*((x - cx)*(x - cx) + (y - cy)*(y - cy)));
	}
	else {
		//        result = (cx+(x-cx)*(1+k*((x-cx)*(x-cx)+(y-cy)*(y-cy))));
		//        or
		result = x + ((x - cx)*k*((x - cx)*(x - cx) + (y - cy)*(y - cy)));
	}
	return result;
}

float getRadialY(float x, float y, float cx, float cy, float k, bool scale, Vec4f props)
{
	float result;
	if (scale)
	{
		float xshift = props[0];
		float yshift = props[1];
		float xscale = props[2];
		float yscale = props[3];

		x = (x*xscale + xshift);
		y = (y*yscale + yshift);
		result = y + ((y - cy)*k*((x - cx)*(x - cx) + (y - cy)*(y - cy)));
	}
	else {
		//        result = (cy+(y-cy)*(1+k*((x-cx)*(x-cx)+(y-cy)*(y-cy))));
		//        or
		result = y + ((y - cy)*k*((x - cx)*(x - cx) + (y - cy)*(y - cy)));
	}
	return result;
}

// Cx and Cy specify the coordinates from where the distorted image will have as initial point and 
// k specifies the distortion factor
void fishEye(InputArray _src, OutputArray _dst, double Cx, double Cy, double k, bool scale)
{
	// die if distortion parameters are not correct
	CV_Assert(Cx >= 0 && Cy >= 0 && k >= 0);

	Mat src = _src.getMat();
	// die if sample image is not the correct type
	//CV_Assert(src.type() == CV_8UC1 || src.type() == CV_8UC3);

	Mat mapx = Mat(src.size(), CV_32FC1);
	Mat mapy = Mat(src.size(), CV_32FC1);

	int w = src.cols;
	int h = src.rows;

	Vec4f props;
	float xShift = calc_shift(0, Cx - 1, Cx, k);
	props[0] = xShift;
	float newCenterX = w - Cx;
	float xShift2 = calc_shift(0, newCenterX - 1, newCenterX, k);

	float yShift = calc_shift(0, Cy - 1, Cy, k);
	props[1] = yShift;
	float newCenterY = w - Cy;
	float yShift2 = calc_shift(0, newCenterY - 1, newCenterY, k);

	float xScale = (w - xShift - xShift2) / w;
	props[2] = xScale;
	float yScale = (h - yShift - yShift2) / h;
	props[3] = yScale;

	float* p = mapx.ptr<float>(0);

	for (int y = 0; y < h; y++)
	{
		for (int x = 0; x < w; x++)
		{
			*p++ = getRadialX((float)x, (float)y, Cx, Cy, k, scale, props);
		}
	}

	p = mapy.ptr<float>(0);
	for (int y = 0; y < h; y++)
	{
		for (int x = 0; x < w; x++)
		{
			*p++ = getRadialY((float)x, (float)y, Cx, Cy, k, scale, props);
		}
	}

	remap(src, _dst, mapx, mapy, CV_INTER_LINEAR, BORDER_CONSTANT);

	//Mat(src).copyTo(_dst);
}
