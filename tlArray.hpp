
#ifndef TLARRAY_H
#define TLARRAY_H

namespace tl {
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
#ifdef USE_TLARRAY
	typedef tlArray Array;
#endif
}


#endif