#ifndef BASIC_H
#define BASIC_H

#include <opencv2/opencv.hpp>
using namespace cv;


//图像某一点的像素值(比Mat::at()快)  eg. dataAt<Vec3b>(img,3,5)
template<typename T>
T& dataAt(cv::Mat & src, int i, int j) {
	return src.at<T>(i, j);
	T* curRow = src.ptr<T>(i);
	return *(curRow + j * src.channels());
}


#endif