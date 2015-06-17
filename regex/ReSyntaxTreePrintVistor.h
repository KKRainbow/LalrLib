#pragma once
#ifndef RE_PRINT_H
#define RE_PRINT_H

#include"config.h"
#include"ReSyntaxExprNode.h"
#include<iostream>
using namespace std;


RE_BEGIN_INTERNAL_NAMESPACE

class ReSyntaxTreePrintVistor :public ReSyntaxExprNodeVistor
{
public:
	int GetDepth(ReSyntaxExprNode* _Node)
	{
		int n = 0;
		while (_Node->Parent() != nullptr)
		{
			_Node = _Node->Parent();
			n++;
		}
		return n;
	}
	virtual void VisitReOrExprNode(ReOrExprNode* _Node)
	{
		cout << "Or节点" <<"\t"<<GetDepth(_Node) <<endl;
	}		//Or Node
	virtual void VisitReConcatExprNode(ReConcatExprNode* _Node)
	{
		cout << "Cat节点" <<"\t"<<GetDepth(_Node) <<endl;
	}		//Cat Node
	virtual void VisitReCharExprNode(ReCharExprNode* _Node)
	{
		wcout << "Char节点" <<"\t"<<_Node->c<<"\t"<<GetDepth(_Node) <<endl;
	}		//Char Node
	virtual void VisitReCapturedGroupExprNode(ReCapturedGroupExprNode* _Node)
	{
		cout << "Capture节点" <<"\t"<<GetDepth(_Node) <<endl;
	}		//Captured group
	virtual void VisitReBackTracedGroupExprNode(ReBackTracedGroupExprNode* _Node)
	{
		cout << "Backtrace节点" <<"\t"<<GetDepth(_Node) <<endl;
	}		//BackTraced goup
	virtual void VisitReSelectionExprNode(ReSelectionExprNode* _Node)
	{
		cout << "Select节点" <<"\t"<<GetDepth(_Node) <<endl;
	}
	virtual void VisitReLookAroundExprNode(ReLookAroundExprNode* _Node)
	{
		cout << "LookAround节点" <<"\t"<<GetDepth(_Node) <<endl;
	}
	virtual void VisitReBackReferenceGroupExprNode(ReBackReferenceGroupExprNode* _Node)
	{
		cout << "BackReference节点" << "\t" << GetDepth(_Node) << endl;
	}
};



RE_END_INTERNAL_NAMESPACE

#endif