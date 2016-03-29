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


#endif