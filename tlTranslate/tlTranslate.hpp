#include <opencv2\opencv.hpp>
using namespace cv;

namespace tl {

	//Mat::convertTo(*,*,alpha,beta)为统一乘alpha加beta再转换

	//彩图double类型,maxVal(255->1)
	void tlImageToDouble(const Mat &input, Mat &output)
	{
		output = input.clone();
		output.convertTo(output, CV_32FC3, 1.0 / 255.0);
		return;
		output = input.clone();
		output.convertTo(output, CV_32FC3);
		double mymax;
		minMaxIdx(output, 0, &mymax);
		output = output / mymax;
	}

	//黑白图像转double类型,maxVal(255->1)
	void tlArrayToDouble(const Mat &input, Mat &output)
	{
		input.convertTo(output, CV_32F, 1.0 / 255.0);
		return;
		output = input.clone();
		output.convertTo(output, CV_32F);
		double mymax;
		minMaxIdx(output, 0, &mymax);
		output = output / mymax;
	}

	//double类型转黑白图,maxVal(1->255)
	void tlDoubleToImage(const Mat &input, Mat &output)
	{
		input.convertTo(output, CV_8UC3, 255.0);
		return;
		output = input.clone();
		double mymax;
		minMaxIdx(output, 0, &mymax);
		output = output / mymax * 255;
		output.convertTo(output, CV_8UC3);
	}

	//double类型转彩图,maxVal(1->255)
	void tlDoubleToArray(const Mat &input, Mat &output)
	{
		input.convertTo(output, CV_8U, 255.0);
		return;
		output = input.clone();
		double mymax;
		minMaxIdx(output, 0, &mymax);
		output = output / mymax * 255;
		output.convertTo(output, CV_8U);
	}

	//数组归一化
	void tlDoubleNomolization(Mat &a)
	{
		double mymax;
		minMaxIdx(a, 0, &mymax);
		a = a / mymax;
	}


}