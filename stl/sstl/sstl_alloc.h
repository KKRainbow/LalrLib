#ifndef SSTL_ALLOC_H
#define SSTL_ALLOC_H
#include"sstl_config.h"
#include"sstl_construct.h"
#include<limits.h>
SSTL_BEGIN_NAMESPACE
template<class T>
class Allocator
{
public:
	typedef T value_type;
	typedef T* pointer;
	typedef const T* const_pointer;
	typedef T& reference;
	typedef const T& const_reference;
	typedef size_t size_type;
	typedef long difference_type;

	static pointer Address(reference x) 
	{
		return static_cast<pointer>(x);
	}
	static const_pointer Address(const_reference x) 
	{
		return static_cast<const_pointer>(x);
	}
	static pointer Allocate(size_type n = 1, const void* hint = 0)
	{
		return _allocate((difference_type)n, (pointer)hint);
	}
	static void Deallocate(pointer p, size_type n) 
	{
		_deallocate(p);
	}
	static size_type Max_size() 
	{
		return size_type(UINT_MAX/sizeof(T)); //²»Ì«Àí½â°¡QAQ
	}
	static void Construct(pointer p, const T& x) 
	{
		_construct(p, x);
	}
	template<class ForwardIter>
	static inline void Construct(ForwardIter first, ForwardIter last,const T& x)
	{
		while (first != last)
		{
			_construct(&*first, x);
			++first;
		}
	}
	static void Destroy(pointer p) 
	{
		_destroy(p);
	}
	template<class ForwardIter>
	static inline void Destroy(ForwardIter first, ForwardIter last)
	{
		_destroy(first, last);
	}
};

SSTL_END_NAMESPACE

#endif
