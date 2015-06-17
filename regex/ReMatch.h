#pragma once
#include"config.h"
#include"ReGroups.h"
#include"../stl/svector.h"


RE_BEGIN_NAMESPACE

class ReMatch
{
	friend class Regex;
	sstl::Vector<inner::ReGroups> groups;
public:
	int Size()const
	{
		return groups.Size();
	}

	const inner::ReGroups& GetGroup(int _Index) const
	{
		return groups[_Index];
	}

	ReMatch();
	~ReMatch();
};


RE_END_NAMESPACE
