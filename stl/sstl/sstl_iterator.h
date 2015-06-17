#ifndef SSTL_ITERATOR_H
#define SSTL_ITERATOR_H
#include"sstl_config.h"
#include<stddef.h>

SSTL_BEGIN_NAMESPACE

class input_iterator_tag{};
class output_iterator_tag{};
class forward_iterator_tag :public input_iterator_tag, public output_iterator_tag{};
class bidirectional_iterator_tag :public forward_iterator_tag{};
class random_access_iterator_tag : public bidirectional_iterator_tag{};

template<class Category,class T,class Distance=ptrdiff_t,class Pointer=T*,class Reference=T&>
struct Iterator
{
	typedef Category iterator_category;
	typedef T value_type;
	typedef Distance distance;
	typedef Pointer pointer;
	typedef Reference reference;
};

template<class Iterator>
struct iterator_traits
{
	typedef typename Iterator::iterator_category iterator_category;
	typedef typename Iterator::value_type value_type;
	typedef typename Iterator::distance difference_type;
	typedef typename Iterator::pointer pointer;
	typedef typename Iterator::reference reference;
};

template<class T>
struct iterator_traits<T *>
{
	typedef random_access_iterator_tag iterator_category;
	typedef T value_type;
	typedef ptrdiff_t difference_type;
	typedef T* pointer;
	typedef T& reference;
};

template<class T>
struct iterator_traits<const T *>
{
	typedef random_access_iterator_tag iterator_category;
	typedef T value_type;
	typedef ptrdiff_t difference_type;
	typedef const T* pointer;
	typedef const T& reference;
};

template<class Iterator>
static inline typename iterator_traits<Iterator>::iterator_category _iterator_category(const Iterator&)
{
	return typename iterator_traits<Iterator>::iterator_category();
}

template<class InputIterator>  //ÎªÊ²Ã´ÊÇInputIter
static inline typename iterator_traits<InputIterator>::difference_type _distance_aux(InputIterator first, InputIterator last, input_iterator_tag)
{
	typename iterator_traits<InputIterator>::difference_type n = 0;
	while (first != last)
	{
		first++;
		n++;
	}
	return n;
}

template<class RandomIterator>  
static inline typename iterator_traits<RandomIterator>::difference_type _distance_aux(RandomIterator first, RandomIterator last, random_access_iterator_tag)
{
	return last - first;
}

template<class Iterator> 
static inline typename iterator_traits<Iterator>::difference_type _distance(Iterator first, Iterator last)
{
	return _distance_aux(first, last, _iterator_category(first));
}


template<class InputIterator, class Distance>
static inline void _advance_aux(InputIterator& i, Distance n, input_iterator_tag)
{
	while (n--)i++;
}


template<class BidirectionalIterator, class Distance>
static inline void _advance_aux(BidirectionalIterator& i, Distance n, bidirectional_iterator_tag)
{
	if (n >= 0)
	{
		while (n--)i++;
	}
	else if (n < 0)
	{
		while (n++)i--;
	}
	
}

template<class RandomIterator, class Distance>
static inline void _advance_aux(RandomIterator& i, Distance n, random_access_iterator_tag)
{
	i += n;
}

template<class InputIterator, class Distance>
static inline void _advance(InputIterator& i, Distance n)
{
	_advance_aux(i, n, _iterator_category(i));
}

//用于在多个迭代器所指至进行最大值运算时，可以返回值最大的迭代器而不是最大的值
template<class Iterator>
class IterCmp
{
public:
	Iterator iter;
	IterCmp(Iterator i) :iter(i){}
	bool operator<(const IterCmp& i)const{ return *iter < *i.iter; }
	bool operator<=(const IterCmp& i)const{ return *iter <= *i.iter; }
	bool operator>(const IterCmp& i)const{ return *iter > *i.iter; }
	bool operator>=(const IterCmp& i)const{ return *iter >= *i.iter; }
	bool operator==(const IterCmp& i)const{ return *iter == *i.iter; }
	bool operator!=(const IterCmp& i)const{ return *iter != *i.iter; }
};


SSTL_END_NAMESPACE
#endif