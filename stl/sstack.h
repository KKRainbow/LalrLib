#ifndef SSTL_STACK_H
#define SSTL_STACK_H
#include"sstl/sstl_deque.h"

SSTL_BEGIN_NAMESPACE

template<class T, class Sequence = Vector<T>>
class Stack:public Object
{
	__LLIB_CLASS_DECLARE(Stack, Object);
public:
	typedef T value_type;
	typedef Sequence container_type;
	typedef size_t size_type;
protected:
	Sequence container;
public:
	void Pop(){ container.PopBack(); }
	void Push(const value_type& x){ container.PushBack(x); }
	value_type& Top(){ return container.Back(); }
	size_type Size() { return container.Size(); }
	bool Empty(){ return container.Empty(); }
	Stack(container_type container = container_type()) :container(container)
	{}
	void Clear()
	{
		container.Clear();
	}
	void Reserve(const size_type n)
	{
		container.Reserve(n);
	}
};



SSTL_END_NAMESPACE


#endif