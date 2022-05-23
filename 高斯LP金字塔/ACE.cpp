#include <iostream>     
#include <opencv2/opencv.hpp>  

using namespace cv;
using namespace std;

Mat matrixWiseMulti(Mat &m1, Mat &m2) {
	Mat dst = m1.mul(m2);
	return dst;
}


//float MaxCG:�Ը�Ƶ�ɷֵ��������ֵ,int n���ֲ��뾶,int C:�Ը�Ƶ��ֱ������ϵ��  
void ACE(Mat &src, int C = 3, int n = 20, float MaxCG = 3) {
	int rows = src.rows;
	int cols = src.cols;

	Mat meanLocal; //ͼ��ֲ���ֵ  
	Mat varLocal;  //ͼ��ֲ�����  
	Mat meanGlobal;//ȫ�־�ֵ
	Mat varGlobal; //ȫ�ֱ�׼��  

	blur(src.clone(), meanLocal, Size(n, n));
	namedWindow("��ͨ�˲�", WINDOW_NORMAL);
	imshow("��ͨ�˲�", meanLocal);
	//imwrite("��ͨ�˲�.bmp", meanLocal);
	Mat highFreq = src - meanLocal;//��Ƶ�ɷ� 
	namedWindow("��Ƶ�ɷ�", WINDOW_NORMAL);
	imshow("��Ƶ�ɷ�", highFreq);
	//imwrite("��Ƶ�ɷ�.bmp", highFreq);

	varLocal = matrixWiseMulti(highFreq, highFreq);
	blur(varLocal, varLocal, Size(n, n));
	//����ɾֲ���׼��  
	varLocal.convertTo(varLocal, CV_32F);
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			varLocal.at<float>(i, j) = (float)sqrt(varLocal.at<float>(i, j));
		}
	}
	meanStdDev(src, meanGlobal, varGlobal);
	Mat gainArr = 0.5 * meanGlobal / varLocal;//����ϵ������  

	//�����������н�ֹ  
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
	//imshow("��������ǿ���Ƶ", gainArr);
	//imshow("�����淽��", dst1);
	Mat dst2 = meanLocal + C * highFreq;
	Mat chigh = C * highFreq;
	imshow("C��ǿ���Ƶ", chigh); 
	//imwrite("��ǿ���Ƶ.bmp", chigh);
	namedWindow("�����淽��", WINDOW_NORMAL);
	imshow("�����淽��", dst2);
	//imwrite("ACE������.bmp", dst2);
}

int main()
{
	const char* img_path = "3.bmp";
	Mat src = imread(img_path, 0);
	namedWindow("src", WINDOW_NORMAL);
	imshow("src", src);
	//Mat HE;
	//equalizeHist(src,HE);
	//imshow("ֱ��ͼ���⻯", HE);
	//imwrite("ֱ��ͼ���⻯.bmp", HE);
	int C = 5;
	int n = 7;
	float MaxCG = 3;
	ACE(src, C, n, MaxCG);
	waitKey();
	return  0;
}