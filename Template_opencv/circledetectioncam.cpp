#include "stdafx.h"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <cmath>
#include <iostream>
using namespace cv;
using namespace std;
int main(int argc, char** argv)
{
	Mat frame, frame_gray;
	Mat gray;
	VideoCapture cap(0);
	if (cap.isOpened() == false)
	{
		cout << "Cannot open the video file" << endl;
		cin.get();
		return -1;
	}
	int framewidth = 480;
	int frameheight = 320;
	cap.set(CV_CAP_PROP_FRAME_WIDTH, framewidth);
	cap.set(CV_CAP_PROP_FRAME_HEIGHT, frameheight);
	while (1){
		cap >> frame;
		cap.read(frame);
		cvtColor(frame, gray, COLOR_BGR2GRAY);
		medianBlur(gray, gray, 5);
		vector<Vec3f> circles;
		HoughCircles(gray, circles, HOUGH_GRADIENT, 1, 100, // change this value to detect circles with different distances to each other
			100, 30, 20, 90 // change the last two parameters
			// (min_radius & max_radius) to detect larger circles
			);
		char c = (char)waitKey(1);
		if (c == 27)
			break;
		for (size_t i = 0; i < circles.size(); i++)
		{
			Vec3i c = circles[i];
			Point center = Point(c[0], c[1]);
			// circle center
			circle(frame, center, 1, Scalar(0, 100, 100), 3,

				LINE_AA);

			// circle outline
			int radius = c[2];
			circle(frame, center, radius, Scalar(255, 0, 0), 3,

				LINE_AA);
		}
		imshow("detected circles", frame);
	}
	waitKey();
	return EXIT_SUCCESS;
}