#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

using namespace cv;

int main()
{
	Mat src, dst,src1;
	src = imread("3.BMP",0); //加载图片
	src1 = src(Rect(0, 0, src.cols - 1, src.rows - 1)); //调整图片尺寸使其像素值为偶数

	namedWindow("src", WINDOW_NORMAL);
	imshow("src", src);

	Ptr<cv::CLAHE> clahe = createCLAHE(); //对图片进行CLAHE处理
	clahe->setClipLimit(4);
	clahe->setTilesGridSize(Size(8, 8));
	clahe->apply(src, dst);

	namedWindow("dst", WINDOW_NORMAL); //显示处理结果
	imshow("dst", dst);
	imwrite("CLAHE处理-3.bmp", dst);

	waitKey(0);
	return 0;

}