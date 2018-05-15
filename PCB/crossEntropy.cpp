#include<iostream>
#include<opencv.hpp>

using namespace std;
using namespace cv;



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

void ComputeCrossEntropy::setEntropyInput(Mat src)
{
	img = src;
}

void ComputeCrossEntropy::setCrossEntropyInput(Mat src1, Mat src2)
{
	img1 = src1;
	img2 = src2;
}

double  ComputeCrossEntropy::getEntropy(void)
{
	double temp[256] = { 0.0 };

	// 计算每个像素的累积值
	for (int m = 0; m<img.rows; m++)
	{// 有效访问行列的方式
		const uchar* t = img.ptr<uchar>(m);
		for (int n = 0; n<img.cols; n++)
		{
			int i = t[n];
			temp[i] = temp[i] + 1;
		}
	}

	// 计算每个像素的概率
	for (int i = 0; i<256; i++)
	{
		temp[i] = temp[i] / (img.rows*img.cols);
	}

	double result = 0;
	// 计算图像信息熵
	for (int i = 0; i<256; i++)
	{
		if (temp[i] == 0.0)
			result = result;
		else
			result = result - temp[i] * (log(temp[i]) / log(2.0));
	}

	return result;

}

double  ComputeCrossEntropy::getCrossEntropy(void)
{
	double temp[256][256] = { 0.0 };

	// 计算联合图像像素的累积值
	for (int m1 = 0, m2 = 0; m1 < img1.rows, m2 < img2.rows; m1++, m2++)
	{    // 有效访问行列的方式
		const uchar* t1 = img1.ptr<uchar>(m1);
		const uchar* t2 = img2.ptr<uchar>(m2);
		for (int n1 = 0, n2 = 0; n1 < img1.cols, n2 < img2.cols; n1++, n2++)
		{
			int i = t1[n1], j = t2[n2];
			temp[i][j] = temp[i][j] + 1;
		}
	}

	// 计算每个联合像素的概率
	for (int i = 0; i < 256; i++)
	{
		for (int j = 0; j < 256; j++)

		{
			temp[i][j] = temp[i][j] / (img1.rows*img1.cols);
		}
	}

	double result = 0.0;
	//计算图像联合信息熵
	for (int i = 0; i < 256; i++)
	{
		for (int j = 0; j < 256; j++)

		{
			if (temp[i][j] == 0.0)
				result = result;
			else
				result = result - temp[i][j] * (log(temp[i][j]) / log(2.0));
		}
	}

	//得到两幅图像的互信息熵
	setEntropyInput(img1);
	double img1_entropy = getEntropy();
	setEntropyInput(img2);
	double img2_entropy = getEntropy();
	result = img1_entropy + img2_entropy - result;

	return result;

}
