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
	//中序遍历，不需要考虑左子树，左子树要么是访问过的，要么是空的
	void BaseIncrement()
	{
		//判断自己有没有右子树，如果有，说明上一次迭代自己作为中间节点被访问
		if (node->right != nullptr) //上一次是中间节点,那么就对右子树进行中序遍历
		{
			node = (node_ptr)NodeType::MinimumNode(node->right);
		}
		else //上一次是作为最右节点访问，下一个应该找到不是右节点的节点
		{
			node_ptr parent = (node_ptr)node->parent;
			while (node==parent->right)	//End节点的特殊性：End节点的Parent指向Root节点，Root节点的Parent指向End节点
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
		//与Increment操作对称
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

//事实上Search Tree只在乎Value，但是知道Key的类型和值是必要的
template < class Key, class Value, class KeyOfValue
	, class Compare
	, class NodeType
	, class IteratorType = _SearchTreeIteratorBase<Value, NodeType>
	, class Alloc = ALLOC(NodeType)>
class _SearchTreeBase
{
public: //以下是通用
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

	//通过Key找Node的指针，没找到则返回nullptr,但无论是否找到，father都会返回值，而且father的左右子树有一个一定是空的，以便于插入新值
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
