#pragma once
#include"ReSyntaxExprNode.h"
#include"ReGroups.h"

RE_BEGIN_INTERNAL_NAMESPACE

class ReSyntaxTreeBuilder
{
private:
	const char* errorStr = nullptr;
	bool hasError = false;
public:
	ReTokenizer tokenizer;
#if  __REGEX_ENCODING==__REGEX_ENCODING_UTF16
	Char_t esc = L'\\';
#elif  __REGEX_ENCODING==__REGEX_ENCODING_ASCII
	Char_t esc = '\\';
#endif
private:
	ReCapturedGroupExprNode* currentCaturedGroup = nullptr;
protected:
	Ptr<ReCapturedGroupExprNode> root = ReNodeFactory<ReCapturedGroupExprNode>().Create();
	Ptr<ReGroups> reGroups;

	ReCapturedGroupExprNode* FindCapturedGroupByName(const String_t& _Name, ReCapturedGroupExprNode* _Root);
	ReSyntaxExprNode* BuildChildReTree(String_t _Re);

public:
	ReSyntaxTreeBuilder(String_t _Re, Char_t _Esc = '\\') :tokenizer(_Re), esc(_Esc){}
	bool Compile()
	{ 
		currentCaturedGroup = root.Obj();
		root->AddLeftChild(Re());
		if (!hasError)
		{
			reGroups = ReGroups::GenReGroups(root.Obj());
			return true;
		}
		else
		{
			return false;
		}
	}
	ReCapturedGroupExprNode* GetRoot()const { return root.Obj(); }
	const char* GetError()const { return errorStr; }
protected:
	ReSyntaxExprNode* Re();
	ReSyntaxExprNode* ReOr();
	ReSyntaxExprNode* Expr();
	ReSyntaxExprNode* Factor();
	ReSyntaxExprNode* Selection();
	ReSyntaxExprNode* NoSelection();
	void SelectionContent(ReSelectionExprNode* _Node);
	ReSyntaxExprNode* CapturedGroup();
	void CapturedGroupTypeConstraint(ReCapturedGroupExprNode* _Node);
	void NamedGroupConstraint(ReCapturedGroupExprNode* _Node);
	void AnonymousGroupConstraint(ReCapturedGroupExprNode* _Node);
	ReSyntaxExprNode* BackTracedGroup();
	void BackTracedGroupInternalNum(ReBackTracedGroupExprNode* _Node);
	ReBackReferenceGroupExprNode* BackReference();
	ReSyntaxExprNode* LookAround();
	void LookAroundTypeConstraint(ReLookAroundExprNode* _Node);
	void Quantifier(ReSyntaxExprNode* _Node);
	void AlterNum(unsigned int& _Num);
	void Char(ReCharExprNode*& _Node);
	void ReportError(const char* error, int pos = 0);
	void Num(unsigned int& _Num);
};


RE_END_INTERNAL_NAMESPACE
