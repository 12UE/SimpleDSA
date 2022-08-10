#pragma once
namespace dsa {
	using Rank = int; //秩
	template <typename T> struct ListNode;
	template <typename T> using ListNodePosi = ListNode<T>*; //列表节点位置
	template <typename T> struct ListNode { //列表节点模板类（以双向链表形式实现）
	// 成员
	   T data; 
	   ListNodePosi<T> pred; 
	   ListNodePosi<T> succ; //数值、前驱、后继
	// 构造函数
	   ListNode() = default;//针对header和trailer的构造
	   ListNode(T e, ListNodePosi<T> p = nullptr, ListNodePosi<T> s = nullptr): data(e), pred(p), succ(s) {} //默认构造器
	// 操作接口
	   ListNodePosi<T> insertAsPred(T const& e) {//紧靠当前节点之前插入新节点
			ListNodePosi<T> x = new ListNode(e, pred, this);
			pred->succ = x;
			pred = x;
			return x;
	   }
	   ListNodePosi<T> insertAsSucc(T const& e) { //紧随当前节点之后插入新节点
			ListNodePosi<T> x = new ListNode(e, this, succ);
			succ->pred = x; 
			succ = x;
			return x;
	   }
	};

	template <typename T> class List { //列表模板类
	private:
	   int _size; ListNodePosi<T> header, trailer; //规模、头哨兵、尾哨兵
	protected:
	   void init(); //列表创建时的初始化
	   int clear(); //清除所有节点
	   void copyNodes(ListNodePosi<T> p, int n); //复制列表中自位置p起的n项
	   ListNodePosi<T> merge(ListNodePosi<T> p, int n, List<T>& L, ListNodePosi<T> q, int m); //归并
	   void mergeSort() { mergeSort(first(), _size) };
	   void mergeSort(ListNodePosi<T> &p , int n); //对从p开始连续的n个节点归并排序
	   void selectionSort(ListNodePosi<T> p, int n); //对从p开始连续的n个节点选择排序
	   void insertionSort(ListNodePosi<T> p, int n); //对从p开始连续的n个节点插入排序

	public:
	// 构造函数
	   List() { init(); } //默认
	   List(List<T> const& L); //整体复制列表L
	   List(List<T> const& L, Rank r, int n); //复制列表L中自第r项起的n项
	   List(ListNodePosi<T> p, int n); //复制列表中自位置p起的n项
	// 析构函数
	   ~List(); //释放（包含头、尾哨兵在内的）所有节点
	// 只读访问接口
	   Rank size() const { return _size; } //规模
	   bool empty() const { return _size <= 0; } //判空
	   T& operator[] (Rank r) const; //重载，支持循秩访问（效率低）
	   ListNodePosi<T> first() const { return header->succ; } //首节点位置
	   ListNodePosi<T> last() const { return trailer->pred; } //末节点位置
	   bool valid(ListNodePosi<T> p) //判断位置p是否对外合法
	   { return p && (trailer != p) && (header != p); } //将头、尾节点等同于nullptr
	   ListNodePosi<T> find(T const& e) const //无序列表查找
	   { return find(e, _size, trailer); }
	   ListNodePosi<T> find(T const& e, int n, ListNodePosi<T> p) const; //无序区间查找
	   ListNodePosi<T> search(T const& e) const //有序列表查找
	   { return search(e, _size, trailer); }
	   ListNodePosi<T> search(T const& e, int n, ListNodePosi<T> p) const; //有序区间查找
	   ListNodePosi<T> selectMax(ListNodePosi<T> p, int n); //在p及其n-1个后继中选出最大者
	   ListNodePosi<T> selectMax() { return selectMax(header->succ, _size); } //整体最大者
	// 可写访问接口
	   ListNodePosi<T> insertAsFirst(T const& e); //将e当作首节点插入
	   ListNodePosi<T> insertAsLast(T const& e); //将e当作末节点插入
	   ListNodePosi<T> emplace_back(ListNodePosi<T> p, T const& e); //将e当作p的后继插入
	   ListNodePosi<T> emplace_front(ListNodePosi<T> p,T const& e); //将e当作p的前驱插入 insetBefore
	   T remove(ListNodePosi<T> p); //删除合法位置p处的节点,返回被删除节点
	   Rank remove(ListNodePosi<T> p, int n); //删除p及其n-1个后继
	   void merge(List<T>& L) { merge(header->succ, _size, L, L.header->succ, L._size); } //全列表归并
	   void sort(ListNodePosi<T> p, int n); //列表区间排序
	   void sort() { sort(first(), _size); } //列表整体排序
	   int deduplicate(); //无序去重
	   int uniquify(); //有序去重
	   void reverse(); //前后倒置（习题）
	// 遍历
	   void traverse(void (*) (T& visit)); //遍历，依次实施visit操作（函数指针，只读或局部性修改）
	   template < typename VST> //操作器
	   void traverse(VST& visit); //遍历，依次实施visit操作（函数对象，可全局性修改）
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
	   init();//初始化
	   while (n--){
		  insertAsLast(p->data);
		  p = p->succ;
	   }
	}

	template<typename T>
	inline ListNodePosi<T> List<T>::merge(ListNodePosi<T> p, int n, List<T>& L, ListNodePosi<T> q, int m)
	{
		ListNodePosi<T> pp = p->pred; //归并之后p可能不再指向首节点，故需先记忆，以便在返回前更新
		while ((0 < m) && (q != p)) //q尚未出界（或在mergeSort()中，p亦尚未出界）之前
		   if ((0 < n) && (p->data <= q->data)) //若p尚未出界且v(p) <= v(q)，则
		      { p = p->succ; n--; } //p直接后移，即完成归入
		   else //否则，将q转移至p之前，以完成归入
		      { emplace_front(p, L.remove((q = q->succ)->pred)); m--; }
		return pp->succ; //更新的首节点
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
	inline void List<T>::selectionSort(ListNodePosi<T> p, int n){//时间复杂度为O(n^2)
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
	inline T& List<T>::operator[](Rank r) const{//效率低，不推荐使用 时间复杂度O(n)
		auto p = first();
		while (0 < r--) {
			p = p->succ;
		}
		return p->data;
	}

	template<typename T>
	inline ListNodePosi<T> List<T>::find(T const& e, int n, ListNodePosi<T> p) const//在p的前n个无序元素中查找元素e
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
	inline ListNodePosi<T> List<T>::search(T const& e, int n, ListNodePosi<T> p) const{//时间复杂福O（n） 由于链表的实现
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
	inline ListNodePosi<T> List<T>::emplace_back(ListNodePosi<T> p, T const& e)//后插入数据e
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
	inline T List<T>::remove(ListNodePosi<T> p)//时间复杂度O(1)
	{
	   T e = p->data;
	   p->pred->succ = p->succ;
	   p->succ->pred = p->pred;
	   delete p;
	   _size--;
	   return e;
	}

	template<typename T>
	inline Rank List<T>::remove(ListNodePosi<T> p, int n){//时间复杂度O(n)
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
	   int oldSize = _size;//记录原来的长度
	   auto p=first();
	   Rank r=1;
	   while (trailer!=(p=p->succ)){
		  auto q = find(p->data, r, p);//查找p的前r个无序元素中是否有p->data
		  q?remove(q):r++;
	   }
	   return oldSize - _size;
	}

	template<typename T>
	inline int List<T>::uniquify()
	{
		if (_size < 2) return 0;
		int oldSize = _size;//记录原来的长度
		auto q,p=first();
		while (trailer!=(p=p->succ)){
			if (p->data != q->data) p = q;
			else remove(q);
		}
		return oldSize - _size;
	}

	template<typename T>
	inline void List<T>::reverse()//前后倒置
	{
	   if(_size < 2) return;
	   for(auto p = first(); p != trailer; p = p->succ){
		  dsa::swap(p->pred, p->succ);
	   }
	   dsa::swap(header, trailer);
	}

	template<typename T>
	inline void List<T>::traverse(void(*)(T& visit))//遍历 依次实施visit操作（函数指针，只读或局部性修改）
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