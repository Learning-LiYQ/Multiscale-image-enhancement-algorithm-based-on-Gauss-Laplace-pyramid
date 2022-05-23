#include <opencv2\opencv.hpp>
#include<opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2\highgui\highgui.hpp>
#include<iostream>

using namespace cv;
using namespace std;

int main()
{
	Mat srcimage1 = imread("21.BMP",0);
	Mat guassimage,tem,lpimage;
	Mat srcimage;
	int w1, h1, w2, h2;

	srcimage = srcimage1(Rect(0, 0, srcimage1.cols - 1, srcimage1.rows - 1));

	//imshow("【原始图】", srcimage);
	w1 = srcimage.rows;
	h1 = srcimage.cols;

	pyrDown(srcimage, guassimage);
	//imshow("【下采样1】", guassimage);

	pyrUp(guassimage, tem);
	//imshow("【下采样后再上采样】", tem);
	w2 = tem.rows;
	h2 = tem.cols;
	//cout << w1 << h1 << w2 << h2;

	lpimage = srcimage - tem;
	imshow("减法LP", lpimage);
	imwrite("第一次高频图.BMP", lpimage);

	waitKey(0);

	return 0;
}