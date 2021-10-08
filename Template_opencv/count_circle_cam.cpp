#include "stdafx.h"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <cmath>
#include <iostream>
using namespace std;
using namespace cv;
int main()
{
	VideoCapture cap(0);
	while (1)
	{
		Mat src;
		cap >> src;
		Mat gray;
		cvtColor(src, gray, COLOR_BGR2GRAY);
		medianBlur(gray, gray, 5);
		vector<Vec3f> circles;
		HoughCircles(gray, circles, HOUGH_GRADIENT, 1, 150,
			// change this value to detect circles with different distances to each other

			100, 30, 0, 90 // change the last two parameters

			// (min_radius & max_radius) to detect larger circles
			);

		int jum = 0;

		for (size_t i = 0; i < circles.size(); i++)
		{
			jum++;
			Vec3i c = circles[i];
			Point center = Point(c[0], c[1]);
			// circle center
			circle(src, center, 1, Scalar(0, 100, 100), 3, LINE_AA);
			// circle outline
			int radius = c[2];
			circle(src, center, radius, Scalar(255, 0, 0), 3, LINE_AA);
		}
		string obyeks;
		obyeks = to_string(jum);
		putText(src, //target image
			"Jumlah :" + obyeks, //text
			Point(10, 100), //top-left position
			FONT_HERSHEY_DUPLEX,
			1.0,
			CV_RGB(118, 185, 0), //font color
			2);
		imshow("Uang", src);
		if (waitKey(1) >= 0) break;
	}
}