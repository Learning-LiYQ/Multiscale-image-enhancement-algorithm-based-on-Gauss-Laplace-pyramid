#include <iostream>     
#include <opencv2/opencv.hpp>  

using namespace cv;
using namespace std;

Mat matrixWiseMulti(Mat &m1, Mat &m2) {
	Mat dst = m1.mul(m2);
	return dst;
}


//float MaxCG:对高频成分的最大增益值,int n：局部半径,int C:对高频的直接增益系数  
void ACE(Mat &src, int C = 3, int n = 20, float MaxCG = 3) {
	int rows = src.rows;
	int cols = src.cols;

	Mat meanLocal; //图像局部均值  
	Mat varLocal;  //图像局部方差  
	Mat meanGlobal;//全局均值
	Mat varGlobal; //全局标准差  

	blur(src.clone(), meanLocal, Size(n, n));
	namedWindow("低通滤波", WINDOW_NORMAL);
	imshow("低通滤波", meanLocal);
	//imwrite("低通滤波.bmp", meanLocal);
	Mat highFreq = src - meanLocal;//高频成分 
	namedWindow("高频成分", WINDOW_NORMAL);
	imshow("高频成分", highFreq);
	//imwrite("高频成分.bmp", highFreq);

	varLocal = matrixWiseMulti(highFreq, highFreq);
	blur(varLocal, varLocal, Size(n, n));
	//换算成局部标准差  
	varLocal.convertTo(varLocal, CV_32F);
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			varLocal.at<float>(i, j) = (float)sqrt(varLocal.at<float>(i, j));
		}
	}
	meanStdDev(src, meanGlobal, varGlobal);
	Mat gainArr = 0.5 * meanGlobal / varLocal;//增益系数矩阵  

	//对增益矩阵进行截止  
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			if (gainArr.at<float>(i, j) > MaxCG) {
				gainArr.at<float>(i, j) = MaxCG;
			}
		}
	}
	gainArr.convertTo(gainArr, CV_8U);
	gainArr = matrixWiseMulti(gainArr, highFreq);
	Mat dst1 = meanLocal + gainArr;
	//imshow("变增益增强后高频", gainArr);
	//imshow("变增益方法", dst1);
	Mat dst2 = meanLocal + C * highFreq;
	Mat chigh = C * highFreq;
	imshow("C增强后高频", chigh); 
	//imwrite("增强后高频.bmp", chigh);
	namedWindow("恒增益方法", WINDOW_NORMAL);
	imshow("恒增益方法", dst2);
	//imwrite("ACE恒增益.bmp", dst2);
}

int main()
{
	const char* img_path = "3.bmp";
	Mat src = imread(img_path, 0);
	namedWindow("src", WINDOW_NORMAL);
	imshow("src", src);
	//Mat HE;
	//equalizeHist(src,HE);
	//imshow("直方图均衡化", HE);
	//imwrite("直方图均衡化.bmp", HE);
	int C = 5;
	int n = 7;
	float MaxCG = 3;
	ACE(src, C, n, MaxCG);
	waitKey();
	return  0;
}