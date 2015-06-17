#ifndef SSTL_CONSTRUCTOR_H
#define SSTL_CONSTRUCTOR_H
#include"sstl_config.h"
#include"stddef.h"
#include"sstl_iterator.h"

extern void* operator new(unsigned int size);
extern void* operator new(unsigned int size,void* p);
extern void* operator new[](unsigned int size);
extern void* operator new[](unsigned int size,void* p);
extern void operator delete(void* p) throw();

SSTL_BEGIN_NAMESPACE

/////内存基本分配工具/////////////////////////////////////////////////////
template<class T>
static inline void _destroy(T* pointer);
template<class T>
static inline T* _allocate(ptrdiff_t size,T* ptr)
{
	T* tmp = (T*)(::operator new((size_t)(size*sizeof(T))));
	__LLIB_ASSERT(tmp != nullptr);
	return tmp;
}
template<class T>
static inline void _deallocate(T* buffer)
{
	::operator delete(buffer);
}

template<class T,class V>
static inline void _construct(T* p, const V& value)
{
	new (p)T(value);
}
template<class ForwardIter>
static inline void __destroy_aux(ForwardIter first, ForwardIter last, FalseType)
{
	for (; first != last; first++)
		_destroy(&*first);
}
template<class ForwardIter>
static inline void __destroy_aux(ForwardIter first, ForwardIter last, TrueType)
{}

template<class T>
static inline void __destroy_aux(T* pointer,TrueType)
{}
template<class T>
static inline void __destroy_aux(T* pointer,FalseType)
{
	pointer->~T();
}
template<class T>
static inline void _destroy(T* pointer)
{
	__destroy_aux(pointer, _type_tarits_has_trival_dtor(*pointer));
}

template<class ForwardIter>
static inline void _destroy(ForwardIter first, ForwardIter last)
{
	__destroy_aux(first, last, _type_tarits_has_trival_dtor(*first));
}


//////////////////////////////////////////////////////////////////////////




/////内存基本处理工具/////////////////////////////////////////////////////
template<class InputIterator,class ForwardIterator>
static inline ForwardIterator _uninitialized_copy(InputIterator first, InputIterator last,
	ForwardIterator result)
{
	ForwardIterator tmp = result;

	__LLIB_TRY
	{
		for (; first != last; ++result, ++first)
		{
			_construct(&*result, *first);
		}
	}
	__LLIB_CATCH_ALL
	{
		_destroy(tmp, result);
		__LLIB_RETHROW;
	}

	return result;
}

template <class ForwardIterator, class T>
static inline void _uninitialized_fill(ForwardIterator first, ForwardIterator last,
	const T& x)
{
	ForwardIterator tmp = first;
	__LLIB_TRY
	{
		for (; first != last; ++first)
		{
			_construct(&*first, x);
		}
	}
	__LLIB_CATCH_ALL
	{
		_destroy(tmp, first);
		__LLIB_RETHROW;
	}
}

template <class ForwardIterator, class Size, class T>
ForwardIterator _uninitialized_fill_n(ForwardIterator first, Size n, const T& x)
{
	ForwardIterator tmp = first;

	__LLIB_TRY
	{
		for (; n--; ++first)
		{
			_construct(&*first, x);
		}
	}
	__LLIB_CATCH_ALL
	{
		_destroy(tmp, first);
		__LLIB_RETHROW;
	}

	return first;
}


//////////////////////////////////////////////////////////////////////////

SSTL_END_NAMESPACE

#endif
