#include"histequal.h"

#define colvl 255
Mat ycMatchHist(cv::Mat src, cv::Mat dstImg)
{
	// ****** 如果是 RGB 图片则转为灰度图片操作 ******  
	Mat out(src);
	Mat srcImg;
	if(src.channels() == 3)
		cvtColor(src, srcImg, CV_BGR2GRAY);
	else if (srcImg.channels() == 4)

		cvtColor(src, srcImg, CV_BGRA2GRAY);
	else
		srcImg = src;
	
//	int colvl = 255;
	int grayLevel[colvl];
	for (int i = 0; i<colvl; i++)   grayLevel[i] = i;

	int grayArr[colvl];
	int srcRow = srcImg.rows;
	int srcCol = srcImg.cols;
	int dstRow = dstImg.rows;
	int dstCol = dstImg.cols;
	float srcCdfArr[colvl] = { 0.f };
	float dstCdfArr[colvl] = { 0.f };
	float tmp;

	// *** 求解源图片的累积直方图（概率）分布 ***   
	memset(grayArr, 0, sizeof(grayArr));//grayArr ={0}
	for (size_t nrow = 0; nrow < srcRow; nrow++)
		for (size_t ncol = 0; ncol < srcCol; ncol++)
		{
			int tag = srcImg.at<uchar>(nrow, ncol);
			grayArr[tag]++;
		}

	tmp = 0;
	for (int i = 0; i<colvl; i++)
	{
		tmp += grayArr[i];
		srcCdfArr[i] = tmp / (srcRow * srcCol);
		// std::cout<<srcCdfArr[i]<<std::endl;  
	}

	// *** 求解目标图片的累积直方图（概率）分布 ***   
	memset(grayArr, 0, sizeof(grayArr));
	for (size_t nrow = 0; nrow < dstRow; nrow++)
		for (size_t ncol = 0; ncol < dstCol; ncol++)
		{
			int tag = dstImg.at<uchar>(nrow, ncol);
			grayArr[tag]++;
		}

	tmp = 0;
	for (int i = 0; i<colvl; i++)
	{
		tmp += grayArr[i];
		dstCdfArr[i] = tmp / (dstRow * dstCol);
	}

	// *** 直方图匹配算法 ***  
	int histMap[colvl];
	int minTag;
	for (int i = 0; i<colvl; i++)
	{
		float minMap = 10.f;
		for (int j = 0; j<colvl; j++)
		{
			if (minMap > abs(srcCdfArr[i] - dstCdfArr[j]))
			{
				minMap = abs(srcCdfArr[i] - dstCdfArr[j]);
				minTag = j;
			}
		}
		histMap[i] = minTag;
	}

	for (size_t nrow = 0; nrow < out.rows; nrow++)
		for (size_t ncol = 0; ncol < out.cols; ncol++)
		{
			int tag = out.at<uchar>(nrow, ncol);
			out.at<uchar>(nrow, ncol) = histMap[tag];
		}

	return out;
}