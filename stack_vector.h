#pragma once
#include"Vector.h"
namespace dsa {
	//这种方法插入删除都是O(n)
	template <typename T> class Stack : public Vector<T> { //将向量的首/末端作为栈底/顶
	public: //原有接口一概沿用
		void push(T const& e) { insert(e); } //入栈：等效于将新元素作为向量的末元素插入
		T pop() { return remove(size() - 1); } //出栈：等效于删除向量的末元素
		T& top() { return (*this)[size() - 1]; } //取顶：直接返回向量的末元素
	};
}
