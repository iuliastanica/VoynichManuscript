#include "stdafx.h"
#include "SaveWords.h"


/// Global variables


void saveWords(Mat _src){
	int const max_value = 255;
	int const max_type = 2;
	int const max_BINARY_value = 255;
	Mat src, dst;
	_src.copyTo(src);
	if (!src.data)
	{
		cout << "Could not open or find the image" << std::endl;
		int a; cin >> a;		return;
	}

	try
	{
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
		vector<Rect> r(size);

		for (int i = 0; i < size; i++)
		{
			/// assuming words are more lenght-y than tall
			if (minRect[i].width != 0 && minRect[i].height <= 3 * h) {
				r[rL] = minRect[i];
				rL++;
				rectangle(drawing, minRect[i], Scalar(255,255,255), 1, 8, 0);
			}
		}

		Mat rects = Mat::zeros(canny_output.size(), CV_8UC3);



		/////////////////////////////////////////PROBLEM PART///////////////////////////////////////////////////////////
		/// Merge rectangles
		/*Rect r1, r2;

		for (int ir = 0;ir<r.size() - 1;ir += 1)
		for (int jr = ir + 1;jr < r.size();jr++) {
		r1 = r[ir], r2 = r[jr];
		if((r1 & r2).area()>0 )
		if((r1 & r2).area() == r1.area() || (r1 & r2).area() == r2.area() || r2.br().x < r1.tl().x || r1.br().x < r2.tl().x)
		{
		r[ir] = r1 | r2;
		r.erase(r.begin() + jr);
		jr--;
		}

		}
		*/
		/////////////////////////////////////////PROBLEM PART///////////////////////////////////////////////////////////



		////////////////////Saving rects/////////////////////////
		int filenumber = 0;
		string filename;



		for (int i = 0;i < r.size();i++) {

			Rect roi = r[i];

			Mat maskROI(dst, roi);

			// ratio of non-zero pixels in the filled region
			double rat = (double)countNonZero(maskROI) / (roi.width*roi.height);
			//cout << "\n Ratio: "<< rat << "\n";

			Mat crop = src(roi);
			if (roi.area() <= h*h * 4 && rat > 0.45 && roi.width>roi.height)
			{
				stringstream ssfn;
				ssfn << "../Words/" << filenumber << ".jpg";
				filename = ssfn.str();
				filenumber++;
				imwrite(filename, crop);
				
			}
		}
		/////////////////saving rectangles//////////////////////


		/// Show in a window
		namedWindow("Contours", CV_WINDOW_KEEPRATIO);
		imshow("Contours", drawing);
	}


	catch (cv::Exception& e)
	{
		const char* err_msg = e.what();
		std::cout << "exception caught: " << err_msg << std::endl;
	}

}


