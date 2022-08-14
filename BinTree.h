#pragma once
#if defined( DSA_REDBLACK )
#define stature(p) ((p) ? (p)->height : 0) //红黑树节点的黑高度（nullptr视作外部节点，对应于0）
#else
#define stature(p) ((p) ? (p)->height : -1) //其余BST中节点的高度（NUll视作空树，对应于-1）
#endif
namespace dsa {
	typedef enum { RB_RED, RB_BLACK } RBColor; //节点颜色
	template <class T> struct BinNode;
	template <class T> using BinNodePosi = BinNode<T>*; //节点位置
	template <class T> struct BinNode { //二叉树节点模板类
	// 成员（为简化描述起见统一开放，读者可根据需要进一步封装）
		T data; //数值
		BinNodePosi<T> parent, lc, rc; //父节点及左、右孩子
		int height; //高度（通用）
		int npl; //Null Path Length（左式堆，也可直接用height代替）
		RBColor color; //颜色（红黑树）
	// 构造函数
		BinNode() :
		   parent(nullptr), lc(nullptr), rc(nullptr), height(0), npl(1), color(RB_RED) { }
		BinNode(T e, BinNodePosi<T> p = nullptr, BinNodePosi<T> lc = nullptr, BinNodePosi<T> rc =nullptr,
		          int h = 0, int l = 1, RBColor c = RB_RED) :
		   data(e), parent(p), lc(lc), rc(rc), height(h), npl(l), color(c) { }
	// 操作接口
		int size(); //统计当前节点后代总数，亦即以其为根的子树的规模    <------[后代总数]
		BinNodePosi<T> insertAsLC(T const& e); //作为当前节点的左孩子插入新节点
		BinNodePosi<T> insertAsRC(T const& e); //作为当前节点的右孩子插入新节点
		BinNodePosi<T> succ(); //取当前节点的直接后继 //中序遍历下的直接后继
		template <class VST> void travLevel(VST& fun) {
			Queue<BinNodePosi<T> > q; //层次遍历队列
			q.enqueue(this); //入队列
			while (!q.empty()) { //当队列不空时
				BinNodePosi<T> x = q.dequeue(); //出队列
				fun(x->data); //访问x
				if (x->lc) q.enqueue(x->lc); //如果有左孩子，入队列
				if (x->rc) q.enqueue(x->rc); //如果有右孩子，入队列
			}

		}
		template <class VST> 
		void travPre(VST& fun) {//时间复杂度为O(n)，空间复杂度为O(n)
			Stack<BinNodePosi<T> > s; //先序遍历栈
			BinNodePosi<T> x = this;
			if(x) s.push(x); //入栈
			while (!s.empty()) { //当栈不空时
				x = s.pop(); //出栈
				fun(x->data); //访问x
				if (x->rc) s.push(x->rc); //如果有右孩子，入栈
				if (x->lc) s.push(x->lc); //如果有左孩子，入栈
			}
		} //子树先序遍历
		template <class VST> void travIn(VST& fun) {
			BinNodePosi<T> x = this;
			auto goAlongLeftBranch = [&](BinNodePosi<T> x, Stack<BinNodePosi<T>>& s)->void {
				while (x){s.push(x); x = x->lc;}
			};
			Stack<BinNodePosi<T>> s;
			while (1){
				goAlongLeftBranch(x, s);
				if (s.empty()) break;
				x = s.pop();
				fun(x.data);
				x = x->rc;
			}
		}
		template <class VST> void travPost(VST& fun) {//递归形式的遍历
			if (this->lc) travPost(fun);
			if (this->rc) travPost(fun);
			fun(this->data);
		}
	// 比较器、判等器（各列其一，其余自行补充）
		bool operator< (BinNode const& bn) { return data < bn.data; } //小于
		bool operator== (BinNode const& bn) { return data == bn.data; } //等于
	};
	
	template<class T>
	inline int BinNode<T>::size(){//时间复杂度 O(n) 线性的时间
		int s = 1;
		if(lc) s += lc->size();//递归
		if(rc) s += rc->size();
		return s;
	}

	template<class T>
	inline BinNodePosi<T> BinNode<T>::insertAsLC(T const& e)
	{
		return lc = new BinNode(e, this);
	}

	template<class T>
	inline BinNodePosi<T> BinNode<T>::insertAsRC(T const& e)
	{
		return rc = new BinNode(e, this);
	}
	//约定高度为-1
	template <class T>class BinTree { //二叉树模板类
	protected:
		int _size; BinNodePosi<T> _root; //规模、根节点
		virtual int updateHeight(BinNodePosi<T> x); //更新节点x的高度
		void updateHeightAbove(BinNodePosi<T> x); //更新节点x及其祖先的高度
	public:
		BinTree() : _size(0), _root(nullptr) { } //构造函数
		~BinTree() { if (0 < _size) remove(_root); } //析构函数
		int size() const { return _size; } //规模
		bool empty() const { return !_root; } //判空
		BinNodePosi<T> root() const { return _root; } //树根
		BinNodePosi<T> insert(T const& e); //插入根节点
		BinNodePosi<T> insert(T const& e, BinNodePosi<T> x); //插入左孩子
		BinNodePosi<T> insertR(BinNodePosi<T> x, T const& e); //插入右孩子
		BinNodePosi<T> attach(BinTree<T> *&, BinNodePosi<T>); //接入左子树
		BinNodePosi<T> attach(BinNodePosi<T>, BinTree<T> *&); //接入右子树
		int remove(BinNodePosi<T>); //子树删除
		BinTree<T>* secede(BinNodePosi<T>); //子树分离
		template <class T,class VST> //操作器
		void travLevel(VST & visit) {
		  if (_root) _root->travLevel(visit); 
		} //层次遍历
		template < typename VST> //操作器
		void travLevel(VST & visit) { 
			if (_root) _root->travLevel(visit); 
		}
		template < typename VST> //操作器
		void travPre(VST & visit) { if (_root) _root->travPre(visit); } //先序遍历
		template < typename VST> //操作器
		void travIn(VST & visit) { if (_root) _root->travIn(visit); } //中序遍历
		template < typename VST> //操作器
		void travPost(VST & visit) { if (_root) _root->travPost(visit); } //后序遍历
		bool operator< (BinTree<T> const& t){
		  return _root && t._root && lt(_root, t._root); 
		}
		bool operator== (BinTree<T> const& t){
		  return _root && t._root && (_root == t._root); 
		}
	
	};
	template<class T>
	inline int BinTree<T>::updateHeight(BinNodePosi<T> x){
		return x->height = 1 + max(stature(x->lc),stature(x->rc));
	}
	template<class T>
	inline void BinTree<T>::updateHeightAbove(BinNodePosi<T> x){
		while(x){
			updateHeight(x);
			x = x->parent;
		}
	}
	template<class T>
	inline BinNodePosi<T> BinTree<T>::insertR(BinNodePosi<T> x, T const& e){
		_size++;
		x->insertAsRc(e);
		updateHeightAbove(x);
		return x->rc;
	}

	//BinTree
}