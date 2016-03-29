/*******************************************************
* tlPoint for 2d position calculate
* Tianlong Yang, yangtianlong111@gmail.com
* Computer Science & Technology, Tsinghua University
* Copyright (c) 2015.11.29 All rights reserved.
********************************************************/

#ifndef TLMATH_H
#define TLMATH_H

#include <cmath>
#include <opencv2\opencv.hpp>
using namespace cv;

namespace tl {

	int _randM() {
		return std::rand() | (std::rand() << 15) | ((std::rand() & 1) << 30);
	}

	//返回[0~x-1]范围内的随机数
	int Rand(int x) {
		return _randM() % x;
	}

	//返回[a~b]范围内的随机数
	int Rand(int a, int b) {
		return a + Rand(b - a + 1);
	}

#ifndef tlSqr
#define tlSqr
	template<class T>
	inline T Sqr(T a) {
		return a*a;
	}
#endif

	
	//将Scalar中的每个元素平方
	inline Scalar Sqr(const Scalar &a) {
		return Scalar(sqr(a[0]), sqr(a[1]), sqr(a[2]), sqr(a[3]));
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

	//废弃--usage:tlGetMeanM<float>(4,2.0,3.0,3.0,3.0);  --> 2.75
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


	Vec3b Mean(const Vec3b &a, const Vec3b &b) {
		return Vec3b((a.val[0] + b.val[0]) / 2, (a.val[1] + b.val[1]) / 2, (a.val[2] + b.val[2]) / 2);
	}
	Scalar Mean(const Scalar &a, const Scalar &b) {
		return Scalar((a.val[0] + b.val[0]) / 2, (a.val[1] + b.val[1]) / 2, (a.val[2] + b.val[2]) / 2);
	}




}


#endif