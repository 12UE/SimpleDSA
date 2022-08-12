#pragma once
#if defined( DSA_REDBLACK )
#define stature(p) ((p) ? (p)->height : 0) //������ڵ�ĺڸ߶ȣ�NULL�����ⲿ�ڵ㣬��Ӧ��0��
#else
#define stature(p) ((p) ? (p)->height : -1) //����BST�нڵ�ĸ߶ȣ�NUll������������Ӧ��-1��
#endif
namespace dsa {
	typedef enum { RB_RED, RB_BLACK } RBColor; //�ڵ���ɫ
	template <typename T> struct BinNode;
	template <typename T> using BinNodePosi = BinNode<T>*; //�ڵ�λ��
	template <typename T> struct BinNode { //�������ڵ�ģ����
	// ��Ա��Ϊ���������ͳһ���ţ����߿ɸ�����Ҫ��һ����װ��
	   T data; //��ֵ
	   BinNodePosi<T> parent, lc, rc; //���ڵ㼰���Һ���
	   int height; //�߶ȣ�ͨ�ã�
	   int npl; //Null Path Length����ʽ�ѣ�Ҳ��ֱ����height���棩
	   RBColor color; //��ɫ���������
	// ���캯��
	   BinNode() :
	      parent(NULL), lc(NULL), rc(NULL), height(0), npl(1), color(RB_RED) { }
	   BinNode(T e, BinNodePosi<T> p = NULL, BinNodePosi<T> lc = NULL, BinNodePosi<T> rc = NULL,
	             int h = 0, int l = 1, RBColor c = RB_RED) :
	      data(e), parent(p), lc(lc), rc(rc), height(h), npl(l), color(c) { }
	// �����ӿ�
	   int size(); //ͳ�Ƶ�ǰ�ڵ����������༴����Ϊ���������Ĺ�ģ    <------[�������]
	   BinNodePosi<T> insertAsLC(T const& e); //��Ϊ��ǰ�ڵ�����Ӳ����½ڵ�
	   BinNodePosi<T> insertAsRC(T const& e); //��Ϊ��ǰ�ڵ���Һ��Ӳ����½ڵ�
	   BinNodePosi<T> succ(); //ȡ��ǰ�ڵ��ֱ�Ӻ�� //��������µ�ֱ�Ӻ��
	   template <typename VST> void travLevel(VST& fun); //������α���
	   template <typename VST> void travPre(VST& fun); //�����������
	   template <typename VST> void travIn(VST& fun); //�����������
	   template <typename VST> void travPost(VST& fun); //�����������
	// �Ƚ������е�����������һ���������в��䣩
	   bool operator< (BinNode const& bn) { return data < bn.data; } //С��
	   bool operator== (BinNode const& bn) { return data == bn.data; } //����
	};
	
	template<typename T>
	inline int BinNode<T>::size(){//ʱ�临�Ӷ� O(n) ���Ե�ʱ��
		int s = 1;
		if(lc) s += lc->size();//�ݹ�
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
	//Լ���߶�Ϊ-1
	template <typename T> class BinTree { //������ģ����
	protected:
	   int _size; BinNodePosi<T> _root; //��ģ�����ڵ�
	   virtual int updateHeight(BinNodePosi<T> x); //���½ڵ�x�ĸ߶�
	   void updateHeightAbove(BinNodePosi<T> x); //���½ڵ�x�������ȵĸ߶�
	public:
	   BinTree() : _size(0), _root(NULL) { } //���캯��
	   ~BinTree() { if (0 < _size) remove(_root); } //��������
	   int size() const { return _size; } //��ģ
	   bool empty() const { return !_root; } //�п�
	   BinNodePosi<T> root() const { return _root; } //����
	   BinNodePosi<T> insert(T const&); //������ڵ�
	   BinNodePosi<T> insert(T const&, BinNodePosi<T>); //��������
	   BinNodePosi<T> insert(BinNodePosi<T>, T const&); //�����Һ���
	   BinNodePosi<T> attach(BinTree<T> *&, BinNodePosi<T>); //����������
	   BinNodePosi<T> attach(BinNodePosi<T>, BinTree<T> *&); //����������
	   int remove(BinNodePosi<T>); //����ɾ��
	   BinTree<T>* secede(BinNodePosi<T>); //��������
	   template < typename VST> //������
	   void travLevel(VST & visit) { if (_root) _root->travLevel(visit); } //��α���
	   template < typename VST> //������
	   void travPre(VST & visit) { if (_root) _root->travPre(visit); } //�������
	   template < typename VST> //������
	   void travIn(VST & visit) { if (_root) _root->travIn(visit); } //�������
	   template < typename VST> //������
	   void travPost(VST & visit) { if (_root) _root->travPost(visit); } //�������
	   bool operator< (BinTree<T> const& t) //�Ƚ������������в��䣩
	   { return _root && t._root && lt(_root, t._root); }
	   bool operator== (BinTree<T> const& t) //�е���
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