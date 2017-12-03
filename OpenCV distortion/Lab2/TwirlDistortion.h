#pragma once
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

void trackbar_callback1(int value, void* userdata);
void trackbar_callback0(int value, void* userdata);
Mat getResult();
