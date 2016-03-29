#ifndef TLGEOMETRY_H
#define TLGEOMETRY_H

#include "../basic.hpp"
#include <opencv2\opencv.hpp>
using namespace cv;

namespace tl {
	template <class T>
	void tlDrawLine(Mat &a,const tlPoint &A,const tlPoint &B,T color) {
		float dx = B.x - A.x, dy = B.y - A.y,delta = dy/dx;
		float y = A.y;
		
		Debug() << dx << " " << dy << " " << delta;
		cout << dx << " " << dy << " " << delta;
		for (int x = A.x; x <= B.x; x++) {
			dataAt<T>(a, x, round(y)) = color;
			y += delta;
		}
	}
}


#endif