#pragma once
#include"algo.h"
namespace dsa {
	using Rank = int; //��
#define DEFAULT_CAPACITY  3 //Ĭ�ϵĳ�ʼ������ʵ��Ӧ���п�����Ϊ����
	template <typename T> class Vector { //����ģ����
	protected:
		Rank _size; Rank _capacity;  T* _elem; //��ģ��������������
		void copyFrom(T const* A, Rank lo, Rank hi); //������������A[lo, hi)
		void expand(); //�ռ䲻��ʱ����
		void shrink(); //װ�����ӹ�Сʱѹ��
		bool bubble(Rank lo, Rank hi); //ɨ�轻��
		void bubbleSort(Rank lo, Rank hi); //���������㷨
		Rank maxItem(Rank lo, Rank hi); //ѡȡ���Ԫ��
		void selectionSort(Rank lo, Rank hi); //ѡ�������㷨
		void merge(Rank lo, Rank mi, Rank hi); //�鲢�㷨
		void mergeSort(Rank lo, Rank hi); //�鲢�����㷨
		void heapSort(Rank lo, Rank hi); //�������Ժ�����ȫ�ѽ��⣩
		Rank partition(Rank lo, Rank hi); //��㹹���㷨
		void quickSort(Rank lo, Rank hi); //���������㷨
		void shellSort(Rank lo, Rank hi); //ϣ�������㷨
	public:
		// ���캯��
		Vector(int c = DEFAULT_CAPACITY, Rank s = 0, T v = 0) //����Ϊc����ģΪs������Ԫ�س�ʼΪv
		{
			_elem = new T[_capacity = c]; for (_size = 0; _size < s; _elem[_size++] = v);
		} //s<=c
		Vector(T const* A, Rank n) { copyFrom(A, 0, n); } //�������帴��
		Vector(T const* A, Rank lo, Rank hi) { copyFrom(A, lo, hi); } //����
		Vector(Vector<T> const& V) { copyFrom(V._elem, 0, V._size); } //�������帴��
		Vector(Vector<T> const& V, Rank lo, Rank hi) { copyFrom(V._elem, lo, hi); } //����
	// ��������
		~Vector() { delete[] _elem; } //�ͷ��ڲ��ռ�
	// ֻ�����ʽӿ�
		Rank size() const { return _size; } //��ģ
		bool empty() const { return !_size; } //�п�
		Rank find(T const& e) const { return find(e, 0, _size); } //���������������
		Rank find(T const& e, Rank lo, Rank hi) const; //���������������
		Rank search(T const& e) const //���������������
		{
			return (0 >= _size) ? -1 : search(e, 0, _size);
		}
		Rank search(T const& e, Rank lo, Rank hi) const; //���������������
		// ��д���ʽӿ�
		T& operator[] (Rank r); //�����±������������������������ʽ���ø�Ԫ��
		const T& operator[] (Rank r) const; //����������ֵ�����ذ汾
		Vector<T>& operator= (Vector<T> const& b); //���ظ�ֵ���������Ա�ֱ�ӿ�¡����
		T remove(Rank r); //ɾ����Ϊr��Ԫ��
		int remove(Rank lo, Rank hi); //ɾ����������[lo, hi)֮�ڵ�Ԫ��
		Rank insert(Rank r, T const& e); //����Ԫ��
		Rank insert(T const& e) { return insert(_size, e); } //Ĭ����ΪĩԪ�ز���
		void sort(Rank lo, Rank hi); //��[lo, hi)����
		void sort() { sort(0, _size); } //��������
		Rank deduplicate(); //����ȥ��
		Rank uniquify(); //����ȥ��
	// ����
		void traverse(void (*) (T& visit)); //������ʹ�ú���ָ�룬ֻ����ֲ����޸ģ�
		template <class BinFunc> void traverse(BinFunc& visit); //������ʹ�ú������󣬿�ȫ�����޸ģ�
	};
	template<typename T>
	inline void Vector<T>::copyFrom(T const* A, Rank lo, Rank hi){
		_elem = new T[_capacity = 2 * (hi - lo)];
		_size = 0;
		while (lo < hi) _elem[_size++] = A[lo++];
	}

	template<typename T>
	inline void Vector<T>::expand(){
		if(_size<_capacity) return;//��δ����ʱ�� ��������
		_capacity = max(_capacity, DEFAULT_CAPACITY);
		T* oldElem = _elem;
		_elem = new T[_capacity<<=1];
		for(int i=0;i<_size;i++)
			_elem[i] = oldElem[i];
		delete[] oldElem;
	}

	template<typename T>
	inline void Vector<T>::shrink(){
		if(_capacity<DEFAULT_CAPACITY<<1) return;//��δ����ʱ�� ��������
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
	inline Rank Vector<T>::find(T const& e, Rank lo, Rank hi) const//�������е��㷨
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


