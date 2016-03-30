#ifndef TLRAND_H
#define TLRAND_H

#include <cstdlib>

namespace tl {
	int _randM() {
		return std::rand() | (std::rand() << 15) | ((std::rand() & 1) << 30);
	}
	int rand(int x) {
		return _randM() % x;
	}
	int rand(int a, int b) {
		return a + rand(b - a + 1);
	}
}

#endif