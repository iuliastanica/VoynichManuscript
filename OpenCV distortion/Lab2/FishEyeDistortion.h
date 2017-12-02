#pragma once
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;


float calc_shift(float x1, float x2, float cx, float k);
float getRadialX(float x, float y, float cx, float cy, float k, bool scale, Vec4f props);
float getRadialY(float x, float y, float cx, float cy, float k, bool scale, Vec4f props);

// Cx and Cy specify the coordinates from where the distorted image will have as initial point and 
// k specifies the distortion factor
void fishEye(InputArray _src, OutputArray _dst, double Cx, double Cy, double k, bool scale = true);
