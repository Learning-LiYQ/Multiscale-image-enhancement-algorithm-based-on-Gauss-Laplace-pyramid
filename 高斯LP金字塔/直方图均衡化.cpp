#include<iostream>  
#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp>  
#include <cv.h>
#include <cvaux.h>
#include <stdlib.h>
#include <imgproc.hpp>

using namespace cv;
using namespace std;

int main() {
	//Mat src = imread("16.bmp");
	//Mat dst = src.clone;
	IplImage *src;
	src = cvLoadImage("16.bmp");
	IplImage *dst = cvCreateImage(cvSize(src->width, src->height), IPL_DEPTH_8U, 1);

	int table[256] = { 0 };
	int range[] = { 0, 255 };
	std::cin >> range[0] >> range[1];
	for (int i = 0; i < src->height; i++)
	{
		for (int j = 0; j < src->width; j++)
		{
			CvScalar s = cvGet2D(src, i, j);
			table[(int)s.val[0]]++;
		}
	}
	double max = 0;
	int sum = 0;
	for (int i = 0; i < 251; i++)
	{
		sum += table[i];
		if (table[i] > max)max = table[i];
		//std::cout << table[i] << std:: endl;
	}
	Mat histgram(256, 256, CV_8U, Scalar(255));
	for (int i = range[0]; i <= range[1]; i++)
	{
		line(histgram, Point(i, 255), Point(i, (255 - table[i] * 255 / max)), Scalar(0));
	}
	namedWindow("histgram");
	imshow("histgram", histgram);

	int new_table[256] = { 0 };
	float s_table[2] = { (float)table[0] / sum };
	for (int i = 1; i < 256; i++)
	{
		s_table[i % 2] = s_table[(i - 1) % 2] + (float)table[i] / sum;
		new_table[i] = (int)(s_table[i % 2] * 255 + 0.5);
	}
	for (int i = 0; i < src->height; i++)
	{
		for (int j = 0; j < src->width; j++)
		{
			CvScalar s = cvGet2D(src, i, j);
			s.val[0] = new_table[(int)s.val[0]];
			cvSet2D(dst, i, j, s);
		}
	}

	cvNamedWindow("Image", WINDOW_NORMAL);//创建窗口
	cvShowImage("Image", dst);//显示图像
	imwrite("直方图.bmp", histgram);
	Mat Img;
	Img = cvarrToMat(dst);
	imwrite("直方图处理后.bmp", Img);
	waitKey();
	return 0;
}
	