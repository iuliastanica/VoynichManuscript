#pragma once
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <stdlib.h>
#include <time.h>

using namespace cv;
using namespace std;

void scramble(InputArray _src, OutputArray _dst, int w, int h, int num);