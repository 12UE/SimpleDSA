#pragma once
namespace dsa {
	using Rank = int; //��
	template <typename T> struct ListNode;
	template <typename T> using ListNodePosi = ListNode<T>*; //�б�ڵ�λ��
	template <typename T> struct ListNode { //�б�ڵ�ģ���ࣨ��˫��������ʽʵ�֣�
	// ��Ա
	   T data; 
	   ListNodePosi<T> pred; 
	   ListNodePosi<T> succ; //��ֵ��ǰ�������
	// ���캯��
	   ListNode() = default;//���header��trailer�Ĺ���
	   ListNode(T e, ListNodePosi<T> p = nullptr, ListNodePosi<T> s = nullptr): data(e), pred(p), succ(s) {} //Ĭ�Ϲ�����
	// �����ӿ�
	   ListNodePosi<T> insertAsPred(T const& e) {//������ǰ�ڵ�֮ǰ�����½ڵ�
			ListNodePosi<T> x = new ListNode(e, pred, this);
			pred->succ = x;
			pred = x;
			return x;
	   }
	   ListNodePosi<T> insertAsSucc(T const& e) { //���浱ǰ�ڵ�֮������½ڵ�
			ListNodePosi<T> x = new ListNode(e, this, succ);
			succ->pred = x; 
			succ = x;
			return x;
	   }
	};

	template <typename T> class List { //�б�ģ����
	private:
	   int _size; ListNodePosi<T> header, trailer; //��ģ��ͷ�ڱ���β�ڱ�
	protected:
	   void init(); //�б���ʱ�ĳ�ʼ��
	   int clear(); //������нڵ�
	   void copyNodes(ListNodePosi<T> p, int n); //�����б�����λ��p���n��
	   ListNodePosi<T> merge(ListNodePosi<T> p, int n, List<T>& L, ListNodePosi<T> q, int m); //�鲢
	   void mergeSort() { mergeSort(first(), _size) };
	   void mergeSort(ListNodePosi<T> &p , int n); //�Դ�p��ʼ������n���ڵ�鲢����
	   void selectionSort(ListNodePosi<T> p, int n); //�Դ�p��ʼ������n���ڵ�ѡ������
	   void insertionSort(ListNodePosi<T> p, int n); //�Դ�p��ʼ������n���ڵ��������

	public:
	// ���캯��
	   List() { init(); } //Ĭ��
	   List(List<T> const& L); //���帴���б�L
	   List(List<T> const& L, Rank r, int n); //�����б�L���Ե�r�����n��
	   List(ListNodePosi<T> p, int n); //�����б�����λ��p���n��
	// ��������
	   ~List(); //�ͷţ�����ͷ��β�ڱ����ڵģ����нڵ�
	// ֻ�����ʽӿ�
	   Rank size() const { return _size; } //��ģ
	   bool empty() const { return _size <= 0; } //�п�
	   T& operator[] (Rank r) const; //���أ�֧��ѭ�ȷ��ʣ�Ч�ʵͣ�
	   ListNodePosi<T> first() const { return header->succ; } //�׽ڵ�λ��
	   ListNodePosi<T> last() const { return trailer->pred; } //ĩ�ڵ�λ��
	   bool valid(ListNodePosi<T> p) //�ж�λ��p�Ƿ����Ϸ�
	   { return p && (trailer != p) && (header != p); } //��ͷ��β�ڵ��ͬ��nullptr
	   ListNodePosi<T> find(T const& e) const //�����б����
	   { return find(e, _size, trailer); }
	   ListNodePosi<T> find(T const& e, int n, ListNodePosi<T> p) const; //�����������
	   ListNodePosi<T> search(T const& e) const //�����б����
	   { return search(e, _size, trailer); }
	   ListNodePosi<T> search(T const& e, int n, ListNodePosi<T> p) const; //�����������
	   ListNodePosi<T> selectMax(ListNodePosi<T> p, int n); //��p����n-1�������ѡ�������
	   ListNodePosi<T> selectMax() { return selectMax(header->succ, _size); } //���������
	// ��д���ʽӿ�
	   ListNodePosi<T> insertAsFirst(T const& e); //��e�����׽ڵ����
	   ListNodePosi<T> insertAsLast(T const& e); //��e����ĩ�ڵ����
	   ListNodePosi<T> emplace_back(ListNodePosi<T> p, T const& e); //��e����p�ĺ�̲���
	   ListNodePosi<T> emplace_front(ListNodePosi<T> p,T const& e); //��e����p��ǰ������ insetBefore
	   T remove(ListNodePosi<T> p); //ɾ���Ϸ�λ��p���Ľڵ�,���ر�ɾ���ڵ�
	   Rank remove(ListNodePosi<T> p, int n); //ɾ��p����n-1�����
	   void merge(List<T>& L) { merge(header->succ, _size, L, L.header->succ, L._size); } //ȫ�б�鲢
	   void sort(ListNodePosi<T> p, int n); //�б���������
	   void sort() { sort(first(), _size); } //�б���������
	   int deduplicate(); //����ȥ��
	   int uniquify(); //����ȥ��
	   void reverse(); //ǰ���ã�ϰ�⣩
	// ����
	   void traverse(void (*) (T& visit)); //����������ʵʩvisit����������ָ�룬ֻ����ֲ����޸ģ�
	   template < typename VST> //������
	   void traverse(VST& visit); //����������ʵʩvisit�������������󣬿�ȫ�����޸ģ�
	   template<class VST>
	   void traverse(ListNodePosi<T> begin,ListNodePosi<T> end,VST & visit) const;

	}; //List

	template<typename T>
	inline void List<T>::init(){
		header = new ListNode<T>; 
		trailer = new ListNode<T>;
		header->succ = trailer; header->pred = nullptr;
		trailer->pred = header; trailer->succ = nullptr;
		_size = 0;
	}

	template<typename T>
	inline int List<T>::clear()
	{
	   int oldSize = _size;
	   while (0 < _size) {
		   remove(header->succ);
	   }
	   return oldSize;
	}

	template<typename T>
	inline void List<T>::copyNodes(ListNodePosi<T> p, int n)
	{
	   init();//��ʼ��
	   while (n--){
		  insertAsLast(p->data);
		  p = p->succ;
	   }
	}

	template<typename T>
	inline ListNodePosi<T> List<T>::merge(ListNodePosi<T> p, int n, List<T>& L, ListNodePosi<T> q, int m)
	{
		ListNodePosi<T> pp = p->pred; //�鲢֮��p���ܲ���ָ���׽ڵ㣬�����ȼ��䣬�Ա��ڷ���ǰ����
		while ((0 < m) && (q != p)) //q��δ���磨����mergeSort()�У�p����δ���磩֮ǰ
		   if ((0 < n) && (p->data <= q->data)) //��p��δ������v(p) <= v(q)����
		      { p = p->succ; n--; } //pֱ�Ӻ��ƣ�����ɹ���
		   else //���򣬽�qת����p֮ǰ������ɹ���
		      { emplace_front(p, L.remove((q = q->succ)->pred)); m--; }
		return pp->succ; //���µ��׽ڵ�
	}

	template<typename T>
	inline void List<T>::mergeSort(ListNodePosi<T>& p, int n)
	{
		if (n < 2) return;
		int m = n >> 1;
		ListNodePosi<T> q = p;
		for (int i = 0; i < m; i++) {
			q = q->succ;
		}
		mergeSort(p, m);
		mergeSort(q, n - m);
		p = merge(p, m, *this, q, n - m);
	}
	template<typename T>
	inline void List<T>::selectionSort(ListNodePosi<T> p, int n){//ʱ�临�Ӷ�ΪO(n^2)
		ListNodePosi<T> head = p->pred;
		ListNodePosi<T> tail = p;
		for (int i = 0; i < n; i++) tail = tail->succ;
		while (1 < n){
			emplace_front(tail, remove(selectMax(head->succ, n)));
			tail = tail->pred;
			n--;
		}
	}

	template<typename T>
	inline void List<T>::insertionSort(ListNodePosi<T> p, int n){
		for (int i = 0; i < n; i++) {
			emplace_back(search(p->data, i, p), p->data);
			p = p->succ;
			remove(p->pred);
		}
	}


	template<typename T>
	inline List<T>::List(List<T> const& L)
	{
	   copyNodes(L.first(), L._size);
	}

	template<typename T>
	inline List<T>::List(List<T> const& L, Rank r, int n)
	{
	   copyNodes(L[r], n);
	}

	template<typename T>
	inline List<T>::List(ListNodePosi<T> p, int n){
		copyNodes(p, n);
	}

	template<typename T>
	inline List<T>::~List()
	{
	   clear();
	   delete header;
	   delete trailer;
	}

	template<typename T>
	inline T& List<T>::operator[](Rank r) const{//Ч�ʵͣ����Ƽ�ʹ�� ʱ�临�Ӷ�O(n)
		auto p = first();
		while (0 < r--) {
			p = p->succ;
		}
		return p->data;
	}

	template<typename T>
	inline ListNodePosi<T> List<T>::find(T const& e, int n, ListNodePosi<T> p) const//��p��ǰn������Ԫ���в���Ԫ��e
	{
	   if(n<0) return nullptr;
	   if(n>_size) n = _size;
	   while (0 < n--) {
		   if (e == (p = p->pred)->data) {
			   return p;
		   }
	   }
	   return nullptr;
	}

	template<typename T>
	inline ListNodePosi<T> List<T>::search(T const& e, int n, ListNodePosi<T> p) const{//ʱ�临�Ӹ�O��n�� ���������ʵ��
		do {
			p = p->pred; n--; 
		} while ((-1 < n) && (e < p->data));
		return p; 
	}

	template<typename T>
	inline ListNodePosi<T> List<T>::selectMax(ListNodePosi<T> p, int n){
		ListNodePosi<T> max = p;
		for (auto current = p; 1 < n; n--) {
			if (max->data <= (current = current->succ)->data) {
				max = current;
			}	
		}
		return max;
	}

	template<typename T>
	inline ListNodePosi<T> List<T>::insertAsFirst(T const& e)
	{
		_size++; return header->insertAsSucc(e);
	}

	template<typename T>
	inline ListNodePosi<T> List<T>::insertAsLast(T const& e)
	{
		_size++; return trailer->insertAsPred(e);
	}

	template<typename T>
	inline ListNodePosi<T> List<T>::emplace_back(ListNodePosi<T> p, T const& e)//���������e
	{
	   _size++;
	   return p->insertAsSucc(e);
	}

	template<typename T>
	inline ListNodePosi<T> List<T>::emplace_front(ListNodePosi<T> p,T const& e)
	{
		_size++;
	   return p->insertAsPred(e);
	}

	template<typename T>
	inline T List<T>::remove(ListNodePosi<T> p)//ʱ�临�Ӷ�O(1)
	{
	   T e = p->data;
	   p->pred->succ = p->succ;
	   p->succ->pred = p->pred;
	   delete p;
	   _size--;
	   return e;
	}

	template<typename T>
	inline Rank List<T>::remove(ListNodePosi<T> p, int n){//ʱ�临�Ӷ�O(n)
	   auto q = p;
	   while (0 < n--) {
		  q = q->succ;
		  remove(p);
	   }
	}

	template<typename T>
	inline void List<T>::sort(ListNodePosi<T> p, int n){
		mergeSort(p, n);
		//insertionSort(p, n);
	}

	template<typename T>
	inline int List<T>::deduplicate()
	{
		if (_size < 2) return 0;
	   int oldSize = _size;//��¼ԭ���ĳ���
	   auto p=first();
	   Rank r=1;
	   while (trailer!=(p=p->succ)){
		  auto q = find(p->data, r, p);//����p��ǰr������Ԫ�����Ƿ���p->data
		  q?remove(q):r++;
	   }
	   return oldSize - _size;
	}

	template<typename T>
	inline int List<T>::uniquify()
	{
		if (_size < 2) return 0;
		int oldSize = _size;//��¼ԭ���ĳ���
		auto q,p=first();
		while (trailer!=(p=p->succ)){
			if (p->data != q->data) p = q;
			else remove(q);
		}
		return oldSize - _size;
	}

	template<typename T>
	inline void List<T>::reverse()//ǰ����
	{
	   if(_size < 2) return;
	   for(auto p = first(); p != trailer; p = p->succ){
		  dsa::swap(p->pred, p->succ);
	   }
	   dsa::swap(header, trailer);
	}

	template<typename T>
	inline void List<T>::traverse(void(*)(T& visit))//���� ����ʵʩvisit����������ָ�룬ֻ����ֲ����޸ģ�
	{
	   auto p=first();
	   while(p!=trailer){
		  visit(p->data);
		  p=p->succ;
	   }
	}



	template<typename T>
	template<typename VST>
	inline void List<T>::traverse(VST& visit)
	{
		traverse(first(), last(), visit);
	}
	template<typename T>
	template<class VST>
	inline void List<T>::traverse(ListNodePosi<T> begin, ListNodePosi<T> end, VST& visit) const{
		while (begin != end) {
		  visit(begin->data);
		  begin = begin->succ;
	   }
	}
}