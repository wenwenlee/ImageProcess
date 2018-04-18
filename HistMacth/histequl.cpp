#include"histequal.h"

#define colvl 255
Mat ycMatchHist(cv::Mat src, cv::Mat dstImg)
{
	// ****** ����� RGB ͼƬ��תΪ�Ҷ�ͼƬ���� ******  
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

	// *** ���ԴͼƬ���ۻ�ֱ��ͼ�����ʣ��ֲ� ***   
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

	// *** ���Ŀ��ͼƬ���ۻ�ֱ��ͼ�����ʣ��ֲ� ***   
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

	// *** ֱ��ͼƥ���㷨 ***  
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