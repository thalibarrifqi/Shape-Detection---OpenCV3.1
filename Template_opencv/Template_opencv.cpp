// Template_opencv.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <opencv2\opencv.hpp>
using namespace cv;


int _tmain(int argc, _TCHAR* argv[])
{
	VideoCapture cap(1); // open the default camera
	if (!cap.isOpened()) // check if we succeeded
		return -1;
	Mat edges;
	namedWindow("edges", 1);
	for (;;)
	{
		Mat frame;
		cap >> frame; // get a new frame from camera
		cvtColor(frame, edges, COLOR_BGR2GRAY);
		GaussianBlur(edges, edges, Size(7, 7), 1.5, 1.5);
		Canny(edges, edges, 0, 30, 3);
		imshow("edges", edges);
		if (waitKey(1) >= 0) break;
	}
	// the camera will be deinitialized automatically in VideoCapture destructor
	return 0;
}

