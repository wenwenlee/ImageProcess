#include<iostream>
#include<opencv.hpp>
#include"histequal.h"

using namespace std;
using namespace cv;


int main(int argc, char **argv)
{
	Mat src = imread(argv[1],0);
	namedWindow("src", CV_WINDOW_NORMAL);
	imshow("src", src);
	Mat equal = imread(argv[2],0);
	namedWindow("equal", CV_WINDOW_NORMAL);
	imshow("equal", equal);
	

	Mat HistEqual = ycMatchHist(src, equal);
	namedWindow("HistEqual", CV_WINDOW_NORMAL);
	imshow("HistEqual", HistEqual);
	waitKey();
}