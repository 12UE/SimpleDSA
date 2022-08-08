#pragma once
#include"algo.h"
namespace dsa {
	using Rank = int; //秩
#define DEFAULT_CAPACITY  3 //默认的初始容量（实际应用中可设置为更大）
	template <typename T> class Vector { //向量模板类
	protected:
		Rank _size; Rank _capacity;  T* _elem; //规模、容量、数据区
		void copyFrom(T const* A, Rank lo, Rank hi); //复制数组区间A[lo, hi)
		void expand(); //空间不足时扩容
		void shrink(); //装填因子过小时压缩
		bool bubble(Rank lo, Rank hi); //扫描交换
		void bubbleSort(Rank lo, Rank hi); //起泡排序算法
		Rank maxItem(Rank lo, Rank hi); //选取最大元素
		void selectionSort(Rank lo, Rank hi); //选择排序算法
		void merge(Rank lo, Rank mi, Rank hi); //归并算法
		void mergeSort(Rank lo, Rank hi); //归并排序算法
		void heapSort(Rank lo, Rank hi); //堆排序（稍后结合完全堆讲解）
		Rank partition(Rank lo, Rank hi); //轴点构造算法
		void quickSort(Rank lo, Rank hi); //快速排序算法
		void shellSort(Rank lo, Rank hi); //希尔排序算法
	public:
		// 构造函数
		Vector(int c = DEFAULT_CAPACITY, Rank s = 0, T v = 0) //容量为c、规模为s、所有元素初始为v
		{
			_elem = new T[_capacity = c]; for (_size = 0; _size < s; _elem[_size++] = v);
		} //s<=c
		Vector(T const* A, Rank n) { copyFrom(A, 0, n); } //数组整体复制
		Vector(T const* A, Rank lo, Rank hi) { copyFrom(A, lo, hi); } //区间
		Vector(Vector<T> const& V) { copyFrom(V._elem, 0, V._size); } //向量整体复制
		Vector(Vector<T> const& V, Rank lo, Rank hi) { copyFrom(V._elem, lo, hi); } //区间
	// 析构函数
		~Vector() { delete[] _elem; } //释放内部空间
	// 只读访问接口
		Rank size() const { return _size; } //规模
		bool empty() const { return !_size; } //判空
		Rank find(T const& e) const { return find(e, 0, _size); } //无序向量整体查找
		Rank find(T const& e, Rank lo, Rank hi) const; //无序向量区间查找
		Rank search(T const& e) const //有序向量整体查找
		{
			return (0 >= _size) ? -1 : search(e, 0, _size);
		}
		Rank search(T const& e, Rank lo, Rank hi) const; //有序向量区间查找
		// 可写访问接口
		T& operator[] (Rank r); //重载下标操作符，可以类似于数组形式引用各元素
		const T& operator[] (Rank r) const; //仅限于做右值的重载版本
		Vector<T>& operator= (Vector<T> const& b); //重载赋值操作符，以便直接克隆向量
		T remove(Rank r); //删除秩为r的元素
		int remove(Rank lo, Rank hi); //删除秩在区间[lo, hi)之内的元素
		Rank insert(Rank r, T const& e); //插入元素
		Rank insert(T const& e) { return insert(_size, e); } //默认作为末元素插入
		void sort(Rank lo, Rank hi); //对[lo, hi)排序
		void sort() { sort(0, _size); } //整体排序
		Rank deduplicate(); //无序去重
		Rank uniquify(); //有序去重
	// 遍历
		void traverse(void (*) (T& visit)); //遍历（使用函数指针，只读或局部性修改）
		template <class BinFunc> void traverse(BinFunc& visit); //遍历（使用函数对象，可全局性修改）
	};
	template<typename T>
	inline void Vector<T>::copyFrom(T const* A, Rank lo, Rank hi){
		_elem = new T[_capacity = 2 * (hi - lo)];
		_size = 0;
		while (lo < hi) _elem[_size++] = A[lo++];
	}

	template<typename T>
	inline void Vector<T>::expand(){
		if(_size<_capacity) return;//尚未满的时候 不用扩容
		_capacity = max(_capacity, DEFAULT_CAPACITY);
		T* oldElem = _elem;
		_elem = new T[_capacity<<=1];
		for(int i=0;i<_size;i++)
			_elem[i] = oldElem[i];
		delete[] oldElem;
	}

	template<typename T>
	inline void Vector<T>::shrink(){
		if(_capacity<DEFAULT_CAPACITY<<1) return;//尚未满的时候 不用扩容
		if (_size << 2 > _capacity) return;
		T* oldElem = _elem;
		_elem = new T[_capacity>>1];
		for(int i=0;i<_size;i++)
			_elem[i] = oldElem[i];
		delete[] oldElem;
	}

	template<typename T>
	inline bool Vector<T>::bubble(Rank lo, Rank hi)
	{
		bool sorted = true;
		while (++lo < hi)
			if (_elem[lo - 1] > _elem[lo])
			{
				sorted = false;
				swap(_elem[lo - 1], _elem[lo]);
			}
		return sorted;
	}

	template<typename T>
	inline void Vector<T>::bubbleSort(Rank lo, Rank hi)
	{
		while (!bubble(lo, hi--));
	}

	template<typename T>
	inline Rank Vector<T>::maxItem(Rank lo, Rank hi)
	{
		Rank max = lo;
		for (++lo; lo < hi; ++lo)
			if (_elem[lo] > _elem[max])
				max = lo;
		return max;
	}

	template<typename T>
	inline void Vector<T>::selectionSort(Rank lo, Rank hi)
	{
		while (hi > lo)
		{
			swap(_elem[hi = maxItem(lo, hi)], _elem[lo++]);
		}
	}

	template<typename T>
	inline void Vector<T>::merge(Rank lo, Rank mi, Rank hi)
	{
		T *A=_elem+lo;
		int lb=mi-lo;
		T *B=new T[lb];
		for(Rank i=0;i<lb;B[i]=A[i++];)
		int lc=hi-mi;
		T *C=_elem+mi;
		for(Rank i=0,j=0,k=0;(j<lb)||(k<lc);){
			if((j<lb)&&(!(k<lc)||(B[j]<C[k]))) A[i++]=B[j++]; 
			if((k<lc)&&(!(j<lb)||(C[k]<B[j]))) A[i++]=C[k++];
		}
		delete []B;
	}

	template<typename T>
	inline void Vector<T>::mergeSort(Rank lo, Rank hi)
	{
		if(hi-lo<2) return;
		int mi=(lo+hi)>>1;
		mergeSort(lo,mi);
		mergeSort(mi,hi);
		merge(lo,mi,hi);
	}
	template<typename T>
	inline void Vector<T>::quickSort(Rank lo, Rank hi)
	{
		if(hi-lo<2) return;
		int mi=(lo+hi)>>1;
		swap(_elem[mi],_elem[hi]);
		int i=lo,j=hi-1;
		while(i<j){
			while(_elem[i]<_elem[hi]) i++;
			while(_elem[j]>_elem[hi]) j--;
			if(i<j) swap(_elem[i++],_elem[j--]);
		}
		swap(_elem[i],_elem[hi]);
		quickSort(lo,i-1);
		quickSort(i+1,hi);
	}

	template<typename T>
	inline Rank Vector<T>::find(T const& e, Rank lo, Rank hi) const//输入敏感的算法
	{
		while ((lo < hi--) && (e != _elem[hi]));
		return hi;
	}

	template<typename T>
	inline Rank Vector<T>::search(T const& e, Rank lo, Rank hi) const
	 {
		while (1 < hi - lo) {
			Rank mi = (lo + hi) >> 1;
			(e < _elem[mi]) ? hi = mi : lo = mi;
		}
		return (e == _elem[lo]) ? lo : -1;
	}

	template<typename T>
	inline T& Vector<T>::operator[](Rank r)
	{
		return _elem[r];
	}

	template<typename T>
	inline const T& Vector<T>::operator[](Rank r) const
	{
		return _elem[r];
	}

	template<typename T>
	inline Vector<T>& Vector<T>::operator=(Vector<T> const& b){
		if (this != &b) {
			delete[] _elem;
			copyFrom(b._elem, 0, b._size);
		}
		return *this;
	}

	template<typename T>
	inline T Vector<T>::remove(Rank r){
		T e = _elem[r];
		remove(r, r+1);
		return e;
	}

	template<typename T>
	inline int Vector<T>::remove(Rank lo, Rank hi){
		if(lo==hi) return 0;
		while(hi<_size)
		{
			_elem[lo++] = _elem[hi++];
		}
		_size = lo;
		shrink();
		return hi-lo;
		
	}

	template<typename T>
	inline Rank Vector<T>::insert(Rank r, T const& e){
		expand();
		for(int i=_size;i>r;i--)
			_elem[i] = _elem[i-1];
		_elem[r] = e;
		_size++;
		return r;
	}

	template<typename T>
	inline void Vector<T>::sort(Rank lo, Rank hi)
	{
		quickSort(lo, hi);
	}

	template<typename T>
	inline Rank Vector<T>::deduplicate()
	{
		int oldSize = _size;
		Rank i = 1;
		while (i < _size)
			if (_elem[i] == _elem[i - 1]) remove(i--);
			else i++;
		return oldSize - _size;
	}

	template<typename T>
	inline Rank Vector<T>::uniquify()
	{
		Rank i=0,j=0;
		while(++j<_size)
			if(_elem[i]!=_elem[j]) _elem[++i]=_elem[j];
		_size=++i;
		shrink();
		return j-i;
	}

	template<typename T>
	inline void Vector<T>::traverse(void(*)(T& visit))
	{
		for(int i=0;i<_size;i++)
			(*visit)(_elem[i]);
	}


	template<typename T>
	template<class BinFunc>
	inline void Vector<T>::traverse(BinFunc& visit){
		for(int i=0;i<_size;i++)
			visit(_elem[i]);
	}

}


