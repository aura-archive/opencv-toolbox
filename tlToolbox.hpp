/****************************************************
* Toolbox for ytl's opencv project use
* Tianlong Yang, yangtianlong111@gmail.com
* Computer Science & Technology, Tsinghua University
* Copyright (c) 2016.3.29 All rights reserved.
*****************************************************/


/**************************************
由于opencv库中存在一些问题和还不太完善的功能，使用此ToolBox进行补充
ytl的Toolbox，由{
一般类模块
tlDebug
tlGeometry
tlArray
opencv函数模块
tlMath
tlTranslate
tlGrating
基础函数
basic
}
组成

tlDebug为类Qt的调试类，可以使用tlDebug()<<xxx来调用，详见tlDebug.hpp
tlArray为矩阵类，用来生成多维矩阵和矩阵相关操作
tlMath为opencv相关计算函数
tlGeometry为几何相关函数,包括坐点类tlPoint和多边形类tlPoly
tlGrating为光栅几何相关函数
tlTranslate为opencv图形转换函数


使用`using namespace tl;`来使用ToolBox里的函数

使用`#define USE_ALIAS`来使用别名:
Debug <= tlDebug
Point <= tlPoint

************************************/

/********************************************
OpenCV中的注意事项
1.Scalar的 / 重载是有问题的, 要用自己重写的div(),
2.Mat的 * / 为矩阵操作,要用Mat::mul(),Mat::div()
********************************************/



#ifndef TOOLBOX_H
#define TOOLBOX_H

//打开debug开关
#define USE_DEBUG
//允许使用别名
#define USE_ALIAS
//使用CV函数
#define USE_CVFUNCTION


#include <opencv2/opencv.hpp>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <iostream>
#include <cmath>
#include <cstdarg>
using namespace std;
using namespace cv;


namespace tl {

	//---------------------------------------------------Basic----------------------------------------------------------------
		//图像某一点的像素值(比Mat::at()快)  eg. dataAt<Vec3b>(img,3,5)
	template<typename T>
	T& dataAt(cv::Mat & src, int i, int j) {
		return src.at<T>(i, j);
		T* curRow = src.ptr<T>(i);
		return *(curRow + j * src.channels());
	}


	template<class T>
	inline T sqr(T a) {
		return a*a;
	}

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

	template <class T>
	T Mean(const T &a, const T &b) {
		return tlGetMean<T>(2, a, b);
	}

	template <class T>
	T Mean(const T &a, const T &b, const T &c) {
		return tlGetMean<T>(3, a, b, c);
	}


	//------------------------------------------Translate-------------------------------------------------------------

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

	//----------------------------------------------End Translate -------------------------------------------------------


	//-------------------------------------------------- Debug ---------------------------------------------------------------
		/************************************
		tlDebug类，使用tlDebug()<<xxx; 调用，默认会在每次输出后自动加回车，若要关闭使用Debug()<<xxx<<NoEnter;

		usage:
		tlDebug().setDebugOutputWay(Off);
		tlDebug().setDebugOutputWay(FileOut);
		tlDebug().setDebugOutputWay(StdOut);
		tlDebug() << "This" << 1 << 's' << "good";
		tlDebug().pause();

		//opencv use
		Mat a(10, 10, CV_8U);
		tlDebug()<<Scalar(1,2,3)<<Vec3b(1,2,3)<<uchar('2');
		tlDebug().printMat<uchar>(a);


		************************************/



		//输出方式 Off:关闭Debug ， Stdout:屏幕输出 ， FileOut:文件输出(默认tlDebug.txt)
	enum tlDebugOutputWay { Off, StdOut, FileOut };
	enum tlDebugCommand { NoEnter, IntType };

	ofstream fout("tlDebug.txt");

	class tlDebug {
	private:
		ostringstream sout;
		//ofstream fout;
		int noEnterStatus;
		int allIntStatus;
		int useNoEnter;
		int useIntType;
		int floatDigit;//小数输出位数(没用)
		string debugOutputFileName;//输出文件名(没用)
		static tlDebugOutputWay debugOutputWay;
	public:
		//----------- default settings-----------------
		tlDebug() {
			debugOutputFileName = "tlDebug.txt";
			useNoEnter = 0;
			useIntType = 0;
			floatDigit = 2;
		}
		~tlDebug() {
			if (debugOutputWay == Off) return;
			if (!useNoEnter) sout << endl;

			if (debugOutputWay == StdOut) {
				cout << sout.str();
			}
			else if (debugOutputWay == FileOut) {
				if (!fout.is_open()) {
					cout << "[ERROR] tlDebug file cannot open!";
				}
				fout << sout.str();
			}
		}
		static void pause() {
			if (debugOutputWay == StdOut) {
				system("pause");
			}
		}
		void changetlDebugFileName(string fileName) {
			debugOutputFileName = fileName;
		}
		void setDebugOutputWay(tlDebugOutputWay outputWay) {
			debugOutputWay = outputWay;
		}
		tlDebugOutputWay getDebugOutputWay() {
			return debugOutputWay;
		}


		//--------output command-------------------
		/*输出命令参数
		usage:
		Debug()<<"("<<NoEnter;
		for (int i=1;i<=3;i++) Debug()<<i<<NoEnter;
		Debug()<<")"<<endl;*/
		tlDebug& operator << (tlDebugCommand command) {
			if (debugOutputWay == Off) return *this;
			if (command == NoEnter) {
				useNoEnter = 1;
			}
			if (command == IntType) {
				useIntType = 1;
			}
			return *this;
		}


		//--------- rederict output stream --------------
		/*小数输出
		usage:
		char a='A';
		Debug()<<a;            --- 'A'
		Debug()<<a<<IntType;   --- 96*/
		tlDebug& tlDebug::operator<< (float a) {
			if (debugOutputWay == Off) return *this;
			sout << setprecision(2) << fixed << (useIntType ? int(a) : a);
			return *this;
		}

		template<typename T>
		tlDebug& operator << (T a) {
			if (debugOutputWay == Off) return *this;
			sout << a;
			return *this;
		}

#ifdef USE_CVFUNCTION
		tlDebug& operator << (Vec3b a) {
			if (debugOutputWay == Off) return *this;
			sout << "(" << int(a[0]) << "," << int(a[1]) << "," << int(a[2]) << ")";
			return *this;
		}
		tlDebug& operator<< (uchar a) {
			if (debugOutputWay == Off) return *this;
			sout << (useIntType ? int(a) : uchar(a));
			return *this;
		}

		//打印Mat以(startX,startY)为起点,长宽为(lenX,lenY)范围的像素值
		template<typename T>
		void printMat(Mat &a, int lenX = 10, int lenY = 10, int startX = 0, int startY = 0)
		{
			tlDebug() << "Mat(" << a.rows << "," << a.cols << "):";
			tlDebug() << "from [" << startX << "," << startY << "] to [" << startX + lenX - 1 << "," << startY + lenY - 1 << "]";
			for (int i = startX; i < startX + lenX; i++) {
				tlDebug() << "(" << NoEnter;
				for (int j = startY; j < startY + lenY; j++) {
					tlDebug() << int(dataAt<T>(a, i, j)) << NoEnter;
					if (j != startY + lenY - 1) tlDebug() << ", " << NoEnter;
					//cout<<a.at<T>(i, j) << " ";
				}
				tlDebug() << ")";
			}
			tlDebug() << "";
		}

		//在控制台打印Mat以(startX,startY)为起点,长宽为(lenX,lenY)范围的像素值,保留1位小数
		template<typename T>
		void printMatToStd(Mat &a, int lenX = 10, int lenY = 10, int startX = 0, int startY = 0, int digit = 1)
		{
			tlDebugStatus temp = tlDebug().getStatus();
			tlDebug().setStatus(StdOut);
			tlDebug() << "Mat(" << a.rows << "," << a.cols << "):";
			tlDebug() << "from [" << startX << "," << startY << "] to [" << startX + lenX - 1 << "," << startY + lenY - 1 << "]";
			for (int i = startX; i < startX + lenX; i++) {
				tlDebug() << "(" << NoEnter;
				for (int j = startY; j < startY + lenY; j++) {
					tlDebug() << float(dataAt<T>(a, i, j)) << NoEnter;
					if (j != startY + lenY - 1) tlDebug() << ", " << NoEnter;
				}
				tlDebug() << ")";
			}
			tlDebug() << "";
			tlDebug().setStatus(temp);
		}


		//在文件打印Mat以(startX,startY)为起点,长宽为(lenX,lenY)范围的像素值,默认打印整个Mat
		template<typename T>
		void printMatToFile(Mat &a, int lenX = 0, int lenY = 0, int startX = 0, int startY = 0, string fileName = "tlDebug.txt")
		{
			tlDebugStatus temp = tlDebug().getStatus();
			tlDebug().setStatus(FileOut);
			if (lenX == 0 && lenY == 0) {
				lenX = 50;// a.rows;
				lenY = 50;// a.cols;
			}
			tlDebug() << "Mat(" << a.rows << "," << a.cols << "):";
			tlDebug() << "from [" << startX << "," << startY << "] to [" << startX + lenX - 1 << "," << startY + lenY - 1 << "]";
			for (int i = startX; i < startX + lenX; i++) {
				tlDebug() << "(" << NoEnter;
				for (int j = startY; j < startY + lenY; j++) {
					tlDebug() << int(dataAt<T>(a, i, j)) << NoEnter;
					if (j != startY + lenY - 1) tlDebug() << ", " << NoEnter;
					//cout<<a.at<T>(i, j) << " ";
				}
				tlDebug() << ")";
			}
			tlDebug() << "";
			tlDebug().setStatus(temp);
		}

		//在文件打印Mat以(startX,startY)为起点,长宽为(lenX,lenY)范围的像素值,默认打印整个Mat,保留一位小数
		template<typename T>
		void printMatToFileFloat(Mat &a, int lenX = 0, int lenY = 0, int startX = 0, int startY = 0, string fileName = "tlDebug.txt", int digit = 1)
		{
			tlDebugStatus temp = tlDebug().getStatus();
			tlDebug().setStatus(FileOut);
			if (lenX == 0 && lenY == 0) {
				lenX = 50;// a.rows;
				lenY = 50;// a.cols;
			}
			tlDebug() << "Mat(" << a.rows << "," << a.cols << "):";
			tlDebug() << "from [" << startX << "," << startY << "] to [" << startX + lenX - 1 << "," << startY + lenY - 1 << "]";
			for (int i = startX; i < startX + lenX; i++) {
				tlDebug() << "(" << NoEnter;
				for (int j = startY; j < startY + lenY; j++) {
					tlDebug() << float(dataAt<T>(a, i, j)) << NoEnter;
					if (j != startY + lenY - 1) tlDebug() << ", " << NoEnter;
				}
				tlDebug() << ")";
			}
			tlDebug() << "";
			tlDebug().setStatus(temp);
		}


#endif

	};


	//是否允许使用Debug别名
#ifdef USE_ALIAS
	typedef tlDebug Debug;
#endif


#ifdef USE_DEBUG
	//默认DebugWay为StdOut
	tlDebugOutputWay tlDebug::debugOutputWay = StdOut;
#else
	tlDebugOutputWay tlDebug::debugOutputWay = Off;
#endif


//---------------------------------------END　DEBUG ----------------------------------------------------------------------





//-------------------------------------------Geometry --------------------------------------------------------------------



/*******************************************
usage:
tlPoint pt(1,1);
(tlPoint(2, 3) + tlPoint(4, 5)).print();
*******************************************/


		template <class T>
		class tlPoint2D {
		private:

		public:
			T x, y;
			tlPoint2D(T x = 0, T y = 0) :x(x), y(y) {};

			void print() const {
				cout << x << " " << y << endl;
			}
			T dis(tlPoint2D& a) {
				return sqr(x - a.x) + sqr(y - a.y);
			}
			float len() {
				return sqrt(sqr(x) + sqr(y));
			}
			tlPoint2D operator + (const tlPoint2D &A) {
				return tlPoint2D(x + A.x, y + A.y);
			}
			tlPoint2D operator - (const tlPoint2D &A) {
				return tlPoint2D(x - A.x, y - A.y);
			}
			tlPoint2D operator * (float k) {
				return tlPoint2D(x*k, y*k);
			}
			tlPoint2D operator / (float k) {
				return tlPoint2D(x / k, y / k);
			}
		};

		typedef tlPoint2D<int> tlPoint;
		typedef tlPoint2D<int> tlPointd;
		typedef tlPoint2D<float> tlPointf;

		//允许使用别名 Point
#ifdef USE_ALIAS
		typedef tlPoint2D<int> Point;
#endif


	class tlPoly {
	private:
	public:
		int n;
		tlPoint *p;
		tlPoly(int n) :n(n) {
			p = new tlPoint[n + 5];
		}
		tlPoly(int n, tlPoint *pp) :n(n) {
			p = new tlPoint[n + 5];
			for (int i = 0; i < n; i++) p[i] = pp[i];
		}
		void setPots(tlPoint *pp) {
			for (int i = 0; i < n; i++) p[i] = pp[i];
		}

		//将Poly所在的点裁剪到[a,b]-[c,d]范围内,返回mask矩阵
		void cut(int a, int b, int c, int d) {
			/*Mat mask = Mat::zeros(c, d, CV_8U);
			p[n] = p[0];
			for (int i = 0; i < n; i++) tlDrawLine(mask, p[i], p[i + 1]);
			for ()
				for (int i = a; i <= b; i++)
					for (int j = c; j <= d; j) {

					}*/
		}

	};


	//允许使用别名 Point
#ifdef USE_ALIAS
	typedef tlPoly Poly;
#endif



//-------------------------------------------End Geometry ----------------------------------------------------------------




//------------------------------------------Array ------------------------------------------------------------------------

	const int xo[] = { 1, -1, 0, 0 };
	const int yo[] = { 0, 0, 1, -1 };
	class tlArray {

		//新建二维数组并初始化
		template<typename T>
		T** newArray(int n, int m) {
			T** a = new T*[n];
			for (int i = 0; i < n; i++) a[i] = new T[m]();
			return a;
		}

		//删除二维数组
		template<typename T>
		void deleteArray(T** a, int n, int m) {
			for (int i = n - 1; i >= 0; i--) delete[] a[i];
			delete[] a;
		}


	};
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


//------------------------------------------End Array---------------------------------------------------------------------


//------------------------------------------------ Grating ---------------------------------------------------------------
	template <class T>
	void tlDrawLine(Mat &a, const tlPoint &A, const tlPoint &B, T color) {
		float dx = B.x - A.x, dy = B.y - A.y, delta;
		if (dx == 0) {
			for (int y = A.y; y <= B.y; y++) if (inMap(a, A.x, y)) {
				dataAt<T>(a, A.x, y) = color;
			}
		}
		delta = dy / dx;
		float y = A.y;

		Debug() << dx << " " << dy << " " << delta;
		cout << dx << " " << dy << " " << delta;
		for (int x = A.x; x <= B.x; x++) if (inMap(a, x, round(y))) {
			dataAt<T>(a, x, round(y)) = color;
			y += delta;
		}
	}

//-------------------------------------------------End Grating  -----------------------------------------------------

}



#endif