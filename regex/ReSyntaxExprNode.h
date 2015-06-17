#pragma once
#ifndef RE_SYNTAXEXPR_H
#define RE_SYNTAXEXPR_H
#include<stdio.h>
#include"config.h"
#include"ReTokenizer.h"
#include"../stl/smap.h"
#include"../stl/svector.h"


RE_BEGIN_INTERNAL_NAMESPACE

class ReGroups;
class ReSyntaxExprNodeVistor;

class ReSyntaxExprNode
{
public:
	typedef sstl::Map<int, ReSyntaxExprNode*> Map;
	static const int OR = 0;
	static const int CAT = 1;
	static const int CHAR = 2;
	static const int CAPTURE = 3;
	static const int SELECTION = 4;
	static const int LOOK = 5;
	static const int BACK = 6;
	static const int BACKREF = 7;
	virtual int NodeType() = 0;
protected:
	void Welcome(ReSyntaxExprNode* _Node, ReSyntaxExprNodeVistor* _Vistor);
	ReSyntaxExprNode* left = nullptr;
	ReSyntaxExprNode* right = nullptr;
	ReSyntaxExprNode* parent = nullptr;
public:
	ReSyntaxExprNode* AddLeftChild(ReSyntaxExprNode* _Node)
	{ 
		if(_Node!=nullptr)
			_Node->parent = this;
		this->left = _Node;
		return _Node; 
	}
	ReSyntaxExprNode* AddRightChild(ReSyntaxExprNode* _Node)
	{
		if (_Node != nullptr)
			_Node->parent = this;
		this->right = _Node;
		return _Node;
	}
	ReSyntaxExprNode*& Left() { return left; }
	ReSyntaxExprNode*& Right() { return right; }
	ReSyntaxExprNode*& Parent() { return parent; }
public://Quantifier Property
	static const unsigned int FINITE = 0xffffff;
	unsigned int from = 1;
	unsigned int to = 1;
	bool isGreedy = false;
	int FAStartNum = 0;
	int FATerminalNum = 0;
public:
	virtual void Accept(ReSyntaxExprNodeVistor* _Vistor)
	{
		if (left)left->Accept(_Vistor);
		if (right)right->Accept(_Vistor);
	}
	virtual void Destroy(){}
	
	ReSyntaxExprNode(){}
	virtual ~ReSyntaxExprNode()
	{
		if(left)delete left;
		if(right)delete right;
	}
};

class ReOrExprNode :public ReSyntaxExprNode					//Or Node
{
public:
public:
	virtual void Accept(ReSyntaxExprNodeVistor* _Vistor);
	virtual int NodeType(){return ReSyntaxExprNode::OR;}
};

class ReConcatExprNode :public ReSyntaxExprNode				//Cat Node
{
public:
public:
	virtual void Accept(ReSyntaxExprNodeVistor* _Vistor);
	virtual int NodeType(){return ReSyntaxExprNode::CAT;}
};

class ReCharExprNode :public ReSyntaxExprNode				//Char Node
{
public:
	bool isSpecial = false;
	Char_t c;
public:
	virtual void Accept(ReSyntaxExprNodeVistor* _Vistor);
	virtual int NodeType(){return ReSyntaxExprNode::CHAR;}
};

class ReCapturedGroupExprNode :public ReSyntaxExprNode		//Captured group
{
public:
	int num;
	String_t name;
	sstl::Vector<ReCapturedGroupExprNode*> childCaptured;
	String_t regularExpression;
	ReGroups* group = nullptr;
public:
	void AddCharsToName(ReCharExprNode* _Node)
	{ 
		if (_Node)
		{ 
			_Node->Parent() = this; 
			name += _Node->c;
		} 
	}
	virtual void Accept(ReSyntaxExprNodeVistor* _Vistor);
	virtual int NodeType(){return ReSyntaxExprNode::CAPTURE;}
};

class ReBackTracedGroupExprNode :public ReSyntaxExprNode	//BackTraced group
{
public:
	Map map;
	String_t name;
	unsigned int backTracedNum = 0;
	bool hasNum = false;
	ReCapturedGroupExprNode* correspondCapturedGroup = nullptr;
public:
	void AddCharsToName(ReCharExprNode* _Node){ if (_Node){ _Node->Parent() = this; name += _Node->c; } }
	virtual void Accept(ReSyntaxExprNodeVistor* _Vistor);
	virtual int NodeType(){return ReSyntaxExprNode::BACK;}
};

class ReBackReferenceGroupExprNode :public ReSyntaxExprNode	//BackReference group
{
public:
	Map map;
	String_t name;
	unsigned int backTracedNum = 0;
public:
	void AddCharsToName(ReCharExprNode* _Node){ if (_Node){ _Node->Parent() = this; name += _Node->c; } }
	virtual void Accept(ReSyntaxExprNodeVistor* _Vistor);
	virtual int NodeType(){ return ReSyntaxExprNode::BACKREF; }
};

class ReSelectionExprNode :public ReSyntaxExprNode			//Selection
{
public:
	bool isSelectNot;
	sstl::Vector<Ptr<ReCharExprNode>> chars;
	void AddChars(ReCharExprNode* _Node){ if (_Node)_Node->Parent() = this; chars.PushBack(_Node); }
	virtual void Accept(ReSyntaxExprNodeVistor* _Vistor);
	virtual int NodeType(){return ReSyntaxExprNode::SELECTION;}
};
class ReLookAroundExprNode :public ReSyntaxExprNode			//LookAround
{
public:
	static const int REVP = 0;	//Reverse positive lookaround
	static const int REVN = 1;	//Reverse negative lookaround
	static const int ORDP = 2;	//Ordered positive lookaround
	static const int ORDN = 3;	//Ordered negative lookaround
	int type;
	virtual void Accept(ReSyntaxExprNodeVistor* _Vistor);
	virtual int NodeType(){return ReSyntaxExprNode::LOOK;}
};


//Factory
class ReNodeFactoryBase
{
public:
	static int count;
public:
	virtual ReSyntaxExprNode* Create() = 0;
};

template<class NodeType>
class ReNodeFactory :public ReNodeFactoryBase
{
public:
	virtual NodeType* Create()
	{
		return new NodeType();
	}
};

class ReSyntaxExprNodeVistor
{
public:
	virtual ~ReSyntaxExprNodeVistor(){}
	virtual void VisitReOrExprNode(ReOrExprNode* _Node) = 0;		//Or Node
	virtual void VisitReConcatExprNode(ReConcatExprNode* _Node) = 0;		//Cat Node
	virtual void VisitReCharExprNode(ReCharExprNode* _Node) = 0;	//Char Node
	virtual void VisitReCapturedGroupExprNode(ReCapturedGroupExprNode* _Node) = 0;	//Captured group
	virtual void VisitReBackTracedGroupExprNode(ReBackTracedGroupExprNode* _Node) = 0;	//BackTraced goup
	virtual void VisitReSelectionExprNode(ReSelectionExprNode* _Node) = 0;
	virtual void VisitReLookAroundExprNode(ReLookAroundExprNode* _Node) = 0;
	virtual void VisitReBackReferenceGroupExprNode(ReBackReferenceGroupExprNode* _Node) = 0;
	static int GetDepth(ReSyntaxExprNode* _Node)
	{
		int n = 0;
		while (_Node->Parent() != nullptr)
		{

			_Node = _Node->Parent();
			n++;
		}
		return n;
	}
};

RE_END_INTERNAL_NAMESPACE


#endif
