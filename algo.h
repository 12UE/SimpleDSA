#pragma once
namespace dsa {
	
	template<class T>
	T& clamp(T value, T min, T max) {
		if (value < min)return min;
		if (value > max)return max;
		return value;
	}
	template<class T>
	void swap(T &a, T &b) {
		T temp = a;
		a = b;
		b = temp;
	}
	template<class Titer,class binPredecate>
	Titer& min_element(const Titer& begin,const Titer& end,binPredecate p){
		Titer min = begin;
		for (Titer it = begin; it != end; ++it) {
			if (p(*it, *min)) {
				min = it;
			}
		}
		return min;
	}
	template<class Iter,class binPredecate>
	void for_each(Iter begin, Iter end, binPredecate fun) {
		for (Iter it = begin; it != end; ++it) {
			fun(*it);
		}
	}
	template <typename... Types>
	size_t hash_val(const Types&... args) {
		size_t seed = 0;
		hash_value(seed, args...);
		return seed;
	}

	template <typename T, typename... Types>
	void hash_value(size_t& seed,
		const T& firstArg,
		const Types&... args) {
		hash_combine(seed, firstArg);
		hash_value(seed, args...);
	}

	template <typename T>
	void hash_value(size_t& seed,
		const T& val) {
		hash_combine(seed, val);
	}

	template<typename T>
	void hash_combine(size_t& seed,
		const T& val) {
		seed ^= std::hash<T>()(val) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
	}

	template<class T>
	T rand() {
		
	}
	

}