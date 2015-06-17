#pragma once
#include"sstl_config.h"

SSTL_BEGIN_NAMESPACE

template<class T>
class IDGenerator
{
	private:
		T counter;
		T begin;

	public:
		IDGenerator(T _Begin = 0)
		{
			begin = counter = _Begin;
		}
		T GetID()
		{
			return counter++;
		}
		T Reset()
		{
			counter = begin; 
		}
};

SSTL_END_NAMESPACE
