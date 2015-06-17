#ifndef SSTL_QUEUE_H
#define SSTL_QUEUE_H
#include"sstl/sstl_deque.h"
#include"sstl/sstl_heap.h"

SSTL_BEGIN_NAMESPACE

template<class T, class Sequence = Deque<T>>
class Queue :public Object
{
	__LLIB_CLASS_DECLARE(Queue, Object);
public:
	typedef T value_type;
	typedef Sequence container_type;
	typedef size_t size_type;
protected:
	Sequence container;
public:
	void Pop(){ container.PopFront(); }
	void Push(const value_type& x){ container.PushBack(x); }
	const value_type& Top(){ return container.Front(); }
	size_type Size(){ return container.Size(); }
	bool Empty(){ return container.Empty(); }
	Queue(container_type container = container_type()) :container(container)
	{}
};

template <class T, class Sequence = Vector<T>,
class Compare = Greater<typename Sequence::value_type> >
class PriorityQueue :public Object
{
	__LLIB_CLASS_DECLARE(PriorityQueue, Object);
public:
	typedef T value_type;
	typedef Sequence container_type;
	typedef size_t size_type;
	typedef _Heap<typename container_type::iterator, Compare> algo;
protected:
	Sequence container;
public:
	void Pop()
	{
		algo::ExtractTop(container.Begin(), container.End());
		container.PopBack();
	}
	void Push(const value_type& x)
	{
		container.PushBack(x);
		algo::ModifyKey(container.Begin(), container.End(), container.End() - 1, x, AlwaysTrue<value_type>());
	}
	const value_type& Top()
	{ 
		return container.Front(); 
	}
	size_type Size(){ return container.Size(); }
	bool Empty(){ return container.Empty(); }
	PriorityQueue(container_type container = container_type()) :container(container)
	{}
};

SSTL_END_NAMESPACE


#endif