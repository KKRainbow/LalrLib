//∂—À„∑®
#ifndef SSTL_SETALGO_H
#define SSTL_SETALGO_H
#include"sstl_config.h"
#include"sstl_iterator.h"
#include"sstl_functors.h"

SSTL_BEGIN_NAMESPACE


template<class InputIterator1,class InputIterator2,class OutputIterator,class Compare>
OutputIterator SetUnion(InputIterator1 first1, InputIterator1 last1
	, InputIterator2 first2, InputIterator2 last2
	, OutputIterator result)
{
	while (*first1 != *last1&&*first2 != *last2)
	{
		if (*first1 < *first2)
		{

		}
	}
}

SSTL_END_NAMESPACE


#endif