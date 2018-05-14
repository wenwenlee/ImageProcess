#ifndef _FFT_H
#define _FFT_H
#include<iostream>
#include<opencv.hpp>

using namespace std;
using namespace cv;

int DFT(const Mat& src, Mat& dst)
{
	/*对输入图像进行检查*/
	if (!src.data)
	{
		cerr << "请检查图像是否存在！" << endl;
		return -1;

	}
	switch (src.channels())
	{
		case 3:
			cvtColor(src, src, COLOR_BGR2GRAY);
			break;
		case 4:
			cvtColor(src, src, COLOR_BGRA2GRAY);
			break;
		default:
			break;
	}
	
	

	//Mat padded;                 //以0填充输入图像矩阵
	//int m = getOptimalDFTSize(I.rows);
	//int n = getOptimalDFTSize(I.cols);
}



#endif
