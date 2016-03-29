#ifndef TLARRAY_H
#define TLARRAY_H


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

		//��pλ��[startX,startY] --- (startX+lenX , startX+lenY)��,���ϱ�,���¿�
		//int inMap(myPoint p, int lenX, int lenY, int startX = 0, int startY = 0);
		//����Mat��Χ�ľ�����
		//int inMap(Mat &a, myPoint p);
		//int inMap(Mat &a, int x, int y);

	};

}



#endif