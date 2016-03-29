#ifndef TLGEOMETRY_H
#define TLGEOMETRY_H

#include "../basic.hpp"
#include "../tlArray/tlArray.hpp"
#include <opencv2\opencv.hpp>
using namespace cv;

namespace tl {
	template <class T>
	void tlDrawLine(Mat &a,const tlPoint &A,const tlPoint &B,T color) {
		float dx = B.x - A.x, dy = B.y - A.y, delta;
		if (dx == 0) {
			for (int y = A.y; y <= B.y; y++) if (tlArray().inMap(a,A.x,y)){
				dataAt<T>(a, A.x, y) = color;
			}
		}
		delta = dy / dx;
		float y = A.y;
		
		Debug() << dx << " " << dy << " " << delta;
		cout << dx << " " << dy << " " << delta;
		for (int x = A.x; x <= B.x; x++) if (tlArray().inMap(a,x,round(y))){
			dataAt<T>(a, x, round(y)) = color;
			y += delta;
		}
	}
}


#endif