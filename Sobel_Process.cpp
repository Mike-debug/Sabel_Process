#include<core/core.hpp>
#include<highgui/highgui.hpp>
#include<imgproc/imgproc.hpp>
#include<opencv.hpp>
#include<bits/stdc++.h>

using namespace std;
using namespace cv;

int main(void) {
	Mat img = imread("left01.jpg", 0);//灰度方式读入图像
	Mat imgX = Mat::zeros(img.size(), CV_16SC1);//数据类型为16位3通道浮点
	Mat imgY = Mat::zeros(img.size(), CV_16SC1);
	Mat imgXY = Mat::zeros(img.size(), CV_16SC1);
	Mat imgX8UC = Mat::zeros(img.size(), CV_8UC1);//保存X方向的soble处理的结果
	Mat imgY8UC = Mat::zeros(img.size(), CV_8UC1);//保存Y方向的soble处理的结果
	Mat imgXY8UC = Mat::zeros(img.size(), CV_8UC1);//合成XY方向的soble处理的结果
	if (!img.data) {
		cout << "Fail to read the image" << endl;
		return -1;
	}
	GaussianBlur(img, img, Size(3, 3), 0, 0, BORDER_DEFAULT);//先做平滑处理

	for (int i = 1; i < img.rows - 1; ++i) {
		for (int j = 1; j < img.cols - 1; ++j) {
			imgX.at<unsigned char>(i, 2 * j) = abs(
				img.at<unsigned char>(i - 1, j + 1)
				+ 2.0 * img.at<unsigned char>(i, j + 1)
				+ img.at<unsigned char>(i + 1, j + 1)
				- img.at<unsigned char>(i - 1, j - 1)
				- 2.0 * img.at<unsigned char>(i, j - 1)
				- img.at<unsigned char>(i + 1, j - 1)
			);
			imgY.at<unsigned char>(i, 2 * j) = abs(
				img.at<unsigned char>(i - 1, j - 1)
				+ 2.0 * img.at<unsigned char>(i - 1, j)
				+ img.at<unsigned char>(i - 1, j + 1)
				- img.at<unsigned char>(i + 1, j - 1)
				- 2.0 * img.at<unsigned char>(i + 1, j)
				- img.at<unsigned char>(i + 1, j + 1)
			);
		}
	}


	//uchar* p = img.data;
	//uchar* px = imgX.data;
	//uchar* py = imgY.data;
	//int step = img.step;
	//int stepxy = imgX.step;
	//
	//for (int i = 1; i < img.rows - 1; i++)
	//{
	//	for (int j = 1; j < img.cols - 1; j++)
	//	{
	//		//通过指针遍历图像上每一个像素 
	//		px[i * imgX.step + j * (stepxy / step)] = abs(p[(i - 1) * step + j + 1] + p[i * step + j + 1] * 2 + p[(i + 1) * step + j + 1] - p[(i - 1) * step + j - 1] - p[i * step + j - 1] * 2 - p[(i + 1) * step + j - 1]);
	//		py[i * imgX.step + j * (stepxy / step)] = abs(p[(i + 1) * step + j - 1] + p[(i + 1) * step + j] * 2 + p[(i + 1) * step + j + 1] - p[(i - 1) * step + j - 1] - p[(i - 1) * step + j] * 2 - p[(i - 1) * step + j + 1]);
	//	}
	//}

	addWeighted(imgX, 0.5, imgY, 0.5, 0, imgXY);

	convertScaleAbs(imgX, imgX8UC);//每个像素取绝对值
	convertScaleAbs(imgY, imgY8UC);//每个像素取绝对值
	convertScaleAbs(imgXY, imgXY8UC);//每个像素取绝对值

	Mat imgSobel;
	Sobel(img, imgSobel, CV_8UC1, 1, 1);
	imshow("Source image", img);
	imshow("X Direction", imgX8UC);
	imshow("Y Direction", imgY8UC);
	imshow("XY Direction Combination", imgXY8UC);
	imshow("OpenCV Sobel", imgSobel);
	waitKey(0);

	return 0;
}