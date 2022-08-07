#pragma once
namespace dsa {
	
	template<class T>
	T clamp(T value, T min, T max) {
		if (value < min)return min;
		if (value > max)return max;
		return value;
	}
	
}