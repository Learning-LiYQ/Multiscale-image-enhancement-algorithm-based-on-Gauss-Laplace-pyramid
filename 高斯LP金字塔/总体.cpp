#include<opencv2\opencv.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2\core\core.hpp>
#include<iostream>
#include<cmath>
#include<opencv2\imgproc\imgproc.hpp>
#include<vector>

using namespace std;
using namespace cv;

void pyr_down(Mat &image, vector<Mat> &pyrimages, int level);
void creat_laplaian(vector<Mat> &pyrimages, Mat &image);
void exp_enh(vector<Mat> &pyrimages2, double x[]);
void CLAHE2guass(vector<Mat> &pyrimages);
void reconst(vector<Mat> &hfimages, vector<Mat> &lfimages, Mat &finalimage);

int main()
{
	Mat src = imread("16.BMP",0);
	namedWindow("src", WINDOW_NORMAL);
	imshow("src", src);
	src = src(Rect(0, 0, src.cols - 1, src.rows - 1)); //ͼƬ�ߴ�Ϊ������Ҫ����Ϊż��

	vector<Mat> guass_image;
	vector<Mat> lp_image;

	pyr_down(src, guass_image, 3); //���ɸ�˹������
	lp_image = guass_image;
	creat_laplaian(lp_image, src); //����������˹������

	double a[] = { 3,2.5,2,1.5,1,0.9,0.85,0.81,0.78 }; //��Ƶָ�������ϵ��
	exp_enh(lp_image, a); //�Ը�Ƶ���ֽ�����ǿ
	CLAHE2guass(guass_image); //�Ե�Ƶ���ֽ���ֱ��ͼ����

	Mat finalimage; //�������յ���ǿͼ��
	reconst(lp_image, guass_image, finalimage);
	namedWindow("����ͼ��", WINDOW_NORMAL);
	imshow("����ͼ��", finalimage);
	//imwrite("����35.bmp", finalimage);

	waitKey(0);
	return 0;
}

void pyr_down(Mat & image, vector<Mat>& pyrimages, int level)
{
	Mat tem = image.clone();
	Mat dst;
	for (int i = 0; i < level; i++)
	{
		pyrDown(tem, dst);
		tem = dst.clone();
		//imshow(format("guass_layer_%d", i), dst);
		pyrimages.push_back(tem);
	}
}

void creat_laplaian(vector<Mat>& pyrimages, Mat & image)
{
	for (int t = pyrimages.size() - 1; t > -1; t--) {
		Mat dst;
		if (t - 1 < 0) {
			pyrUp(pyrimages[t], dst, image.size());
			subtract(image, dst, dst);
			//dst = dst + Scalar(127, 127, 127);
			pyrimages[t] = dst;
			imshow(format("lap_layer_%d", t), dst);
		}
		else {
			pyrUp(pyrimages[t], dst, pyrimages[t - 1].size());
			subtract(pyrimages[t - 1], dst, dst);
			//dst = dst + Scalar(127, 127, 127);
			pyrimages[t] = dst;
			imshow(format("lap_layer_%d", t), dst);
		}
	
	}
}

void exp_enh(vector<Mat>& pyrimages2, double x[])
{
	int t = pyrimages2.size() - 1;
	int z = 0;
	for ( z ; z <= t; z++) {
	
		//pow(pyrimages2[z], 0.8, pyrimages2[z]);
		pyrimages2[z] = x[z] * pyrimages2[z];
		pyrimages2[z] = pyrimages2[z] + Scalar(127, 127, 127);
		imshow(format("laplaian_layer_%d", z), pyrimages2[z]);
		}
}

void CLAHE2guass(vector<Mat>& pyrimages)
{
	int t = pyrimages.size() - 1;
	int z = 0;
	for (z; z <= t; z++) {
		Mat tem = pyrimages[z].clone();
		Ptr<cv::CLAHE> clahe = createCLAHE();
		clahe->setClipLimit(4);
		clahe->apply(tem, pyrimages[z]);
		//��һ����0~255    
		//normalize(pyrimages[z], pyrimages[z], 0, 255, CV_MINMAX);
		//ת����8bitͼ����ʾ    
		//convertScaleAbs(pyrimages[z], pyrimages[z]);
		imshow(format("CLAHE_layer_%d", z), pyrimages[z]);
	}
}

void reconst(vector<Mat>& hfimages, vector<Mat>& lfimages, Mat &finalimage) //hfimagesΪ��Ƶͼ��lfimageΪ��Ƶͼ��
{
	int t = lfimages.size() - 1;
	Mat tem;
	Mat dst;
	pyrUp(lfimages[t], tem,hfimages[t].size());
	//addWeighted(tem,1,hfimages[t],1,0.0,dst);
	dst = tem + hfimages[t];
	//imshow(format("reconst_layer_%d", t), dst);
	t = t - 1;
	for (t; t > -1; t--) {
		pyrUp(dst, tem, hfimages[t].size());
		//addWeighted(tem, 1, hfimages[t], 1, 0.0, dst);
		dst = tem + hfimages[t];
		//imshow(format("reconst_layer_%d", t), dst);
	}
	finalimage = dst.clone();
}
