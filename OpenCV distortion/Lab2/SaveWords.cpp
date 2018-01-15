

#include "stdafx.h"
#include "SaveWords.h"


/// Global variables
int kr=0;

vector<Rect> getWords(Mat _src) {
	int const max_value = 255;
	int const max_type = 2;
	int const max_BINARY_value = 255;
	Mat src, dst;
	_src.copyTo(src);
	Mat src_gray;
	int cols = src.cols;
	int rows = src.rows;
	cvtColor(src, src_gray, CV_BGR2GRAY);

	///filter noise
	bilateralFilter(src_gray, dst, 9, 30, 30);


	///binarization
	// Set maxValue, blockSize and c (constant value)
	double maxValue = 255;
	int blockSize = 9;

	double c = 11;
	// Adaptive Threshold
	adaptiveThreshold(dst, dst, maxValue, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY_INV, blockSize, c);


	/// morphological operations to unite letters and separate words
	///
	int dilation_size = 6, erosion_size = 1;
	Mat element = getStructuringElement(MORPH_RECT,
		Size(2 * dilation_size + 1, 1),
		Point(-1, -1));
	/// Apply the dilation operation
	dilate(dst, dst, element);
	Mat element2 = getStructuringElement(MORPH_RECT,
		Size(1, 2 * erosion_size + 1),
		Point(-1, -1));
	/// Apply the erosion operation
	erode(dst, dst, element2);
	Mat element3 = getStructuringElement(MORPH_RECT,
		Size(2 * 2 + 1, 1),
		Point(-1, -1));
	dilate(dst, dst, element3);


	///Contour detection
	int thresh = 30;
	RNG rng(12345);
	Mat canny_output;
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	vector<Vec4f> fitLines;

	/// Detect edges using canny
	Canny(dst, canny_output, thresh, thresh * 2, 3);
	/// Find contours
	findContours(canny_output, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

	///rectangles
	vector<Rect> minRect(contours.size());
	/// a rect for each contour

	/// Draw contours
	Mat drawing = Mat::zeros(canny_output.size(), CV_8UC3);
	//discard specific contours -> too large or too small
	int contoursThresh = cols + rows;
	int i = 0;
	int nR = 0, h = 0;
	for (;i<contours.size();)
	{
		vector<Point> cnt = contours[i];
		int l = arcLength(cnt, 0);



		if (l >= contoursThresh / 10)
		{
			Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
			minRect[i] = boundingRect(cnt);
			h += minRect[i].height;
			nR++;

			drawContours(drawing, contours, i, color, CV_FILLED, 8, hierarchy, 0, Point());

			//bounding rectangles for contours
			rectangle(drawing, minRect[i], color, 1, 8, 0);
		}
		i++;
	}

	//approximate space between words with median height of word
	if (nR == 0)
		nR = 1;
	h = h / nR; cout << h;

	//in r we store words rectangles
	int size = minRect.size(), rL = 0;
	vector<Rect> rects(size);

	//int kr = 0;
	for (int i = 0;i < minRect.size();i++) {

		Rect roi = minRect[i];

		Mat maskROI(dst, roi);

		// ratio of non-zero pixels in the filled region
		double rat = (double)countNonZero(maskROI) / (roi.width*roi.height);
		//cout << "\n Ratio: "<< rat << "\n";

		Mat crop = src(roi);
		if (roi.width != 0 && roi.height <= 3 * h && roi.area() <= h*h * 4 && rat > 0.45 && roi.width>roi.height)
		{
			rects[kr] = roi; 
			//cout << "\n" << rects[kr].x << " " << rects[kr].y;
			kr++;
		}
	}

	/// Show in a window
	namedWindow("Contours", CV_WINDOW_KEEPRATIO);
	imwrite("../Words/_contours.jpg", drawing);
	imshow("Contours", drawing);

	return rects;
}

void saveWords(Mat _src) {
	vector<Rect> rects = getWords(_src);
	int filenumber = 0;
	string filename;


	for (int i = 0;i < kr;i++) {

		Rect roi = rects[i];
		Mat crop = _src(roi);

		stringstream ssfn;
		ssfn << "../Words/" << filenumber << "word.jpg";
		filename = ssfn.str();
		filenumber++;
		imwrite(filename, crop);


	}
}



