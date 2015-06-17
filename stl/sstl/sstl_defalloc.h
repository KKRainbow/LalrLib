#ifndef SSTL_DEFALLOC_H
#define SSTL_DEFALLOC_H
#include"sstl_config.h"
#include"sstl_alloc.h"

SSTL_BEGIN_NAMESPACE

template<class T>
class ElementaryAllocator : public Allocator<T>
{
	
};

SSTL_END_NAMESPACE

#endif