#include "ReGroups.h"
#include<iostream>
RE_BEGIN_INTERNAL_NAMESPACE

ReGroups::ReGroups()
{
	result.Reserve(10);
}


ReGroups::~ReGroups()
{
}

void ReGroups::GenReGroupsAux(ReCapturedGroupExprNode* _Captured, ReGroups* _Root, ReGroups* _Current)
{
	_Captured->group = _Current;

	if (_Captured->name.Length() != 0)
	{
		_Root->map.Insert(sstl::MakePair(_Captured->name, _Current));
	}

	_Current->name = _Captured->name;

	for (auto& n : _Captured->childCaptured)
	{
		ReGroups* child = new ReGroups();
		GenReGroupsAux(n, _Root, child);
		_Current->childCapturedGroups.PushBack(child);
	}
}

ReGroups* ReGroups::GenReGroups(ReCapturedGroupExprNode* _RootCaptured)
{
	ReGroups* res = new ReGroups();
	GenReGroupsAux(_RootCaptured,res, res);
	return res;
}

ReGroups* ReGroups::operator[](int index)const
{
	return childCapturedGroups[index].Obj();
}

const ReGroups* ReGroups::FindByName(String_t _Index)const
{
	auto res = map.Find(_Index);
	return res == map.End()?nullptr:res->second;
}

void ReGroups::CopyReGroups(ReGroups* _Des, const ReGroups* _Src)
{
	_Des->map.Clear();
	_Des->childCapturedGroups.Clear();
	
	CopyReGroupsAux(_Des, _Des, _Src);
}

void ReGroups::CopyReGroupsAux(ReGroups* _Root, ReGroups* _Des, const ReGroups* _Src)
{
	_Des->name = _Src->name;
	_Des->result = _Src->result;
	if (_Des->name.Length() != 0)
	{
		_Root->map.Insert(sstl::MakePair(_Des->name, _Des));
	}

	for (auto& group : _Src->childCapturedGroups)
	{
		ReGroups* newgroup = new ReGroups();
		_Des->childCapturedGroups.PushBack(newgroup);
		CopyReGroupsAux(_Root, newgroup, group.Obj());
	}
}

void ReGroups::Clear()	//清除捕获到的字符串，但是ReGroup的拓扑结构不变
{
	result.Clear();
	for (auto& groupPtr : childCapturedGroups)
	{
		groupPtr->Clear();
	}
}

RE_END_INTERNAL_NAMESPACE
