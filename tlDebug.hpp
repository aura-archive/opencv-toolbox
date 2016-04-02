/****************************************************
* tlDebug for ytl's debug use
* Tianlong Yang, yangtianlong111@gmail.com
* Computer Science & Technology, Tsinghua University
* Copyright (c) 2016.3.29 All rights reserved.
*****************************************************/


/************************************
tlDebug类，使用tlDebug()<<xxx; 调用，默认会在每次输出后自动加回车，若要关闭使用Debug()<<xxx<<NoEnter;

用法举例
tlDebug().setDebugOutputWay(Off);
tlDebug().setDebugOutputWay(FileOut);
tlDebug().setDebugOutputWay(StdOut);
tlDebug() << "This" << 1 << 's' << "good";
tlDebug().pause();

//opencv use
#define USE_OPENCV
Mat a(10, 10, CV_8U);
tlDebug()<<Scalar(1,2,3)<<Vec3b(1,2,3)<<uchar('2');
tlDebug().printMat<uchar>(a);


************************************/

#ifndef TLDEBUG_H
#define TLDEBUG_H


//#define NOT_USE_DEBUG

#ifdef USE_OPENCV
#include <opencv2\opencv.hpp>
using namespace cv;
#endif

#ifdef USE_TLPOINT
#include "tlGeometry.hpp"
#endif
#include <iomanip>
#include <sstream>
#include <fstream>
#include <iostream>
using namespace std;

namespace tl {









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

#ifdef USE_TLPOINT
		tlDebug& operator << (tlPoint a) {
			if (debugOutputWay == Off) return *this;
			sout << "(" << a.x << "," << a.y << ") ";
			return *this;
		}
#endif 


#ifdef USE_OPENCV
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
#ifdef USE_TLDEBUG
	typedef tlDebug Debug;
#endif


#ifdef NOT_USE_DEBUG
	tlDebugOutputWay tlDebug::debugOutputWay = Off;
#else
	//默认DebugWay为StdOut
	tlDebugOutputWay tlDebug::debugOutputWay = StdOut;
#endif

}



#endif