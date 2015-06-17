#ifndef SSTL_RBTREE_H
#define SSTL_RBTREE_H

#include"sstl_alloc.h"
#include"sstl_tree.h"
#include"sstl_tuple.h"

SSTL_BEGIN_NAMESPACE
enum RBColor_Type{ RED, BLACK };
static RBColor_Type nil_color = RBColor_Type::BLACK;//�սڵ����ɫ


template<class Value>
class RBTreeNode :public _SearchTreeNodeBase<Value, RBTreeNode<Value>>
{
	typedef _SearchTreeNodeBase<Value, RBTreeNode<Value>> Base;
public:
	using _SearchTreeNodeBase<Value, RBTreeNode<Value>>::MinimumNode;
	using _SearchTreeNodeBase<Value, RBTreeNode<Value>>::MaximumNode;
	typedef RBTreeNode<Value> self;
	RBColor_Type RBColor;
};


template<class Value>
class RBTreeIterator :public _SearchTreeIteratorBase<Value, RBTreeNode<Value>>
{
public:
	using _SearchTreeIteratorBase<Value, RBTreeNode<Value>>::node;
	using typename _SearchTreeIteratorBase<Value, RBTreeNode<Value>>::value_type;
	using typename _SearchTreeIteratorBase<Value, RBTreeNode<Value>>::node_ptr;
protected:
	void Decrement()
	{
		if (node->parent->parent == node&&node->RBColor == RBColor_Type::RED)//��End�ڵ�
		{
			node = node->right;
			return;
		}
		this->BaseDecrement();
	}
	void Increment()
	{
		this->BaseIncrement();
	}
public:
	typedef RBTreeIterator<Value> self;
public:
	self& operator++()
	{
		Increment();
		return *this;
	}
	self operator++(int)
	{
		self tmp = *this;
		++*this;
		return tmp;
	}
	self& operator--()
	{
		Decrement();
		return *this;
	}
	self operator--(int)
	{
		self tmp = *this;
		--*this;
		return tmp;
	}

	value_type& operator*()
	{
		return node->data;
	}
	value_type* operator->()
	{
		return &(node->data);
	}

	bool operator==(const self& x)const{ return node == x.node; }
	bool operator!=(const self& x)const{ return !(*this == x); }

#ifdef __WIN32
	RBTreeIterator(node_ptr n = nullptr):_SearchTreeIteratorBase(n)
	{

	}
#elif defined(__LINUX) |defined(__CROS)
	using _SearchTreeIteratorBase<Value, RBTreeNode<Value>>::_SearchTreeIteratorBase;
#endif
};


template < class Key, class Value, class KeyOfValue, class Compare = Greater<Key>, class Alloc = ALLOC(RBTreeNode<Value>)>
class RBTree :public _SearchTreeBase<Key, Value, KeyOfValue, Compare, RBTreeNode<Value>, RBTreeIterator<Value>, Alloc>
{
public:
	using  _SearchTreeBase<Key, Value, KeyOfValue, Compare, RBTreeNode<Value>, RBTreeIterator<Value>, Alloc>::alloc;
	using  _SearchTreeBase<Key, Value, KeyOfValue, Compare, RBTreeNode<Value>, RBTreeIterator<Value>, Alloc>::End;
	using  _SearchTreeBase<Key, Value, KeyOfValue, Compare, RBTreeNode<Value>, RBTreeIterator<Value>, Alloc>::Begin;
	using  _SearchTreeBase<Key, Value, KeyOfValue, Compare, RBTreeNode<Value>, RBTreeIterator<Value>, Alloc>::PutNode;
	using  _SearchTreeBase<Key, Value, KeyOfValue, Compare, RBTreeNode<Value>, RBTreeIterator<Value>, Alloc>::Root;
	using  _SearchTreeBase<Key, Value, KeyOfValue, Compare, RBTreeNode<Value>, RBTreeIterator<Value>, Alloc>::Leftmost;
	using  _SearchTreeBase<Key, Value, KeyOfValue, Compare, RBTreeNode<Value>, RBTreeIterator<Value>, Alloc>::Rightmost;
	using  _SearchTreeBase<Key, Value, KeyOfValue, Compare, RBTreeNode<Value>, RBTreeIterator<Value>, Alloc>::Header;
	using  _SearchTreeBase<Key, Value, KeyOfValue, Compare, RBTreeNode<Value>, RBTreeIterator<Value>, Alloc>::LeftChild;
	using  _SearchTreeBase<Key, Value, KeyOfValue, Compare, RBTreeNode<Value>, RBTreeIterator<Value>, Alloc>::RightChild;
	using  _SearchTreeBase<Key, Value, KeyOfValue, Compare, RBTreeNode<Value>, RBTreeIterator<Value>, Alloc>::MinimumNode;
	using  _SearchTreeBase<Key, Value, KeyOfValue, Compare, RBTreeNode<Value>, RBTreeIterator<Value>, Alloc>::MaximumNode;
	using  _SearchTreeBase<Key, Value, KeyOfValue, Compare, RBTreeNode<Value>, RBTreeIterator<Value>, Alloc>::DestroyNode;
	using  _SearchTreeBase<Key, Value, KeyOfValue, Compare, RBTreeNode<Value>, RBTreeIterator<Value>, Alloc>::Parent;
	using  _SearchTreeBase<Key, Value, KeyOfValue, Compare, RBTreeNode<Value>, RBTreeIterator<Value>, Alloc>::NodeKey;
	using  _SearchTreeBase<Key, Value, KeyOfValue, Compare, RBTreeNode<Value>, RBTreeIterator<Value>, Alloc>::key_compare;
	using typename  _SearchTreeBase<Key, Value, KeyOfValue, Compare, RBTreeNode<Value>, RBTreeIterator<Value>, Alloc>::link_type;
	using typename  _SearchTreeBase<Key, Value, KeyOfValue, Compare, RBTreeNode<Value>, RBTreeIterator<Value>, Alloc>::size_type;
	using typename  _SearchTreeBase<Key, Value, KeyOfValue, Compare, RBTreeNode<Value>, RBTreeIterator<Value>, Alloc>::node_type;
	using typename  _SearchTreeBase<Key, Value, KeyOfValue, Compare, RBTreeNode<Value>, RBTreeIterator<Value>, Alloc>::iterator;
	using typename  _SearchTreeBase<Key, Value, KeyOfValue, Compare, RBTreeNode<Value>, RBTreeIterator<Value>, Alloc>::reference;
	using typename  _SearchTreeBase<Key, Value, KeyOfValue, Compare, RBTreeNode<Value>, RBTreeIterator<Value>, Alloc>::key_type;
	using typename  _SearchTreeBase<Key, Value, KeyOfValue, Compare, RBTreeNode<Value>, RBTreeIterator<Value>, Alloc>::value_type;
protected:
	typedef RBColor_Type color_type;

	size_type node_count = 0;

	link_type nil_node = nullptr;

	static color_type& Color(link_type x){ return x == nullptr ? nil_color : x->RBColor; }//�սڵ�Ϊ��ɫ

	void __Transplant(link_type base, link_type branch);
	void __LeftRotate(link_type pivot);
	void __RightRotate(link_type pivot);
	void __InsertionFixUp(link_type new_node);

	link_type __CopyAux(link_type src_node)
	{
		if (src_node == nullptr)return nullptr;
		link_type des_node = alloc.Allocate(1);
		alloc.Construct(des_node, node_type());
		*des_node = *src_node;

		des_node->left = __CopyAux(src_node->left);
		if (des_node->left)des_node->left->parent = des_node;

		des_node->right = __CopyAux(src_node->right);
		if (des_node->right)des_node->right->parent = des_node;

		return des_node;
	}
	//�����ǲ���λ�õĸ��ڵ�
	iterator __Insert(link_type ins_pos_father, const Value& v);

	//ɾ����������Ҫɾ���Ľڵ�
	void __DeletionFixUP(link_type pos);
	void Delete(link_type pos);
public:
	//Info
	size_type Size()const{ return node_count; }
	bool Empty()const{ return this->Size() == 0; }
	//Access
	reference operator[](const key_type& key)
	{
		Value tmp;
		KeyOfValue()(tmp) = key;
		auto pair = InsertUnique(tmp);
		return pair.first.node->data;
	}
	iterator Find(const Key& key)const
	{
		link_type res = this->FindNode(key);
		return res == nullptr ? this->End() : res;
	}
	size_type Count(const Key& key)
	{
		iterator iter = this->Find(key);
		size_type n = 0;
		if (iter == this->End())return 0;
		if (this->key_compare(key, key) == true) //�Ǵ��ڵ��ڻ���С�ڵ��ڣ����������ҽ�β
		{
			while (iter !=  this->End() && key == NodeKey(iter.node))
			{
				++n;
				++iter;
			}
		}
		else
		{
			while (key == NodeKey(iter.node))
			{
				++n;
				if (iter == Begin())
				{
					break;
				}
				else --iter;
			}
		}
		return n;
	}
	Pair<iterator, iterator> EqualRange(const Key& key)
	{
		iterator iter_findfirst = Find(key);
		iterator iter_findlast = iter_findfirst;
		size_type n = 0;
		if (iter_findfirst == End())
		{
			return MakePair(End(), End());
		}
		while (iter_findlast != End() && key == NodeKey(iter_findlast.node))
		{
			++iter_findlast;
			++n;
		}

		//��ǰ���ұȽ��鷳����Ϊ��Ҫ��������ҿ����������
		iterator step;
		while (true)
		{
			if (iter_findfirst == Begin())
			{
				++n;
				return MakePair(iter_findfirst, iter_findlast);
			}
			else
			{
				if (key == NodeKey(iter_findfirst.node))
				{
					step = iter_findfirst;
					--iter_findfirst;
					++n;
				}
				else
				{
					return MakePair(step, iter_findlast);
				}
			}
		}


	}
	//Modify
	Pair<iterator, bool> InsertUnique(const Value& v);//�������ظ�
	iterator InsertEqual(const Value& v);//�����ظ�

	void Delete(iterator pos){ Delete(pos.node); }

private:
	void ClearAux(link_type des)
	{
		if (des == nullptr)return;
		if (des->left)ClearAux(des->left);
		if (des->right)ClearAux(des->right);
		delete des;
	}
public:
	void Clear()	//�����нڵ���������������У����Destroy��������Deletion Fix-up
	{
		if (this->Size() != 0)
		{
			ClearAux(Root());
		}
		Root() = nullptr;
		Leftmost() = Header();
		Rightmost() = Header();
		node_count = 0;
	}
	//Ctor
	RBTree() : _SearchTreeBase<Key, Value, KeyOfValue, Compare, RBTreeNode<Value>, RBTreeIterator<Value>, Alloc>()
	{
		nil_node = this->GetNode();
		Color(Header()) = RBColor_Type::RED;
	}
	~RBTree()
	{
		this->Clear();	
		PutNode(nil_node);
		PutNode(Header());
		Header() = nullptr;
	}
	RBTree(const RBTree& x) :RBTree()
	{
		nil_node = this->GetNode();

		node_count = x.node_count;
		if (node_count != 0)
		{
			link_type res = __CopyAux(x.Root());
			Root() = res;
			Root()->parent = Header();
			Leftmost() = MinimumNode(Root());
			Rightmost() = MaximumNode(Root());
		}
	}

	RBTree& operator = (const RBTree& x);
};

template < class Key, class Value, class KeyOfValue, class Compare, class Alloc>
RBTree<Key, Value, KeyOfValue, Compare, Alloc>& RBTree<Key, Value, KeyOfValue, Compare, Alloc>::operator=(const RBTree& x)
{
	if (this == &x)return *this;	//��ֹ�Լ����Լ���ֵ
	this->Clear();
	this->node_count = x.node_count;

	if (node_count != 0)
	{
		link_type res = __CopyAux(x.Root());
		Root() = res;
		Root()->parent = Header();
		Leftmost() = this->MinimumNode(Root());
		Rightmost() = this->MaximumNode(Root());
	}

	return *this;
}

template < class Key, class Value, class KeyOfValue, class Compare, class Alloc>
void RBTree<Key, Value, KeyOfValue, Compare, Alloc>::__LeftRotate(link_type pivot)
{
	link_type y = RightChild(pivot);
	__LLIB_ASSERT(y != nullptr);
	RightChild(pivot) = LeftChild(y);
	if (LeftChild(y) != nullptr)
	{
		Parent(LeftChild(y)) = pivot;
	}
	Parent(y) = Parent(pivot);

	if (Root() == pivot) //pivot�Ǹ��ڵ�
	{
		Root() = y;
	}
	else
	{
		if (LeftChild(Parent(pivot)) == pivot)
		{
			LeftChild(Parent(pivot)) = y;
		}
		else RightChild(Parent(pivot)) = y;
	}

	LeftChild(y) = pivot;
	Parent(pivot) = y;
}

template < class Key, class Value, class KeyOfValue, class Compare, class Alloc>
void RBTree<Key, Value, KeyOfValue, Compare, Alloc>::__RightRotate(link_type pivot)
{
	link_type y = LeftChild(pivot);
	__LLIB_ASSERT(y != nullptr);

	LeftChild(pivot) = RightChild(y);
	if (RightChild(y) != nullptr)
	{
		Parent(RightChild(y)) = pivot;
	}
	Parent(y) = Parent(pivot);

	if (Root() == pivot) //pivot�Ǹ��ڵ�
	{
		Root() = y;
	}
	else
	{
		if (LeftChild(Parent(pivot)) == pivot)
		{
			LeftChild(Parent(pivot)) = y;
		}
		else RightChild(Parent(pivot)) = y;
	}

	RightChild(y) = pivot;
	Parent(pivot) = y;
}

template < class Key, class Value, class KeyOfValue, class Compare, class Alloc>
void RBTree<Key, Value, KeyOfValue, Compare, Alloc>::__InsertionFixUp(link_type new_node)
{
	while (new_node != Root() && Parent(new_node) != Root() && Color(Parent(new_node)) == RBColor_Type::RED)
	{
		if (Parent(new_node) == LeftChild(Parent(Parent(new_node))))
		{
			link_type uncle_node = RightChild(Parent(Parent(new_node)));
			if (Color(uncle_node) == RBColor_Type::RED)
			{
				Color(Parent(new_node)) = RBColor_Type::BLACK;
				Color(uncle_node) = RBColor_Type::BLACK;
				Color(Parent(uncle_node)) = RBColor_Type::RED;
				new_node = Parent(uncle_node);
				continue;
			}
			else
			{
				if (RightChild(Parent(new_node)) == new_node)
				{
					new_node = Parent(new_node);
					__LeftRotate(new_node);
				}
				Color(Parent(new_node)) = RBColor_Type::BLACK;
				Color(Parent(Parent(new_node))) = RBColor_Type::RED;
				__RightRotate(Parent(Parent(new_node)));
			}
		}
		else
		{
			link_type uncle_node = LeftChild(Parent(Parent(new_node)));
			if (Color(uncle_node) == RBColor_Type::RED)
			{
				Color(Parent(new_node)) = RBColor_Type::BLACK;
				Color(uncle_node) = RBColor_Type::BLACK;
				Color(Parent(uncle_node)) = RBColor_Type::RED;
				new_node = Parent(uncle_node);
				continue;
			}
			else
			{
				if (LeftChild(Parent(new_node)) == new_node)
				{
					new_node = Parent(new_node);
					__RightRotate(new_node);
				}
				Color(Parent(new_node)) = RBColor_Type::BLACK;
				Color(Parent(Parent(new_node))) = RBColor_Type::RED;
				__LeftRotate(Parent(Parent(new_node)));
			}
		}
	}
	Color(Root()) = RBColor_Type::BLACK;
}

template < class Key, class Value, class KeyOfValue, class Compare, class Alloc>
Pair<typename RBTree<Key, Value, KeyOfValue, Compare, Alloc>::iterator, bool> RBTree<Key, Value, KeyOfValue, Compare, Alloc>::InsertUnique(const Value& v)
{
	typedef Pair<typename RBTree<Key, Value, KeyOfValue, Compare, Alloc>::iterator, bool> ResPair;
	link_type ins_pos_father;
	link_type equal_node;
	if ((equal_node = this->FindNode(KeyOfValue()(v), ins_pos_father)) != nullptr) //���ظ���ֵ
	{
		iterator resiter(equal_node);
		return ResPair(resiter, false);
	}
	else
	{
		//���ﲻ�ùܲ������ǲ����ң�����__Insert������
		return ResPair(__Insert(ins_pos_father, v), true);
	}
}

template < class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename RBTree<Key, Value, KeyOfValue, Compare, Alloc>::iterator RBTree<Key, Value, KeyOfValue, Compare, Alloc>::InsertEqual(const Value& v)
{
	link_type ins_pos_father, step;
	ins_pos_father = Header();
	step = Root();
	while (step != nullptr)
	{
		const key_type& cur_key = NodeKey(step);
		ins_pos_father = step;
		if (key_compare(KeyOfValue()(v), cur_key))
			step = RightChild(step);
		else
			step = LeftChild(step);
	}
	return __Insert(ins_pos_father, v);
}

template < class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename RBTree<Key, Value, KeyOfValue, Compare, Alloc>::iterator RBTree<Key, Value, KeyOfValue, Compare, Alloc>::__Insert(link_type ins_pos_father, const Value& v)
{
	link_type new_node = this->CreateNode(v);
	LeftChild(new_node) = nullptr;
	RightChild(new_node) = nullptr;
	Parent(new_node) = ins_pos_father;
	Color(new_node) = RBColor_Type::RED;
	++node_count;
	iterator res(new_node);

	if (ins_pos_father == Header())	//˵����β�����ǵ�һ���ڵ�
	{
		Leftmost() = new_node;
		Rightmost() = new_node;
		Root() = new_node;
		Color(new_node) = RBColor_Type::BLACK;
	}
	else
	{
		if (this->key_compare(this->NodeKey(new_node), this->NodeKey(ins_pos_father))) //��Ϊ�Һ��Ӳ���
		{
			if (Rightmost() == ins_pos_father)Rightmost() = new_node;
			RightChild(ins_pos_father) = new_node;
		}
		else
		{
			if (Leftmost() == ins_pos_father)Leftmost() = new_node;
			LeftChild(ins_pos_father) = new_node;
		}
		__InsertionFixUp(new_node);
	}
	return res;
}

template < class Key, class Value, class KeyOfValue, class Compare, class Alloc>
void RBTree<Key, Value, KeyOfValue, Compare, Alloc>::__DeletionFixUP(link_type x)
{
	link_type brother;
	while (Parent(x) != Header() && x != Root() && Color(x) == RBColor_Type::BLACK)
	{
		if (LeftChild(Parent(x)) == x)	//x�ڵ���������
		{
			brother = RightChild(Parent(x));
			if (Color(brother) == RBColor_Type::RED) //Case 1 
			{
				Color(Parent(x)) = RBColor_Type::RED;
				Color(brother) = RBColor_Type::BLACK;
				__LeftRotate(Parent(x));
			}
			else
			{
				if (brother == nullptr || (Color(LeftChild(brother)) == RBColor_Type::BLACK&&Color(RightChild(brother)) == RBColor_Type::BLACK))//Brother��x�ĺ�ɫͬʱ����
				{
					if (brother != nullptr)						//Case 2
						Color(brother) = RBColor_Type::RED;
					x = Parent(x);
				}
				else//Bother���ӽڵ㲻ͬɫ			
				{
					if (Color(RightChild(brother)) == RBColor_Type::BLACK)//Case 3
					{
						Color(LeftChild(brother)) = RBColor_Type::BLACK;
						Color(brother) = RBColor_Type::RED;
						__RightRotate(brother);
						brother = Parent(brother);
					}
					else //Case 4
					{
						Color(brother) = Color(Parent(x));
						Color(RightChild(brother)) = RBColor_Type::BLACK;
						Color(Parent(x)) = RBColor_Type::BLACK;
						__LeftRotate(Parent(x));
						x = Root();	//�˳�ѭ��
					}
				}
			}
		}
		else//x�ڵ���������
		{
			brother = LeftChild(Parent(x));
			if (Color(brother) == RBColor_Type::RED) //Case 1 
			{
				Color(Parent(x)) = RBColor_Type::RED;
				Color(brother) = RBColor_Type::BLACK;
				__RightRotate(Parent(x));
			}
			else
			{
				if (brother == nullptr || (Color(LeftChild(brother)) == RBColor_Type::BLACK&&Color(RightChild(brother)) == RBColor_Type::BLACK))//Brother��x�ĺ�ɫͬʱ����
				{
					if (brother != nullptr)						//Case 2
						Color(brother) = RBColor_Type::RED;
					x = Parent(x);
				}
				else//Bother���ӽڵ㲻ͬɫ			
				{
					if (Color(LeftChild(brother)) == RBColor_Type::BLACK)//Case 3
					{
						Color(RightChild(brother)) = RBColor_Type::BLACK;
						Color(brother) = RBColor_Type::RED;
						__LeftRotate(brother);
						brother = Parent(brother);
					}
					else //Case 4
					{
						Color(brother) = Color(Parent(x));
						Color(LeftChild(brother)) = RBColor_Type::BLACK;
						Color(Parent(x)) = RBColor_Type::BLACK;
						__RightRotate(Parent(x));
						x = Root();	//�˳�ѭ��
					}
				}
			}
		}
	}
	Color(Root()) = RBColor_Type::BLACK;
}

template < class Key, class Value, class KeyOfValue, class Compare, class Alloc>
void RBTree<Key, Value, KeyOfValue, Compare, Alloc>::__Transplant(link_type base, link_type branch)
{
	__LLIB_ASSERT(base != nullptr);
	if (Root() == base)
	{
		Root() = branch;
	}
	else
	{
		if (LeftChild(Parent(base)) == base)
		{
			LeftChild(Parent(base)) = branch;
		}
		else
			RightChild(Parent(base)) = branch;
	}
	if (branch != nullptr)Parent(branch) = Parent(base);
}

template < class Key, class Value, class KeyOfValue, class Compare, class Alloc>
void RBTree<Key, Value, KeyOfValue, Compare, Alloc>::Delete(link_type z)
{
	//z�Ǽ�����ɾ���Ľڵ㣬x�Ǽ����滻y�Ľڵ�
	link_type y, x; //y�Ǽ�����x�ڵ��滻�Ľڵ㣬Ҳ�Ǽ����滻z�Ľڵ�
	color_type y_origin_color;
	link_type tmp = nullptr;
	//�ж�z����������Ϊ�յ����
	y = z;
	y_origin_color = Color(z);
	tmp = Parent(y);
	if (LeftChild(z) == nullptr)	//ֻ���������z�ſ�����Leftmost
	{
		if (Leftmost() == z)
		{
			Leftmost() = RightChild(z) == nullptr ? Parent(z) : MinimumNode(RightChild(z));
		}
		x = RightChild(z);
		__Transplant(y, x);
	}
	else if (RightChild(z) == nullptr)
	{
		if (Rightmost() == z)
		{
			Rightmost() = LeftChild(z) == nullptr ? Parent(z) : MaximumNode(LeftChild(z));
		}
		x = LeftChild(z);
		__Transplant(y, x);
	}
	//x�����������Բ�Ϊ��
	else
	{
		y = MinimumNode(RightChild(z));
		y_origin_color = Color(y);
		x = RightChild(y);
		tmp = Parent(y);
		if (Parent(y) == z)//z��y�ĸ��ף�������ΪҪɾȥz����������ֱ����y�ڵ��滻z�ͺ���
		{
			//do nothing
			tmp = y;
		}
		else
		{
			if (x != nullptr)	//��Ҫ��x�滻y�ڵ�
			{
				__Transplant(y, x);//����y����ų�����
			}
			else	//���˼����else����������ɻ����Բ�QAQ�����ӵĲ�ǳ��QAQ
			{
				if (y == LeftChild(Parent(y)))
					LeftChild(Parent(y)) = nullptr;
				else
					RightChild(Parent(y)) = nullptr;
			}
			RightChild(y) = RightChild(z);
			Parent(RightChild(y)) = y;
		}
		__Transplant(z, y);
		LeftChild(y) = LeftChild(z);
		Parent(LeftChild(y)) = y;
		Color(y) = Color(z);
	}
	//y�Ѻ�ɫ����x��x���к��ɫ���ߺں�ɫ
	if (y_origin_color == color_type::BLACK)
	{
		if (x == nullptr)
		{
			nil_node->parent = tmp;
			nil_node->right = nil_node->left = nullptr;
			Color(nil_node) = color_type::BLACK;
			x = nil_node;
		}
		__DeletionFixUP(x);
	}

	DestroyNode(z);
	node_count--;
}

SSTL_END_NAMESPACE

#endif
