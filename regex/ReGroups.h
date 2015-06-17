#pragma once
#ifndef RE_CAPTUREDGROUP_H
#define RE_CAPTUREDGROUP_H

#include"config.h"
#include"ReSyntaxExprNode.h"

#include"../stl/svector.h"

RE_BEGIN_INTERNAL_NAMESPACE

class ReGroups
{
private:
	static void GenReGroupsAux(ReCapturedGroupExprNode* _RootCaptured, ReGroups* _Root, ReGroups* _Current);
	static void CopyReGroupsAux(ReGroups* _Root, ReGroups* _Des, const ReGroups* _Src);
	
protected:
	friend class ReGenEpsilonNFAVistor;
public:
	String_t name;
	sstl::Vector<String_t> result;
	sstl::Vector<Ptr<ReGroups>> childCapturedGroups;
	sstl::Map<String_t, ReGroups*> map;
	void Clear();	//清除捕获到的字符串，但是ReGroup的拓扑结构不变
	ReGroups();
	~ReGroups();
	static ReGroups* GenReGroups(ReCapturedGroupExprNode* _RootCaptured);
	static void CopyReGroups(ReGroups* _Des, const ReGroups* _Src);
	ReGroups* operator[](int index)const;
	const ReGroups* FindByName(String_t _Index)const;

	ReGroups(const ReGroups& groups)
	{
		CopyReGroups(this, &groups);
	}

	ReGroups& operator=(const ReGroups& groups)
	{
		if (this == &groups)return *this;
		CopyReGroups(this, &groups);
		return *this;
	}
};

RE_END_INTERNAL_NAMESPACE

#endif
