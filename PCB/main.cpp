#include<iostream>
#include<opencv.hpp>

using namespace std;
using namespace cv;

/*加载图像
src 加载的图像，图像为灰度图
WinName 待加载图像路径*/
int Load_Img(Mat& src,const char* WinName)
{
	 src = imread(WinName);
	if (!src.data)
	{
		cerr << "请检查图像是否被正确加载！" << endl;
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
	
	cvNamedWindow(WinName, 1);
	imshow(WinName, src);
	waitKey(0);
	/*destroyWindow(WinName);*/

	return 1;
}

void ProceeImage(const Mat& src, Mat& dst)
{
	Mat med, threshold;
	int ksize = 3;
	medianBlur(src, med, ksize);
	/*GaussianBlur(src,Gauss,)*/
	
	/*threshold(med, threshold,0,126,THRESH_BINARY);*/
	int max_value = 255;
	int blockSize = 5;
	int constValue = 5;
	adaptiveThreshold(med, threshold, max_value, 
		CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY_INV, blockSize, constValue);
	dst = threshold;
}
int main(int argc, char** argv)
{
	/* 加载PCB板图像
	Standard_image 标准PCB板
	Dected_image 待检测PCB板
	*/

	Mat Standard_image,Dected_image;
	Mat Processed_standard,dst;

	Load_Img(Standard_image, argv[1]);
	ProceeImage(Standard_image, Processed_standard);
	imshow("Processed_standard", Processed_standard);
	waitKey(0);
	Mat element = getStructuringElement(MORPH_RECT, Size(1, 3)); //getStructuringElement函数返回的是指定形状和尺寸的结构元素
	Mat Erode,Dilate;

	dilate(Processed_standard, Dilate, element);//膨胀操作
	imshow("膨胀效果图", Dilate);
	waitKey(0);
	erode(Dilate, Erode, element); //腐蚀操作
	dilate(Erode, Dilate, element);//膨胀操作
	erode(Dilate, Erode, element); //腐蚀操作
	dilate(Erode, Dilate, element);//膨胀操作
	Mat element2 = getStructuringElement(MORPH_RECT, Size(5, 5)); //getStructuringElement函数返回的是指定形状和尺寸的结构元素
	erode(Dilate, Erode, element); //腐蚀操作
	//erode(Erode, Erode, element); //腐蚀操作
	imshow("腐蚀效果图", Erode);
	waitKey(0);


	

}
