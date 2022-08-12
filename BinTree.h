#pragma once
#if defined( DSA_REDBLACK )
#define stature(p) ((p) ? (p)->height : 0) //红黑树节点的黑高度（NULL视作外部节点，对应于0）
#else
#define stature(p) ((p) ? (p)->height : -1) //其余BST中节点的高度（NUll视作空树，对应于-1）
#endif
namespace dsa {
	typedef enum { RB_RED, RB_BLACK } RBColor; //节点颜色
	template <typename T> struct BinNode;
	template <typename T> using BinNodePosi = BinNode<T>*; //节点位置
	template <typename T> struct BinNode { //二叉树节点模板类
	// 成员（为简化描述起见统一开放，读者可根据需要进一步封装）
	   T data; //数值
	   BinNodePosi<T> parent, lc, rc; //父节点及左、右孩子
	   int height; //高度（通用）
	   int npl; //Null Path Length（左式堆，也可直接用height代替）
	   RBColor color; //颜色（红黑树）
	// 构造函数
	   BinNode() :
	      parent(NULL), lc(NULL), rc(NULL), height(0), npl(1), color(RB_RED) { }
	   BinNode(T e, BinNodePosi<T> p = NULL, BinNodePosi<T> lc = NULL, BinNodePosi<T> rc = NULL,
	             int h = 0, int l = 1, RBColor c = RB_RED) :
	      data(e), parent(p), lc(lc), rc(rc), height(h), npl(l), color(c) { }
	// 操作接口
	   int size(); //统计当前节点后代总数，亦即以其为根的子树的规模    <------[后代总数]
	   BinNodePosi<T> insertAsLC(T const& e); //作为当前节点的左孩子插入新节点
	   BinNodePosi<T> insertAsRC(T const& e); //作为当前节点的右孩子插入新节点
	   BinNodePosi<T> succ(); //取当前节点的直接后继 //中序遍历下的直接后继
	   template <typename VST> void travLevel(VST& fun); //子树层次遍历
	   template <typename VST> void travPre(VST& fun); //子树先序遍历
	   template <typename VST> void travIn(VST& fun); //子树中序遍历
	   template <typename VST> void travPost(VST& fun); //子树后序遍历
	// 比较器、判等器（各列其一，其余自行补充）
	   bool operator< (BinNode const& bn) { return data < bn.data; } //小于
	   bool operator== (BinNode const& bn) { return data == bn.data; } //等于
	};
	
	template<typename T>
	inline int BinNode<T>::size(){//时间复杂度 O(n) 线性的时间
		int s = 1;
		if(lc) s += lc->size();//递归
		if(rc) s += rc->size();
		return s;
	}

	template<typename T>
	inline BinNodePosi<T> BinNode<T>::insertAsLC(T const& e)
	{
		return lc = new BinNode(e, this);
	}

	template<typename T>
	inline BinNodePosi<T> BinNode<T>::insertAsRC(T const& e)
	{
		return rc = new BinNode(e, this);
	}
	//约定高度为-1
	template <typename T> class BinTree { //二叉树模板类
	protected:
	   int _size; BinNodePosi<T> _root; //规模、根节点
	   virtual int updateHeight(BinNodePosi<T> x); //更新节点x的高度
	   void updateHeightAbove(BinNodePosi<T> x); //更新节点x及其祖先的高度
	public:
	   BinTree() : _size(0), _root(NULL) { } //构造函数
	   ~BinTree() { if (0 < _size) remove(_root); } //析构函数
	   int size() const { return _size; } //规模
	   bool empty() const { return !_root; } //判空
	   BinNodePosi<T> root() const { return _root; } //树根
	   BinNodePosi<T> insert(T const&); //插入根节点
	   BinNodePosi<T> insert(T const&, BinNodePosi<T>); //插入左孩子
	   BinNodePosi<T> insert(BinNodePosi<T>, T const&); //插入右孩子
	   BinNodePosi<T> attach(BinTree<T> *&, BinNodePosi<T>); //接入左子树
	   BinNodePosi<T> attach(BinNodePosi<T>, BinTree<T> *&); //接入右子树
	   int remove(BinNodePosi<T>); //子树删除
	   BinTree<T>* secede(BinNodePosi<T>); //子树分离
	   template < typename VST> //操作器
	   void travLevel(VST & visit) { if (_root) _root->travLevel(visit); } //层次遍历
	   template < typename VST> //操作器
	   void travPre(VST & visit) { if (_root) _root->travPre(visit); } //先序遍历
	   template < typename VST> //操作器
	   void travIn(VST & visit) { if (_root) _root->travIn(visit); } //中序遍历
	   template < typename VST> //操作器
	   void travPost(VST & visit) { if (_root) _root->travPost(visit); } //后序遍历
	   bool operator< (BinTree<T> const& t) //比较器（其余自行补充）
	   { return _root && t._root && lt(_root, t._root); }
	   bool operator== (BinTree<T> const& t) //判等器
	   { return _root && t._root && (_root == t._root); }
	
	};
	template<typename T>
	inline int BinTree<T>::updateHeight(BinNodePosi<T> x){
		return x->height = 1 + max(stature(x->lc),stature(x->rc));
	}
	template<typename T>
	inline void BinTree<T>::updateHeightAbove(BinNodePosi<T> x){
		while(x){
			updateHeight(x);
			x = x->parent;
		}
	}
	//BinTree
}