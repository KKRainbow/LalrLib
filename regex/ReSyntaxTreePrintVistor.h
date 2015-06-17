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
		cout << "Or�ڵ�" <<"\t"<<GetDepth(_Node) <<endl;
	}		//Or Node
	virtual void VisitReConcatExprNode(ReConcatExprNode* _Node)
	{
		cout << "Cat�ڵ�" <<"\t"<<GetDepth(_Node) <<endl;
	}		//Cat Node
	virtual void VisitReCharExprNode(ReCharExprNode* _Node)
	{
		wcout << "Char�ڵ�" <<"\t"<<_Node->c<<"\t"<<GetDepth(_Node) <<endl;
	}		//Char Node
	virtual void VisitReCapturedGroupExprNode(ReCapturedGroupExprNode* _Node)
	{
		cout << "Capture�ڵ�" <<"\t"<<GetDepth(_Node) <<endl;
	}		//Captured group
	virtual void VisitReBackTracedGroupExprNode(ReBackTracedGroupExprNode* _Node)
	{
		cout << "Backtrace�ڵ�" <<"\t"<<GetDepth(_Node) <<endl;
	}		//BackTraced goup
	virtual void VisitReSelectionExprNode(ReSelectionExprNode* _Node)
	{
		cout << "Select�ڵ�" <<"\t"<<GetDepth(_Node) <<endl;
	}
	virtual void VisitReLookAroundExprNode(ReLookAroundExprNode* _Node)
	{
		cout << "LookAround�ڵ�" <<"\t"<<GetDepth(_Node) <<endl;
	}
	virtual void VisitReBackReferenceGroupExprNode(ReBackReferenceGroupExprNode* _Node)
	{
		cout << "BackReference�ڵ�" << "\t" << GetDepth(_Node) << endl;
	}
};



RE_END_INTERNAL_NAMESPACE

#endif