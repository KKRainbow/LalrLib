#ifndef SSTL_TREE_H
#define SSTL_TREE_H
#include"sstl_iterator.h"
SSTL_BEGIN_NAMESPACE

template<class Value,class DerivedClass>
class _SearchTreeNodeBase
{
public:
	typedef _SearchTreeNodeBase<Value, DerivedClass> self;
	typedef DerivedClass drived;
	drived* left = nullptr;
	drived* right = nullptr;
	drived* parent = nullptr;
	Value data;
	static drived* MinimumNode(drived* basenode)
	{
		if (basenode->left == basenode)return basenode;
		while (basenode->left != nullptr)
		{
			basenode = basenode->left;
		}
		return basenode;
	}
	static drived* MaximumNode(drived* basenode)
	{
		if (basenode->left == basenode)return basenode;
		while (basenode->right != nullptr)
		{
			basenode = basenode->right;
		}
		return basenode;
	}
};

template<class Value, class NodeType>
class _SearchTreeIteratorBase :public Iterator<bidirectional_iterator_tag, Value, ptrdiff_t,Value*,Value&>
{
private:
	typedef _SearchTreeIteratorBase<Value, NodeType> self;
public:
	typedef bidirectional_iterator_tag iterator_category;
	typedef ptrdiff_t difference_type;
	typedef NodeType* node_ptr;
	typedef Value value_type;

	node_ptr node;
protected:
	//�������������Ҫ������������������Ҫô�Ƿ��ʹ��ģ�Ҫô�ǿյ�
	void BaseIncrement()
	{
		//�ж��Լ���û��������������У�˵����һ�ε����Լ���Ϊ�м�ڵ㱻����
		if (node->right != nullptr) //��һ�����м�ڵ�,��ô�Ͷ������������������
		{
			node = (node_ptr)NodeType::MinimumNode(node->right);
		}
		else //��һ������Ϊ���ҽڵ���ʣ���һ��Ӧ���ҵ������ҽڵ�Ľڵ�
		{
			node_ptr parent = (node_ptr)node->parent;
			while (node==parent->right)	//End�ڵ�������ԣ�End�ڵ��Parentָ��Root�ڵ㣬Root�ڵ��Parentָ��End�ڵ�
			{
				node = parent;
				parent = parent->parent;
			}
			if (node->right != parent)
				node = parent;
		}
	}
	void BaseDecrement()
	{
		//��Increment�����Գ�
		if (node->left != nullptr) 
		{
			node = (node_ptr)NodeType::MaximumNode(node->left);
		}
		else 
		{
			node_ptr parent = (node_ptr)node->parent;
			while (node == parent->left)
			{
				node = parent;
				parent = parent->parent;
			}
			node = parent;
		}
	}
public:
	_SearchTreeIteratorBase(node_ptr n) :node(n){}
	_SearchTreeIteratorBase():node(nullptr){}
};

//��ʵ��Search Treeֻ�ں�Value������֪��Key�����ͺ�ֵ�Ǳ�Ҫ��
template < class Key, class Value, class KeyOfValue
	, class Compare
	, class NodeType
	, class IteratorType = _SearchTreeIteratorBase<Value, NodeType>
	, class Alloc = ALLOC(NodeType)>
class _SearchTreeBase
{
public: //������ͨ��
	typedef Key key_type;
	typedef Value value_type;
	typedef value_type* pointer;
	typedef value_type& reference;
	typedef const value_type* const_pointer;
	typedef const value_type& const_reference;

	typedef size_t size_type;
	typedef ptrdiff_t difference_type;

	typedef IteratorType iterator;
	typedef Alloc allocator_type;

	typedef NodeType node_type;
	typedef node_type* link_type;
private:
	link_type header;

protected:
	allocator_type alloc;

	link_type GetNode(){ return alloc.Allocate();  }
	void PutNode(link_type node){ alloc.Deallocate((link_type)node, 1); }
	link_type CreateNode(const_reference x)
	{
		link_type node = GetNode();
		_construct(&(node->data), x);
		return node;
	}
	void DestroyNode(link_type node)
	{
		_destroy(&(node->data));
		PutNode(node);
	}

	Compare key_compare;

	link_type& Header()const { return (link_type&)header; }
	link_type& Root()const { return (link_type&)(Header()->parent); }
	link_type& Leftmost()const { return (link_type&)Header()->left; }
	link_type& Rightmost()const { return (link_type&)Header()->right; }

	static link_type& LeftChild(link_type x){ return (link_type&)x->left; }
	static link_type& RightChild(link_type x){ return (link_type&)x->right; }
	static link_type& Parent(link_type x){ return (link_type&)x->parent; }
	static reference Data(link_type x){ return (link_type&)x->data; }

	static link_type MinimumNode(link_type x){ return (link_type)NodeType::MinimumNode(x); }
	static link_type MaximumNode(link_type x){ return (link_type)NodeType::MaximumNode(x); }

	static key_type& NodeKey(link_type x){ return (key_type&)(KeyOfValue()(x->data)); }

	//ͨ��Key��Node��ָ�룬û�ҵ��򷵻�nullptr,�������Ƿ��ҵ���father���᷵��ֵ������father������������һ��һ���ǿյģ��Ա��ڲ�����ֵ
	link_type FindNode(const key_type& key, link_type& father)	const
	{
		link_type step = Root();
		father = Header();
		if (step == nullptr)return nullptr;

		while (step != nullptr)
		{
			const key_type& cur_key = NodeKey(step);
			father = step;
			if (cur_key == key)
			{
				return step;
			}
			if (key_compare(key, cur_key))
			{
				step = RightChild(step);
			}
			else
			{
				step = LeftChild(step);
			}
		}
		return nullptr;
	}
	link_type FindNode(const key_type&  key)const
	{
		link_type tmp;
		return FindNode(key, tmp);
	}

public:
	_SearchTreeBase()
	{
		Header() = GetNode();

		Root() = nullptr;
		Leftmost() = Header();
		Rightmost() = Header();
	}

	Compare KeyComp()const{ return key_compare; }
	iterator Begin()const{ return iterator(Leftmost()); }
	iterator begin()const{ return iterator(Leftmost()); }
	iterator End()const{ return iterator(Header()); }
	iterator end()const{ return iterator(Header()); }
};

SSTL_END_NAMESPACE

#endif
