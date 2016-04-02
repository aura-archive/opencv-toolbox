/****************************************************
* Toolbox for ytl's opencv project use
* Tianlong Yang, yangtianlong111@gmail.com
* Computer Science & Technology, Tsinghua University
* Copyright (c) 2016.3.29 All rights reserved.
*****************************************************/


/**************************************
由于opencv库中存在一些问题和还不太完善的功能，使用此ToolBox进行补充
ytl的Toolbox，主要由{
tlToolbox
tlDebug
tlGeometry
}
组成

tlDebug为类Qt的调试类，可以使用tlDebug()<<xxx来调用，详见tlDebug.hpp
tlArray为矩阵类，用来生成多维矩阵和矩阵相关操作
//tlMath为opencv相关计算函数
tlGeometry为几何相关函数,包括坐点类tltlPoint和多边形类tlPoly
//tlGrating为光栅几何相关函数
//tlTranslate为opencv图形转换函数


使用`using namespace tl;`来使用ToolBox里的函数

使用`#define USE_ALIAS`来使用别名:
Debug <= tlDebug
tlPoint <= tltlPoint

************************************/



/********************************************
OpenCV中的注意事项
1.Scalar的 / 重载是有问题的, 要用自己重写的div(),
2.Mat的 * / 为矩阵操作,要用Mat::mul(),Mat::div()
********************************************/


/**************************************
PS.OpenCV常见函数用法
Mat a(300,300,CV_8UC3);
imread(a,"")
**************************************/


#ifndef TOOLBOX_H
#define TOOLBOX_H


//允许使用别名
#define USE_ALIAS
//使用CV函数
#define USE_OPENCV

#ifdef USE_ALIAS
#define USE_TLDEBUG
#define USE_TLPOINT
#define USE_TLPOLY
#define USE_TLLINE
#define USE_TLARRAY
#endif

#include "tlArray.hpp"
#include "tlDebug.hpp"
#include "tlGeometry.hpp"
#include "tlRand.hpp"
#include <opencv2/opencv.hpp>
#include <cmath>
#include <cstdarg>
using namespace std;
using namespace cv;


namespace tl {
	//图像某一点的像素值(比Mat::at()快)  eg. dataAt<Vec3b>(img,3,5)
	template<typename T>
	T& dataAt(cv::Mat & src, int i, int j) {
		return src.at<T>(i, j);
		T* curRow = src.ptr<T>(i);
		return *(curRow + j * src.channels());
	}

	template<typename T>
	T& dataAt(cv::Mat & src, tlPoint p) {
		return src.at<T>((int)p.x, (int)p.y);
		T* curRow = src.ptr<T>((int)p.x);
		return *(curRow + (int)p.y * src.channels());
	}

	template <class T>
	inline T Sqr(T a) {
		return a*a;
	}

	//将Scalar中的每个元素平方
	inline Scalar Sqr(const Scalar &a) {
		return Scalar(Sqr(a[0]), Sqr(a[1]), Sqr(a[2]), Sqr(a[3]));
	}

	//将Scalar中的每个元素开根
	inline Scalar Sqrt(const Scalar &a)
	{
		return Scalar(std::sqrt(a[0]), std::sqrt(a[1]), std::sqrt(a[2]), std::sqrt(a[3]));
	}

	//a的每一个通道分别乘以b对应的值
	Mat Mul(const Mat &a, const Scalar &b) {
		Mat c = a.clone();
		vector<Mat> clist;
		split(c, clist);
		for (int i = 0; i < 3; i++)
		{
			clist[i] = clist[i] * b[i];
		}
		merge(clist, c);
		return c;
	}


	//将两个Scalar的元素对应相乘
	inline Scalar MulEach(const Scalar &a, const Scalar &b) {
		return Scalar(a[0] * b[0], a[1] * b[1], a[2] * b[2], a[3] * b[3]);
	}

	//将两个Scalar的元素对应相除
	inline Scalar DivEach(const Scalar &a, const Scalar &b) {
		return Scalar(a[0] / b[0], a[1] / b[1], a[2] / b[2], a[3] / b[3]);
	}

	//将a的每个元素除以x
	inline Scalar DivEach(const Scalar &a, float x) {
		return Scalar(a[0] / x, a[1] / x, a[2] / x, a[3] / x);
	}


	inline bool operator < (const Vec3b &a, const Vec3b &b) {
		return a[0] < b[0] && a[1] < b[1] && a[2] < b[2];
	}

	inline bool operator > (const Vec3b &a, const Vec3b &b) {
		return a[0] > b[0] && a[1] > b[1] && a[2] > b[2];
	}

	inline Vec3b operator - (const Vec3b &a, const Vec3b &b) {
		return Vec3b(a[0] - b[0], a[1] - b[1], a[2] - b[2]);
	}

	inline Vec3b operator + (const Vec3b &a, const Vec3b &b) {
		return Vec3b(a[0] + b[0], a[1] + b[1], a[2] + b[2]);
	}
	inline Vec3b operator / (const Vec3b &a, const Vec3b &b) {
		return Vec3b(a[0] / b[0], a[1] / b[1], a[2] / b[2]);
	}

	inline Vec3b operator / (const Vec3b &a, double x) {
		return Vec3b(uchar(a[0] / x), uchar(a[1] / x), uchar(a[2] / x));
	}

	inline Vec3b operator * (const Vec3b &a, double x) {
		return Vec3b(uchar(a[0] * x), uchar(a[1] * x), uchar(a[2] * x));
	}

	inline Vec3b Abs(const Vec3b &a) {
		return Vec3b(std::abs(a[0]), std::abs(a[1]), std::abs(a[2]));
	}

	template <class T>
	T tlGetMean(int tot, ...)
	{
		T sum = 0, tmp; int count = 0;

		va_list pt;
		va_start(pt, tot);
		for (int i = 0; i < tot; i++) {
			tmp = va_arg(pt, T);
			sum += tmp;
			count++;
		}
		va_end(pt);
		return sum / count;
	}

	template <class T>
	T Mean(const T &a, const T &b) {
		return tlGetMean<T>(2, a, b);
	}

	template <class T>
	T Mean(const T &a, const T &b, const T &c) {
		return tlGetMean<T>(3, a, b, c);
	}

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

	//在a上用颜色color画直线
	template <class T>
	void tlDrawLine(Mat &a, const tlPoint &A, const tlPoint &B, T color) {
		float dx = B.x - A.x, dy = B.y - A.y, deltay, deltax;
		if (dx == 0) {
			for (int y = A.y; y <= B.y; y++) if (inMap(a, A.x, y)) {
				dataAt<T>(a, A.x, y) = color;
			}
			return;
		}
		deltay = dy / abs(dx);
		deltax = dx > 0 ? 1 : -1;
		float y = A.y;
		for (int x = A.x; x != B.x; x += deltax) if (inMap(a, x, round(y))) {
			dataAt<T>(a, x, round(y)) = color;
			y += deltay;
		}
		if (inMap(a, B.x, round(y))) {
			dataAt<T>(a, B.x, round(y)) = color;
		}
	}



	//点p位于[startX,startY]-(startX+lenX , startX+lenY)中,左上闭,右下开
	int inMap(tlPoint p, int lenX, int lenY, int startX = 0, int startY = 0) {
		return startX <= p.x && p.x < startX + lenX
			&& startY <= p.y && p.y < startY + lenY;
	}
	//点在Mat范围的矩形中
	int inMap(Mat &a, tlPoint p) {
		return 0 <= p.x && p.x < a.rows
			&& 0 <= p.y && p.y < a.cols;
	}
	//点在[0,0]-(x,y)中
	int inMap(Mat &a, int x, int y) {
		return 0 <= x && x < a.rows
			&& 0 <= y && y < a.cols;
	}


	void _floodFill(Mat &mask, tlPoint seed) {
		tlPoint *Q = new tlPoint[mask.rows*mask.cols + 5];
		int p2 = 0;
		Q[++p2] = seed;
		dataAt<uchar>(mask, seed) = 1;
		imshow("mask", mask*255);
		return;
		for (int p1 = 1; p1 <= p2; p1++) {
			tlPoint pnow = Q[p1];
			Debug() << p1 << " " << p2<<" "<<pnow;
			for (int dir = 0; dir < 4; dir++) {
				tlPoint pnew = moveTo(pnow, dir);
				if (dataAt<uchar>(mask, pnew) == 0 && inMap(mask,pnew)) {
					dataAt<uchar>(mask, pnew) == 1;
					Q[++p2] = pnew;
				}
			}
		}
	}

	//在a上画color颜色的多边形p
	template <class T>
	void tlDrawPoly(Mat &a, tlPoly &poly, T color) {
		/*Mat mask = Mat::zeros(a.rows, a.cols, CV_8U);
		poly.a[poly.n] = poly.a[0];
		for (int i = 0; i < poly.n; i++) {
			tlDrawLine<uchar>(mask, poly.a[i], poly.a[i + 1], 1);
		}
		tlPoint p;
		do {
			p.x = rand(a.rows);
			p.y = rand(a.cols);

		} while (!(poly.checkPointIn(p) && dataAt<uchar>(a, p.x, p.y) == 0));
		_floodFill(mask, p);
		for (int i = 0; i < a.rows; i++) {
			for (int j = 0; j < a.cols; j++) {
				if (dataAt<uchar>(mask, i, j) == 1) {
					dataAt<T>(a, i, j) = color;
				}
			}
		}*/
		for (int i = 0; i < a.rows; i++)
			for (int j = 0; j < a.cols; j++) {
				if (poly.checkPointIn(tlPoint(i, j))) {
					dataAt<T>(a, i, j) = color;
				}
			}
	}



}

#endif
