#pragma once
#if defined( DSA_REDBLACK )
#define stature(p) ((p) ? (p)->height : 0) //������ڵ�ĺڸ߶ȣ�nullptr�����ⲿ�ڵ㣬��Ӧ��0��
#else
#define stature(p) ((p) ? (p)->height : -1) //����BST�нڵ�ĸ߶ȣ�NUll������������Ӧ��-1��
#endif
namespace dsa {
	typedef enum { RB_RED, RB_BLACK } RBColor; //�ڵ���ɫ
	template <class T> struct BinNode;
	template <class T> using BinNodePosi = BinNode<T>*; //�ڵ�λ��
	template <class T> struct BinNode { //�������ڵ�ģ����
	// ��Ա��Ϊ���������ͳһ���ţ����߿ɸ�����Ҫ��һ����װ��
		T data; //��ֵ
		BinNodePosi<T> parent, lc, rc; //���ڵ㼰���Һ���
		int height; //�߶ȣ�ͨ�ã�
		int npl; //Null Path Length����ʽ�ѣ�Ҳ��ֱ����height���棩
		RBColor color; //��ɫ���������
	// ���캯��
		BinNode() :
		   parent(nullptr), lc(nullptr), rc(nullptr), height(0), npl(1), color(RB_RED) { }
		BinNode(T e, BinNodePosi<T> p = nullptr, BinNodePosi<T> lc = nullptr, BinNodePosi<T> rc =nullptr,
		          int h = 0, int l = 1, RBColor c = RB_RED) :
		   data(e), parent(p), lc(lc), rc(rc), height(h), npl(l), color(c) { }
	// �����ӿ�
		int size(); //ͳ�Ƶ�ǰ�ڵ����������༴����Ϊ���������Ĺ�ģ    <------[�������]
		BinNodePosi<T> insertAsLC(T const& e); //��Ϊ��ǰ�ڵ�����Ӳ����½ڵ�
		BinNodePosi<T> insertAsRC(T const& e); //��Ϊ��ǰ�ڵ���Һ��Ӳ����½ڵ�
		BinNodePosi<T> succ(); //ȡ��ǰ�ڵ��ֱ�Ӻ�� //��������µ�ֱ�Ӻ��
		template <class VST> void travLevel(VST& fun) {
			Queue<BinNodePosi<T> > q; //��α�������
			q.enqueue(this); //�����
			while (!q.empty()) { //�����в���ʱ
				BinNodePosi<T> x = q.dequeue(); //������
				fun(x->data); //����x
				if (x->lc) q.enqueue(x->lc); //��������ӣ������
				if (x->rc) q.enqueue(x->rc); //������Һ��ӣ������
			}

		}
		template <class VST> 
		void travPre(VST& fun) {//ʱ�临�Ӷ�ΪO(n)���ռ临�Ӷ�ΪO(n)
			Stack<BinNodePosi<T> > s; //�������ջ
			BinNodePosi<T> x = this;
			if(x) s.push(x); //��ջ
			while (!s.empty()) { //��ջ����ʱ
				x = s.pop(); //��ջ
				fun(x->data); //����x
				if (x->rc) s.push(x->rc); //������Һ��ӣ���ջ
				if (x->lc) s.push(x->lc); //��������ӣ���ջ
			}
		} //�����������
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
		template <class VST> void travPost(VST& fun) {//�ݹ���ʽ�ı���
			if (this->lc) travPost(fun);
			if (this->rc) travPost(fun);
			fun(this->data);
		}
	// �Ƚ������е�����������һ���������в��䣩
		bool operator< (BinNode const& bn) { return data < bn.data; } //С��
		bool operator== (BinNode const& bn) { return data == bn.data; } //����
	};
	
	template<class T>
	inline int BinNode<T>::size(){//ʱ�临�Ӷ� O(n) ���Ե�ʱ��
		int s = 1;
		if(lc) s += lc->size();//�ݹ�
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
	//Լ���߶�Ϊ-1
	template <class T>class BinTree { //������ģ����
	protected:
		int _size; BinNodePosi<T> _root; //��ģ�����ڵ�
		virtual int updateHeight(BinNodePosi<T> x); //���½ڵ�x�ĸ߶�
		void updateHeightAbove(BinNodePosi<T> x); //���½ڵ�x�������ȵĸ߶�
	public:
		BinTree() : _size(0), _root(nullptr) { } //���캯��
		~BinTree() { if (0 < _size) remove(_root); } //��������
		int size() const { return _size; } //��ģ
		bool empty() const { return !_root; } //�п�
		BinNodePosi<T> root() const { return _root; } //����
		BinNodePosi<T> insert(T const& e); //������ڵ�
		BinNodePosi<T> insert(T const& e, BinNodePosi<T> x); //��������
		BinNodePosi<T> insertR(BinNodePosi<T> x, T const& e); //�����Һ���
		BinNodePosi<T> attach(BinTree<T> *&, BinNodePosi<T>); //����������
		BinNodePosi<T> attach(BinNodePosi<T>, BinTree<T> *&); //����������
		int remove(BinNodePosi<T>); //����ɾ��
		BinTree<T>* secede(BinNodePosi<T>); //��������
		template <class T,class VST> //������
		void travLevel(VST & visit) {
		  if (_root) _root->travLevel(visit); 
		} //��α���
		template < typename VST> //������
		void travLevel(VST & visit) { 
			if (_root) _root->travLevel(visit); 
		}
		template < typename VST> //������
		void travPre(VST & visit) { if (_root) _root->travPre(visit); } //�������
		template < typename VST> //������
		void travIn(VST & visit) { if (_root) _root->travIn(visit); } //�������
		template < typename VST> //������
		void travPost(VST & visit) { if (_root) _root->travPost(visit); } //�������
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