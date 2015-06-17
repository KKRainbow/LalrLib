#pragma once
#ifndef RE_EPSILONNFA_H
#define RE_EPSILONNFA_H

#include"config.h"
#include"ReSyntaxExprNode.h"
#include"ReNFA.h"


RE_BEGIN_INTERNAL_NAMESPACE

class ReGenEpsilonNFAVistor :public ReSyntaxExprNodeVistor
{
private:
	ReNFA& nfa;
public:
	typedef ReNFA::StateNum StateNum;
	ReGenEpsilonNFAVistor(ReNFA& _NFA) :nfa(_NFA){}
	~ReGenEpsilonNFAVistor(){}

	virtual void VisitReOrExprNode(ReOrExprNode* _Node);		//Or Node
	virtual void VisitReConcatExprNode(ReConcatExprNode* _Node);		//Cat Node
	virtual void VisitReCharExprNode(ReCharExprNode* _Node);		//Char Node
	virtual void VisitReCapturedGroupExprNode(ReCapturedGroupExprNode* _Node);		//Captured group
	virtual void VisitReBackTracedGroupExprNode(ReBackTracedGroupExprNode* _Node);		//BackTraced goup
	virtual void VisitReSelectionExprNode(ReSelectionExprNode* _Node);
	virtual void VisitReLookAroundExprNode(ReLookAroundExprNode* _Node);
	virtual void VisitReBackReferenceGroupExprNode(ReBackReferenceGroupExprNode* _Node);

	void ConstructQulifiedRe(ReSyntaxExprNode* _Node);
};



RE_END_INTERNAL_NAMESPACE

#endif
