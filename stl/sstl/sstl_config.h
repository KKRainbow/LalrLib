#ifndef SSTL_CONFIG_H
#define SSTL_CONFIG_H
#include"../../basic/basic.h"

#define SSTL_USE_BASIC_ALLOC
#define SSTL_USE_INITIALIZER_LIST

#ifdef SSTL_USE_BASIC_ALLOC
#	define ALLOC(T) Allocator<T>
#else
#	define ALLOC Allocator
#endif

#ifdef __LLIB_USE_NAMESPACE
#	define SSTL_NAMESPACE				sstl
#	define SSTL_BEGIN_NAMESPACE			__LLIB_BEGIN_NAMESPACE namespace SSTL_NAMESPACE{
#	define SSTL_END_NAMESPACE			}}
#else
#	define SSTL_NAMESPACE
#	define SSTL_BEGIN_NAMESPACE
#	define SSTL_END_NAMESPACE
#endif


#endif
