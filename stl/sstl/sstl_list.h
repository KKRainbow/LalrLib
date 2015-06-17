#ifndef SSTL_LIST_H
#define SSTL_LIST_H

#include"sstl_config.h"
#include"sstl_iterator.h"
#include"sstl_alloc.h"
#include"sstl_algobase.h"
#include<stddef.h>

SSTL_BEGIN_NAMESPACE

template<class T>
class __ListNode
{
public:
	typedef __ListNode<T>* node_pointer;
	node_pointer prev;
	node_pointer next;
	T data;
};

template<class T,class Ref = T&,class Ptr = T*>
class __ListIterator :public Iterator<bidirectional_iterator_tag, T, ptrdiff_t, Ptr, Ref>
{
	private:
		typedef Iterator<bidirectional_iterator_tag, T, ptrdiff_t, Ptr, Ref> BaseIterator; 
public:
	typedef __ListIterator<T> iterator;
	typedef __ListIterator<T, Ref, Ptr> self;
	typedef __ListNode<T>* link_type;
	typedef size_t size_type;
	using typename  BaseIterator::reference;
	//using typename BaseIterator::const_reference;
	using typename BaseIterator::pointer;
	//using typename BaseIterator::const_pointer;
public:
	link_type node;
public:
	__ListIterator(){}
	__ListIterator(link_type x) :node(x){}
	__ListIterator(const self& x) = default;// :node(x.node){}

	//Override operator
	self& operator =(const self& x)
	{
		if (this == &x)return *this;	//防止自己给自己赋值
		node = x.node; return *this; 
	}

	bool operator ==(const self& x)const{ return node == x.node; }
	bool operator !=(const self& x)const{ return !(node == x.node); }

	reference operator*()const { return node->data; }
	pointer operator->()const{ return &(node->data); }

	self& operator++() //前缀
	{
		node = static_cast<link_type>(node->next);
		return *this;
	}
	self operator++(int)
	{
		self tmp = *this;
		++*this;
		return tmp;
	}

	self& operator --() //前缀
	{
		node = static_cast<link_type>(node->prev);
		return *this;
	}
	self operator --(int)
	{
		self tmp = *this;
		--*this;
		return tmp;
	}
};

template<class T, class Alloc = ALLOC(__ListNode<T >)>
class List :public Object
{
	__LLIB_CLASS_DECLARE(List, Object);
public:
	typedef T value_type;
	typedef value_type* pointer;
	typedef value_type& reference;
	typedef const T* const_pointer;
	typedef const T& const_reference;

	typedef size_t size_type;
	typedef ptrdiff_t difference_type;

	typedef __ListIterator<T,reference,pointer> iterator;
	typedef Alloc allocator_type;
protected:
	typedef __ListNode<T> list_node;
	typedef list_node* link_type;

protected:
	allocator_type alloc;
	link_type empty_node;
protected:
	link_type GetNode(){ return alloc.Allocate(); }
	void PutNode(link_type node){ alloc.Deallocate(node,1); }

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
	void Transfer(iterator position, iterator first, iterator last);
protected:
	void Initialize()
	{
		empty_node = GetNode();
		empty_node->next = empty_node;
		empty_node->prev = empty_node;
	}
public://构造函数
	explicit List(const allocator_type& mAlloc = allocator_type()) :alloc(mAlloc)
	{
		Initialize();
	}
	explicit List(size_type n, const value_type& val = value_type()
		, const allocator_type& mAlloc = allocator_type()) :List(mAlloc)
	{
		Insert(Begin(), n, val);
	}
	template <class InputIterator>
	List(InputIterator first, InputIterator last,
		const allocator_type& mAlloc = allocator_type()) : List(mAlloc)
	{
		Insert(Begin(), first, last);
	}
	List(const List<T>& x) :List(x.alloc)
	{
		Insert(iterator(empty_node), iterator(x.empty_node->next), iterator(x.empty_node));
	}
	~List()
	{
		Clear();
		PutNode(empty_node);
	}
public:
	List& operator = (List& x)
	{
		if (this == &x)return *this;	//防止自己给自己赋值
		Clear();
		Insert(iterator(empty_node), iterator(x.empty_node->next), iterator(x.empty_node));
		return *this;
	}
public:
	iterator Begin(){ return iterator((*empty_node).next); }
	iterator End(){ return iterator(empty_node); }
	iterator begin(){ return iterator((*empty_node).next); }
	iterator end(){ return iterator(empty_node); }
	bool Empty()const{ return empty_node->next == empty_node; }
	size_type Size(){ return _distance(Begin(), End()); }

	reference Front(){ return *(Begin()); }
	reference Back(){ return *(End()); }

	iterator Insert(iterator position, const_reference x)
	{
		link_type tmp = CreateNode(x);
		tmp->next = position.node;
		tmp->prev = position.node->prev;

		position.node->prev->next = tmp;
		position.node->prev = tmp;

		return tmp;
	}
	void Insert(iterator position,size_type n, const_reference x)
	{
		while (n--)
		{
			Insert(position, x);
		}
	}
	template <class InputIterator>
	void Insert(iterator position, InputIterator first, InputIterator last)
	{
		for (; first != last; ++first)
		{
			Insert(position, *first);
		}
	}

	iterator Erase(iterator position)
	{
		iterator tmp = position.node->next;
		position.node->next->prev = position.node->prev;
		position.node->prev->next = position.node->next;

		DestroyNode(position.node);
		return tmp;
	}
	iterator Erase(iterator first, iterator last)
	{
		while (first != last)
		{
			first = Erase(first);
		}
		
		return last;
	}

	void PushFront(const_reference x){ Insert(Begin(), x); }
	void PushBack(const_reference x){ Insert(End(), x); }
	void PopFront(){ Erase(Begin()); }
	void PopBack(){ Erase(--End()); }
	
	void Clear(){ Erase(Begin(), End()); }
	void Remove(const_reference value)
	{
		iterator first = Begin(), last = End();
		while( first != last)
		{
			if (*first == value)
			{
				first = Erase(first);
			}
			else
			{
				++first;
			}
		}
	}
	void Unique()
	{
		iterator first = Begin(), last = End(),next;
		while (first != last&&first.node->next != last.node)
		{
			if (*first == first.node->next->data)
			{
				first = Erase(first);
			}
			else
			{
				++first;
			}
		}
	}
	
public:


	
};

//把first到last移到position之前
template<class T, class Alloc>
void List<T, Alloc>::Transfer(iterator position, iterator first, iterator last)
{
	if (position == last)return;

	last.node->prev->next = position.node;
	first.node->prev->next = last.node;
	position.node->prev->next = first.node;

	link_type tmp = position.node->prev;

	position.node->prev = last.node->prev;
	last.node->prev = first.node->prev;
	first.node->prev = tmp;
}


SSTL_END_NAMESPACE

#endif
