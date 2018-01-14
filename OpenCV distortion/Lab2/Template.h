#pragma once
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <stdlib.h>

using namespace cv;
using namespace std;

void match(InputArray _src, InputArray _temp, OutputArray _dst);