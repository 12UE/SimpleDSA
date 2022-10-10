#pragma once
namespace dsa {
	using Rank = int;
#define DEFAULT_CAPACITY  3 
	template <typename T> class Vector { 
	protected:
		Rank _size; Rank _capacity;  T* _elem;
		void copyFrom(T const* A, Rank lo, Rank hi); 
		void expand(); 
		void shrink(); 
		bool bubble(Rank lo, Rank hi); 
		void bubbleSort(Rank lo, Rank hi); 
		Rank maxItem(Rank lo, Rank hi); 
		void selectionSort(Rank lo, Rank hi); 
		void merge(Rank lo, Rank mi, Rank hi); 
		void mergeSort(Rank lo, Rank hi); 
		void heapSort(Rank lo, Rank hi); 
		Rank partition(Rank lo, Rank hi);
		void quickSort(Rank lo, Rank hi); 
		void shellSort(Rank lo, Rank hi);
		
	public:
		Vector(int c = DEFAULT_CAPACITY, Rank s = 0, T v = 0) {
			_elem = new T[_capacity = c]; for (_size = 0; _size < s; _elem[_size++] = v);
		} //s<=c
		template<typename ...elem>
		Vector(elem...args) {
			_elem = new T[_capacity =sizeof...(args)];
			_size = 0;
			for (auto& e : { args... }) {
				_elem[_size++] = e;
			}
		}
		Vector(T const* A, Rank n) { copyFrom(A, 0, n); }
		Vector(T const* A, Rank lo, Rank hi) { copyFrom(A, lo, hi); } 
		Vector(Vector<T> const& V) { copyFrom(V._elem, 0, V._size); } 
		Vector(Vector<T> const& V, Rank lo, Rank hi) { copyFrom(V._elem, lo, hi); }
		~Vector() { delete[] _elem; }
		class iterator {
		private:
			T* _ptr;
		public:
			iterator(T* ptr) :_ptr(ptr) {}
			T& operator*() { return *_ptr; }
			iterator& operator++(int) { return iterator(_ptr++); }
			iterator& operator++() { ++_ptr; return *this; }
			iterator& operator--(int) { return iterator(_ptr--); }
			iterator& operator--() { --_ptr; return *this; }
			int operator-(iterator other) { return _ptr - other._ptr; }//地址空间连续
			bool operator==(const iterator& rhs) { return _ptr == rhs._ptr; }
			bool operator!=(const iterator& rhs) { return _ptr != rhs._ptr; }
		};
		iterator  begin() { return iterator(_elem); }
		iterator  end() { return iterator(_elem + _size); }
		Rank size() const { return _size; } 
		bool empty() const { return !_size; }
		Rank find(T const& e) const { return find(e, 0, _size); }
		Rank find(T const& e, Rank lo, Rank hi) const;
		Rank search(T const& e) const{
			return (0 >= _size) ? -1 : search(e, 0, _size);
		}
		Rank search(T const& e, Rank lo, Rank hi) const;
		T& operator[] (Rank r);
		const T& operator[] (Rank r) const; 
		Vector<T>& operator= (Vector<T> const& b);
		T remove(Rank r);
		int remove(Rank lo, Rank hi);
		Rank insert(Rank r, T const& e);
		Rank insert(T const& e) { return insert(_size, e); } 
		void sort(Rank lo, Rank hi); 
		void sort() { sort(0, _size); } 
		Rank deduplicate(); 
		Rank uniquify(); 
		void traverse(void (*) (T& visit));
		template <class BinFunc> void traverse(BinFunc& visit);
		void clear();
	};
	template<typename T>
	inline void Vector<T>::copyFrom(T const* A, Rank lo, Rank hi){
		_elem = new T[_capacity = 2 * (hi - lo)];
		_size = 0;
		while (lo < hi) _elem[_size++] = A[lo++];
	}

	template<typename T>
	inline void Vector<T>::expand(){
		if(_size<_capacity) return;
		_capacity = max(_capacity, DEFAULT_CAPACITY);
		T* oldElem = _elem;
		_elem = new T[_capacity<<=1];
		for(int i=0;i<_size;i++)
			_elem[i] = oldElem[i];
		delete[] oldElem;
	}

	template<typename T>
	inline void Vector<T>::shrink(){
		if(_capacity<DEFAULT_CAPACITY<<1) return;
		if (_size << 1 > _capacity) return;//50%
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
		Rank i = 0; T* A = _elem + lo;
		Rank j = 0, lb = mi - lo; T* B = new T[lb]; 
		for (Rank i = 0; i < lb; i++) B[i] = A[i]; 
		Rank k = 0, lc = hi - mi; T* C = _elem + mi; 
		while ((j < lb) && (k < lc)) 
		   A[i++] = (B[j] <= C[k]) ? B[j++] : C[k++]; 
		while (j < lb) 
		   A[i++] = B[j++]; 
		delete[] B; 
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
	inline void Vector<T>::heapSort(Rank lo, Rank hi)
	{
		
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
	inline Rank Vector<T>::find(T const& e, Rank lo, Rank hi) const
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
		if (r < 0) return T();
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
		mergeSort(lo, hi);
	}

	template<typename T>
	inline Rank Vector<T>::deduplicate()//针对
	{
		int oldSize = _size;
		Rank i = 1;
		while (i < _size)
			(find(_elem[i], 0, i) < 0) ?
			i++ : remove(i);
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
	inline void Vector<T>::clear()
	{
		delete[] _elem;
		_elem = NULL;
		_size = 0;
		_capacity = 0;
	}


	template<typename T>
	template<class BinFunc>
	inline void Vector<T>::traverse(BinFunc& visit){
		for(int i=0;i<_size;i++)
			visit(_elem[i]);
	}

}


