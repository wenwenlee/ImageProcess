#include<iostream>
#include<opencv.hpp>

using namespace std;
using namespace cv;


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

void ProcessImg::setInput(const Mat img)
{
	src = img;
}

void ProcessImg::filterImg(const int ksize)
{
	medianBlur(src, dst, ksize);
}

void ProcessImg::thredImg(const double threshValue)
{
	const int max_value = 255;
	threshold(dst, dst, threshValue, max_value, THRESH_BINARY);
}

void ProcessImg::getOutput(Mat& OutPutImg)
{
	OutPutImg = dst;
}
