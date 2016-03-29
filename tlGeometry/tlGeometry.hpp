/*******************************************************
* tlPoint for 2d position calculate
* Tianlong Yang, yangtianlong111@gmail.com
* Computer Science & Technology, Tsinghua University
* Copyright (c) 2015.11.29 All rights reserved.
********************************************************/

/*******************************************
usage:
tlPoint pt(1,1);
(tlPoint(2, 3) + tlPoint(4, 5)).print();
*******************************************/

#ifndef TLPOINT_H
#define TLPOINT_H

#include "../tlGrating/tlGrating.hpp"
#include <cmath>

namespace tl {

#ifndef tlSqr
#define tlSqr
	template<class T>
	inline T sqr(T a) {
		return a*a;
	}
#endif

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

}


class tlPoly {
private:
public:
	int n;
	tlPoint *p;
	tlPoly(int n) :n(n){
		p = new tlPoint[n + 5];
	}
	tlPoly(int n,tlPoint *pp) :n(n){
		p = new tlPoint[n+5];
		for (int i = 0; i < n; i++) p[i] = pp[i];
	}
	void setPots(tlPoint *pp) {
		for (int i = 0; i < n; i++) p[i] = pp[i];
	}

	//将Poly所在的点裁剪到[a,b]-[c,d]范围内,返回mask矩阵
	void cut(int a,int b,int c,int d) {
		Mat mask = Mat::zeros(c, d, CV_8U);
		p[n] = p[0];
		for (int i = 0; i < n; i++) tlDrawLine(mask, p[i], p[i + 1]);
		for ()
		for (int i = a; i <=b; i++)
			for (int j = c; j <= d; j) {

			}
	}

};


//允许使用别名 Point
#ifdef USE_ALIAS
typedef tlPoly Poly;
#endif

#endif