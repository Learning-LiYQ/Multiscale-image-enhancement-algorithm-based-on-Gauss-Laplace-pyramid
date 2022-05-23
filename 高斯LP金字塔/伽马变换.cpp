#include<opencv2\opencv.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2\core\core.hpp>
#include<iostream>
#include<cmath>
#include<opencv2\imgproc\imgproc.hpp>
#include<vector>

using namespace std;
using namespace cv;

int main() {
	Mat image = imread("3.bmp",0);
	// Convert to floating point:
	Mat X;
	image.convertTo(X, CV_32FC1);
	//image.convertTo(X, CV_32F);
	// Start preprocessing:
	Mat I;
	float gamma = 1.5;
	pow(X, gamma, I);

	//归一化到0~255    
	normalize(I,I, 0, 255, CV_MINMAX);
	//转换成8bit图像显示    
	convertScaleAbs(I, I);

	// Draw it on screen:
	imshow("Original Image", image);
	imshow("Gamma correction image", I);
	// Show the images:
	waitKey();
	// Success!
	return 0;
}