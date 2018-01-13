#pragma once

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

void cyl_callback1(int value, void* userdata);
void cyl_callback0(int value, void* userdata);


Mat getCylResult();