#include<opencv2\opencv.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2\core\core.hpp>
#include<iostream>
#include<cmath>

using namespace std;
using namespace cv;

int main()
{
	Mat lpsrc, lpdst;
	lpsrc = imread("第一次高频图.BMP");
	lpdst = lpsrc.clone();
	lpdst = lpdst + Scalar(127, 127, 127);
	imshow("原图", lpdst);

	int rownumber = lpdst.rows;
	int colnumber = lpdst.cols;

	for (int i = 0; i < rownumber; ++i)
	{
		uchar* data = lpdst.ptr<uchar>(i);
		for (int j = 0; j < colnumber; ++j)
		{
			data[j] = 1.8*pow(data[j], 0.7);
		}
	}
	
	imshow("处理后", lpdst);

	waitKey(0);
	return 0;
}