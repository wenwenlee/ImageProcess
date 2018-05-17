#include<iostream>
#include<opencv.hpp>
#include"function.h"
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
	Mat med, thresholdImg;
	int ksize = 3;
	medianBlur(src, med, ksize);
	/*GaussianBlur(src,Gauss,)*/
	
	/*threshold(med, threshold,0,126,THRESH_BINARY);*/
	int max_value = 255;
	int blockSize = 5;
	int constValue = 5;
	double threshValue = 125;
	/*adaptiveThreshold(med, threshold, max_value, 
		CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY_INV, blockSize, constValue);*/
	threshold(med, thresholdImg, threshValue, max_value, THRESH_BINARY);

	/*threshold(med, threshold, 0, 126, THRESH_BINARY);*/
	dst = thresholdImg;
}
int main(int argc, char** argv)
{
	/* 加载PCB板图像
	Standard_image 标准PCB板
	Dected_image 待检测PCB板
	*/

	Mat Standard_image,Dected_image;
	Mat Processed_standard, Processed_Dected,dst;

	Load_Img(Standard_image, argv[1]);
	/*ProceeImage(Standard_image, Processed_standard);*/
	Load_Img(Dected_image, argv[2]);
	/*ProceeImage(Dected_image, Processed_Dected);*/
	
	int ksize = 3;
	double threshValue = 125;
	ProcessImg process;
	
	process.setInput(Standard_image);
	process.filterImg(ksize);
	process.thredImg(threshValue);
	process.getOutput(Processed_standard);

	ProcessImg process2;
	process2.setInput(Dected_image);
	process2.filterImg(ksize);
	process2.thredImg(threshValue);
	process2.getOutput(Processed_Dected);


	imshow("Processed_standard", Processed_standard);
	waitKey(0);

	imshow("Processed_Dected", Processed_Dected);
	waitKey(0);
	//Mat element = getStructuringElement(MORPH_RECT, Size(1, 3)); //getStructuringElement函数返回的是指定形状和尺寸的结构元素
	//Mat Erode,Dilate;

	//dilate(Processed_standard, Dilate, element);//膨胀操作
	//imshow("膨胀效果图", Dilate);
	//waitKey(0);
	//erode(Dilate, Erode, element); //腐蚀操作
	//dilate(Erode, Dilate, element);//膨胀操作
	//erode(Dilate, Erode, element); //腐蚀操作
	//dilate(Erode, Dilate, element);//膨胀操作
	//Mat element2 = getStructuringElement(MORPH_RECT, Size(5, 5)); //getStructuringElement函数返回的是指定形状和尺寸的结构元素
	//erode(Dilate, Erode, element); //腐蚀操作
	////erode(Erode, Erode, element); //腐蚀操作
	//imshow("腐蚀效果图", Erode);
	//waitKey(0);
	Mat FFT_dst,src2;
	Load_Img(src2, argv[2]);
	FFT(src2, FFT_dst);
	imshow("fft", FFT_dst);
	waitKey(0);

	Mat img1 = imread(argv[1]);
	Mat img2 = imread(argv[2]);
	

	//Rect rect1(80 ,370 ,150 ,130);
	Rect rect1(235, 140, 725, 540);
	Rect rect2(40, 320 ,200 ,190);
	Mat image_roi1 = img1(rect1);
	imshow("roi1",image_roi1);
	waitKey(0);

	double maxCrossEntropy = 0;
	double crossEntropy;
	int bestAngle = 0;
	Point center(src2.cols / 2, src2.rows / 2);
	Mat dst2;
	ComputeCrossEntropy t;
	
	//获取调整角度
	for (int angle = -10; angle < 10; angle++)
	{
		
		Mat rotMatS = getRotationMatrix2D(center, angle, 1.0);
		warpAffine(src2, dst2, rotMatS, src2.size(), 1, 0, Scalar(255, 255, 255));
		cout << angle << endl;
		imshow("RotatedSrc", dst2);

		waitKey(0);
		Mat image_roi2 = img1(rect2);
		t.setCrossEntropyInput(img1, dst2);
		double CrossEntropy = t.getCrossEntropy();
		cout << "CrossEntropy:" << " " << CrossEntropy << endl;
		if (maxCrossEntropy < CrossEntropy)
		{
			maxCrossEntropy = CrossEntropy;
			bestAngle = angle;
		}
		
	
	}
	Mat rotMatS = getRotationMatrix2D(center, bestAngle, 1.0);
	warpAffine(src2, dst2, rotMatS, src2.size(), 1, 0, Scalar(255, 255, 255));
	imshow("romated", dst2);
	waitKey(0);
	Mat image_roi2 = img1(rect1);
	imshow("roi2", image_roi2);
	waitKey(0);

	Mat image_roi3 = Processed_standard(rect1);
	imshow("roi3", image_roi3);
	waitKey(0);

	Mat image_roi4 = Processed_Dected(rect1);
	imshow("roi4", image_roi4);
	waitKey(0);

}
