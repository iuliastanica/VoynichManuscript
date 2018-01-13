#include "stdafx.h"
#include "Scramble.h"

void chooseRand(const Mat &result, Mat &roi1, Mat &roi2, int w, int h)
{
	int top1, left1, top2, left2; //random corners for ROIs
	top1 = rand() % (result.rows - h);
	top2 = rand() % (result.rows - h);
	left1 = rand() % (result.cols - w);
	left2 = rand() % (result.cols - w);
	roi1 = result(Rect(left1, top1, w, h));
	roi2 = result(Rect(left2, top2, w, h));

	for (int i = 0; i < roi1.cols; i++)
		for (int j = 0; j < roi1.rows; j++)
			if (roi1.at<uchar>(j, i) <= 2 || roi2.at<uchar>(j, i) <= 2) //don't choose from black background
			{
				top1 = rand() % (result.rows - h);
				top2 = rand() % (result.rows - h);
				left1 = rand() % (result.cols - w);
				left2 = rand() % (result.cols - w);
				roi1 = result(Rect(left1, top1, w, h));
				roi2 = result(Rect(left2, top2, w, h));
				i = 0; j = 0;
			}
}

/*
void detectFeatures(Mat &result)
{
	int minHessian = 400;

	SurfFeatureDetector detector(minHessian);

	std::vector<KeyPoint> keypoints_1;

	detector.detect(result, keypoints_1);

	Mat img_keypoints_1;
	drawKeypoints(result, keypoints_1, img_keypoints_1, Scalar::all(-1), DrawMatchesFlags::DEFAULT);

	imshow("Keypoints 1", img_keypoints_1);
}
*/

//width, height of the ROI
void scramble(InputArray _src, OutputArray _dst, int w, int h, int num)
{
	srand(time(NULL));
	Mat src = _src.getMat();
	cvtColor(src, src, CV_BGR2GRAY);
	Mat result, resRec;
	src.copyTo(result);

	for (int i = 0; i < num; i++)
	{
		Mat roi1, roi2;
		chooseRand(result, roi1, roi2, w, h);

		Mat aux;
		roi1.copyTo(aux);
		roi2.copyTo(roi1);
		aux.copyTo(roi2);

		//rectangle(result, Rect(left1, top1, w + 1, h + 1), Scalar(0, 0, 255), 2, 8);
		//rectangle(result, Rect(left2, top2, w + 1, h + 1), Scalar(0, 0, 255), 2, 8);
	}

	result.copyTo(_dst);
}