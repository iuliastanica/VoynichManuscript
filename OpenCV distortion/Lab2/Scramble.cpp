#include "stdafx.h"
#include "Scramble.h"
#include "SaveWords.h"

Mat result, original, *auxDest;
int roi_w, roi_h;
int imgIndice = 0;

Mat getResultScram()
{
	return result;
}

void chooseRand(Mat &roi1, Mat &roi2)
{
	int top1, left1, top2, left2; //random corners for ROIs
	top1 = rand() % (result.rows - roi_h);
	top2 = rand() % (result.rows - roi_h);
	left1 = rand() % (result.cols - roi_w);
	left2 = rand() % (result.cols - roi_w);
	roi1 = result(Rect(left1, top1, roi_w, roi_h));
	roi2 = result(Rect(left2, top2, roi_w, roi_h));

	for (int i = 0; i < roi1.cols; i++)
		for (int j = 0; j < roi1.rows; j++)
		{
			if (roi1.at<uchar>(j, i) <= 2 || roi2.at<uchar>(j, i) <= 2) //don't choose from black background
			{
				top1 = rand() % (result.rows - roi_h);
				top2 = rand() % (result.rows - roi_h);
				left1 = rand() % (result.cols - roi_w);
				left2 = rand() % (result.cols - roi_w);
				roi1 = result(Rect(left1, top1, roi_w, roi_h));
				roi2 = result(Rect(left2, top2, roi_w, roi_h));
				i = 0; j = 0;
			}
			//other restrictions
		}
}

void chooseWords()//Mat &roi1, Mat &roi2)
{
	vector<Rect> rects = getWords(original);
	Mat imgRect;
	original.copyTo(imgRect);

	namedWindow("imgRect", CV_WINDOW_KEEPRATIO);
	imshow("imgRect", imgRect);
}

void trackbar_callback_scr(int value, void* userdata)
{
	original.copyTo(result);
	cvtColor(result, result, CV_BGR2GRAY);
	for (int i = 0; i < value; i++)
	{
		Mat roi1, roi2;
		chooseRand(roi1, roi2);

		Mat aux;
		roi1.copyTo(aux);
		roi2.copyTo(roi1);
		aux.copyTo(roi2);
	}
	result.copyTo(*auxDest);

	string name = "Results/ImageScramble" + to_string(value) + "_" + to_string(imgIndice) + ".jpg";
	imgIndice++;
	imwrite(name, result);

	imshow("Result window", result);
}

//width, height of the ROI
void scramble(Mat _src, Mat &_dst, int w, int h)
{
	srand(time(NULL));
	roi_w = w;
	roi_h = h;

	_src.copyTo(result);
	_src.copyTo(original);
	cvtColor(result, result, CV_BGR2GRAY);
	result.copyTo(_dst);
	auxDest = &_dst;

	//chooseWords();

	try
	{
		createTrackbar("Scramble", "Source Image", NULL, 30, trackbar_callback_scr, &result);
		setTrackbarPos("Scramble", "Source Image", 5);
		//result.copyTo(_dst);
	}
	catch (cv::Exception& e)
	{
		const char* err_msg = e.what();
		std::cout << "exception caught: " << err_msg << std::endl;
	}
}