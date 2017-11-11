#pragma once
#include <ofImage.h>

namespace helpers {
	template <typename N> N Min(const N &a, const N &b) {
		return a < b ? a : b;
	}

	template <typename N> N Max(const N &a, const N &b) {
		return a > b ? a : b;
	}
}