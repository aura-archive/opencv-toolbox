#ifndef TLARRAY_H
#define TLARRAY_H

#include "../tlGeometry/tlGeometry.hpp"

namespace tl {
	const int xo[] = { 1, -1, 0, 0 };
	const int yo[] = { 0, 0, 1, -1 };
	class tlArray {
	
		//�½���ά���鲢��ʼ��
		template<typename T>
		T** newArray(int n, int m) {
			T** a = new T*[n];
			for (int i = 0; i < n; i++) a[i] = new T[m]();
			return a;
		}

		//ɾ����ά����
		template<typename T>
		void deleteArray(T** a, int n, int m) {
			for (int i = n - 1; i >= 0; i--) delete[] a[i];
			delete[] a;
		}

		//��pλ��[startX,startY]-(startX+lenX , startX+lenY)��,���ϱ�,���¿�
		int inMap(tlPoint p, int lenX, int lenY, int startX = 0, int startY = 0) {
			return startX <= p.x && p.x < startX + lenX
				&& startY <= p.y && p.y < startY + lenY;
		}
		//����Mat��Χ�ľ�����
		int inMap(Mat &a, tlPoint p) {
			return 0 <= p.x && p.x < a.rows
				&& 0 <= p.y && p.y < a.cols;
		}
		//����[0,0]-(x,y)��
		int inMap(Mat &a, int x, int y) {
			return 0 <= x && x < a.rows
				&& 0 <= y && y < a.cols;
		}

	};

}



#endif