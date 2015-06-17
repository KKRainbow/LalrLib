#ifndef SSTL_ALGOBASE_H
#define SSTL_ALGOBASE_H
#include"sstl_config.h"
#include"sstl_iterator.h"
#include"string.h"

SSTL_BEGIN_NAMESPACE

template<class T>
T max(T a, T b)
{
	return a > b ? a : b;
}

template<class T, class... Args>
T max(T a, T b, Args... rest)
{
	return a > b ? max(a, rest...) : max(b, rest...);
}

template<class T>
T min(T a, T b)
{
	return a < b ? a : b;
}

template<class T, class... Args>
T min(T a, T b, Args... rest)
{
	return a < b ? max(a, rest...) : max(b, rest...);
}

//迭代器复制(指针间distance为判据（Random Iterator)
template<class RandomIterator,class OutputInterator>
inline OutputInterator __copy_iter(RandomIterator first, RandomIterator last, OutputInterator result, random_access_iterator_tag)
{
	auto dis = last - first;
	while (dis--)
	{
		*result++ = *first++;
	}
	return result;
}
//迭代器复制以指针是否相等判据（Input Iterator)
template<class InputIterator, class OutputIterator>
inline OutputIterator __copy_iter(InputIterator first, InputIterator last, OutputIterator result, input_iterator_tag)
{
	auto dis = last - first;
	for (; first != last;)
	{
		*result++ = *first++;
	}
	return result;
}
//指针类型的复制 //是Trivial oper= template<class T>
template<class T>
inline T* __copy_ptr(const T* first, const T* last, T* result, TrueType)
{
	memmove(result, first, sizeof(T)*(last - first));
	return result + (last - first);
}
//指针类型的复制//不是Trivial oper=
template<class T>
inline T* __copy_ptr(const T* first, const T* last, T* result, FalseType)
{
	return __copy_iter(first, last, result,_iterator_category(first));
}

template<class InputIterator,class OutputIterator>
struct __copy_dispatch
{
	OutputIterator operator()(InputIterator first, InputIterator last, OutputIterator result)
	{
		return __copy_iter(first, last, result, _iterator_category(first));
	}
};

template<class T>
struct __copy_dispatch<T*,T*>
{
	T* operator()(T* first, T* last, T* result)
	{
		typedef typename TypeTraits<T>::has_trival_assignment_operator t;
		return __copy_ptr<T>(first, last, result, t());
	}
};

template<class T>
struct __copy_dispatch<const T*, T*>
{
	T* operator()(const T* first, const T* last, T* result)
	{
		typedef typename TypeTraits<T>::has_trival_assignment_operator t;
		return __copy_ptr<T>(first, last, result, t());
	}
};

template<class InputIterator, class OutputIterator>
inline OutputIterator copy(InputIterator first, InputIterator last, OutputIterator result)
{
	return __copy_dispatch<InputIterator, OutputIterator>()(first, last, result);
}

//Swap函数，交换两个变量
template<class InputIterator>
void swap(InputIterator a, InputIterator b)
{
	typedef typename iterator_traits<InputIterator>::value_type value_type;
	value_type tmp;
	tmp = *a;
	*a = *b;
	*b = tmp;
}

template<class ForwardIteraotr,class T>
inline ForwardIteraotr findn(ForwardIteraotr first,ForwardIteraotr last,const T& val,int n) 
{
	if(n == 0)return first;
	ForwardIteraotr step = last;
	int count = 0;
	for(;first!=last;first++)
	{
		if(*first == val)
		{
			if(count == 0)
			{
				step = first;
			}
			++count;
			if(count == n)
			{
				return step;
			}
		}
		else
		{
			count = 0;
		}
	}
	return last;
}

template<class ForwardIteraotr,class T>
inline ForwardIteraotr find(ForwardIteraotr first,ForwardIteraotr last,const T& val) 
{
	return findn(first,last,val,1);
}

SSTL_END_NAMESPACE
#endif
