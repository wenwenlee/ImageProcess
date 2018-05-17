#pragma once
#ifndef _FUNCTION_H
#define _FUNCTION_H

#include<iostream>
#include<opencv.hpp>

using namespace std;
using namespace cv;

/*快速傅里叶变换FFT
src 输入图像
dst 输出图像
*/
int FFT(const Mat& src, Mat& dst);

double Entropy(Mat img);

double ComEntropy(Mat img1, Mat img2);

class ComputeCrossEntropy
{
public:
	double getEntropy(void);
	double getCrossEntropy(void);
	void setEntropyInput(Mat src);
	void setCrossEntropyInput(Mat src1, Mat src2);
private:
	Mat img;
	Mat img1, img2;
};
/*图像预处理类 ProcessImg
setInput： 用于传入需要处理的图像
filterImg：采用中值滤波对图像进行滤波
thredImg： 根据预设值的阈值threshValue对图像进行二值化
getOutput：获得处理后的图像
*/
class ProcessImg
{
public:
	void setInput(const Mat img);
	void filterImg(const int ksize);
	void thredImg(const double threshValue);
	void getOutput(Mat& OutPutImg);
private:
	Mat src;
	Mat dst;

};
#endif
