#pragma once
namespace dsa {
	template<class T> class Queue :public List<T> {
	public:
		void enqueue(T const& e) { insertAsLast(e); }
		T dequeue() { return remove(first()); }
		T& front() { return first()->data; }
	};
}