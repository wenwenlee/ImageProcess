#include<iostream>
#include<opencv.hpp>

using namespace std;
using namespace cv;

int FFT(const Mat& src, Mat& dst)
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
	
	
	/*获取傅里叶变换合适尺寸
	padded 扩展后图像
	m 行
	n 列
	*/
	Mat padded;                 
	int m = getOptimalDFTSize(src.rows);
	int n = getOptimalDFTSize(src.cols);

	/*对图像进行填充，以0填充输入图像矩阵，上方和左方不做填充处理
	src 输入图像
	padded 填充后图像
	m 行
	n 列
	*/
	copyMakeBorder(src, padded, 0, m - src.rows, 0, n - src.cols, BORDER_CONSTANT, Scalar::all(0));

	/*进行傅里叶变换的计算
	说明：
	DFT要分别计算实部和虚部，把要处理的图像作为输入的实部、一个全零的图像作为输入的虚部。
	dft()输入和输出应该分别为单张图像，所以要先用merge()把实虚部图像合并，分别处于图像comImg的两个通道内。
	计算得到的实虚部仍然保存在comImg的两个通道内。
	*/
	Mat planes[] = { Mat_<float>(padded), Mat::zeros(padded.size(),CV_32F) };
	Mat complexI;
	merge(planes, 2, complexI);     //将planes融合合并成一个多通道数组complexI
	dft(complexI, complexI);        //进行傅里叶变换


	/*获取DFT图像
	说明：
	一般都会用幅度图像来表示图像傅里叶的变换结果（傅里叶谱）。
	幅度的计算公式：magnitude = sqrt(Re(DFT)^2 + Im(DFT)^2)。
	由于幅度的变化范围很大，而一般图像亮度范围只有[0,255]，
	容易造成一大片漆黑，只有几个点很亮。所以要用log函数把数值的范围缩小。
	*/
	//计算幅值，转换到对数尺度(logarithmic scale)
	//=> log(1 + sqrt(Re(DFT(I))^2 + Im(DFT(I))^2))
	split(complexI, planes);        //planes[0] = Re(DFT(I),planes[1] = Im(DFT(I))
									//即planes[0]为实部,planes[1]为虚部
	magnitude(planes[0], planes[1], planes[0]);     //planes[0] = magnitude
	Mat magI = planes[0];

	magI += Scalar::all(1);
	log(magI, magI);                //转换到对数尺度(logarithmic scale)
    //如果有奇数行或列，则对频谱进行裁剪
	magI = magI(Rect(0, 0, magI.cols&-2, magI.rows&-2));

	/*重新排列傅里叶图像中的象限，使得原点位于图像中心
	说明：
	dft()直接获得的结果中，低频部分位于四角，高频部分位于中间。
	习惯上会把图像做四等份，互相对调，使低频部分位于图像中心，也就是让频域原点位于中心。
	*/
	int cx = magI.cols / 2;
	int cy = magI.rows / 2;

	Mat q0(magI, Rect(0, 0, cx, cy));       //左上角图像划定ROI区域
	Mat q1(magI, Rect(cx, 0, cx, cy));      //右上角图像
	Mat q2(magI, Rect(0, cy, cx, cy));      //左下角图像
	Mat q3(magI, Rect(cx, cy, cx, cy));     //右下角图像

	//变换左上角和右下角象限										
	Mat tmp;
	q0.copyTo(tmp);
	q3.copyTo(q0);
	tmp.copyTo(q3);

	//变换右上角和左下角象限
	q1.copyTo(tmp);
	q2.copyTo(q1);
	tmp.copyTo(q2);

	/*归一化处理，用0-1之间的浮点数将矩阵变换为可视的图像格式
	说明：
	虽然用log()缩小了数据范围，但仍然不能保证数值都落在[0,255]之内，
	所以要先用normalize()规范化到[0,1]内，再用convertTo()把小数映射到[0,255]内的整数。
	结果保存在一幅单通道图像内：
	*/
	normalize(magI, magI, 0, 1, CV_MINMAX);
	dst = magI;
}


