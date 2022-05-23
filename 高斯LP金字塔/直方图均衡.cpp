#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

using namespace cv;

int main()
{
	Mat src, dst,src1;
	src = imread("3.BMP",0); //����ͼƬ
	src1 = src(Rect(0, 0, src.cols - 1, src.rows - 1)); //����ͼƬ�ߴ�ʹ������ֵΪż��

	namedWindow("src", WINDOW_NORMAL);
	imshow("src", src);

	Ptr<cv::CLAHE> clahe = createCLAHE(); //��ͼƬ����CLAHE����
	clahe->setClipLimit(4);
	clahe->setTilesGridSize(Size(8, 8));
	clahe->apply(src, dst);

	namedWindow("dst", WINDOW_NORMAL); //��ʾ������
	imshow("dst", dst);
	imwrite("CLAHE����-3.bmp", dst);

	waitKey(0);
	return 0;

}