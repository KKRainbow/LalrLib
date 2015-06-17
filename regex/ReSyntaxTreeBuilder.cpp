#include "ReSyntaxTreeBuilder.h"
RE_BEGIN_INTERNAL_NAMESPACE

#define CHECK_NULL(t) \
if (t->Left() == nullptr&&t->Right() == nullptr)\
{\
	delete t; \
	t = nullptr; \
}

//Parser and build syntax tree
int ReNodeFactoryBase::count = 0;

ReCapturedGroupExprNode* ReSyntaxTreeBuilder::FindCapturedGroupByName(const String_t& _Name,ReCapturedGroupExprNode* _Root)
{
	ReCapturedGroupExprNode* res;
	if (_Root->childCaptured.Empty())return nullptr;
	for (auto& n : _Root->childCaptured)
	{
		if (n->name == _Name)return n;
		else
		{
			res = FindCapturedGroupByName(_Name, n);
			if (res != nullptr)
				return res;
		}
	}
	return nullptr;
}
ReSyntaxExprNode* ReSyntaxTreeBuilder::BuildChildReTree(String_t _Re)
{
	ReSyntaxTreeBuilder builder(_Re, esc);
	builder.Compile();
	ReSyntaxExprNode* res = builder.GetRoot()->Left();	//��Ҫ�ʼ�Ǹ�Captured��
	delete builder.GetRoot();
	return res;
}

ReSyntaxExprNode* ReSyntaxTreeBuilder::Re()	//���ڵ�ʹ��Or����ʵCatҲ����
{
	ReSyntaxExprNode* _Root = ReNodeFactory<ReOrExprNode>().Create();
	_Root->AddLeftChild(Expr());
	if (tokenizer.Match(__R('|')))
	{
		_Root->AddRightChild(ReOr());
	}
	CHECK_NULL(_Root);

	if (_Root&&_Root->Right() == nullptr)
	{
		ReSyntaxExprNode* tmp = _Root->Left();
		_Root->Left() = nullptr;
		delete _Root;
		_Root = tmp;
		_Root->Parent() = nullptr;
	}

	return _Root;
}
ReSyntaxExprNode* ReSyntaxTreeBuilder::ReOr()
{
	if (tokenizer.NextIfMatch(__R('|')))
	{
		return Re();
	}
	else
	{
		return nullptr;
	}
}
ReSyntaxExprNode* ReSyntaxTreeBuilder::Expr()	//�൱��Concatenate
{
	ReSyntaxExprNode* _Root = ReNodeFactory<ReConcatExprNode>().Create();

	_Root->AddLeftChild(Factor());

	if (_Root->Left() != nullptr)
	{
		_Root->AddRightChild(Expr());
	}

	CHECK_NULL(_Root);
	if (_Root&&_Root->Right() == nullptr)
	{
		ReSyntaxExprNode* tmp = _Root->Left();
		_Root->Left() = nullptr;
		delete _Root;
		_Root = tmp;
		_Root->Parent() = nullptr;
	}
	return _Root;
}
ReSyntaxExprNode* ReSyntaxTreeBuilder::Factor()
{
	ReSyntaxExprNode* res;
	if (tokenizer.Match(__R('(')))
	{
		if ((tokenizer.Match(__R('<'), 1)
			|| tokenizer.Match(__R('?'), 1))
			&& (tokenizer.Match(__R('='), 2)
			|| tokenizer.Match(__R('!'), 2))
			)
		{
			res =  LookAround();
		}
		else if (tokenizer.Match(__R('<'), 1)
			&& (tokenizer.IsPosDigital(2)
			|| tokenizer.Match(__R('$'), 2)))
		{
			res = BackTracedGroup();
			Quantifier(res);
		}
		else if (tokenizer.Match(__R('<'), 1)
			&& (tokenizer.IsPosDigital(2)
			|| tokenizer.Match(__R('#'), 2)))
		{
			res = BackReference();
			Quantifier(res);
		}
		else //ֻ��ͨ���������жϲ����б�
		{
			res = CapturedGroup();
			Quantifier(res);
		}
	}
	else if (tokenizer.Match(__R('[')))
	{
		if (tokenizer.Match(__R('^'), 1))
		{
			res = NoSelection();
			Quantifier(res);
		}
		else
		{
			res = Selection();
			Quantifier(res);
		}

	}
	else if (tokenizer.Finish())
	{
		return nullptr;
	}
	else if (!tokenizer.IsPosSpecial())
	{
		ReCharExprNode* tmp = nullptr;
		Char(tmp);
		res = tmp;
		Quantifier(res);
	}
	else
	{
		return nullptr;
	}
	return res;
}
ReSyntaxExprNode* ReSyntaxTreeBuilder::Selection()
{
	ReSelectionExprNode* res = ReNodeFactory<ReSelectionExprNode>().Create();
	res->isSelectNot = false;

	if (!tokenizer.NextIfMatch(__R('['))){ ReportError("No [ in a selection"); delete res; return nullptr; }

	SelectionContent(res);

	if (!tokenizer.NextIfMatch(__R(']'))){ ReportError("No ] in a selection"); delete res; return nullptr; }

	return res;

}
ReSyntaxExprNode* ReSyntaxTreeBuilder::NoSelection()
{
	ReSelectionExprNode* res = ReNodeFactory<ReSelectionExprNode>().Create();
	res->isSelectNot = true;

	if (!tokenizer.NextIfMatch(__R('['))){ ReportError("No [ in a noselection"); delete res; return nullptr; }
	if (!tokenizer.NextIfMatch(__R('^'))){ ReportError("No ^ in a noselection"); delete res; return nullptr; }

	SelectionContent(res);

	if (!tokenizer.NextIfMatch(__R(']'))){ ReportError("No ] in a noselection"); delete res; return nullptr; }

	return res;
}
void ReSyntaxTreeBuilder::SelectionContent(ReSelectionExprNode* _Node)
{
	ReCharExprNode* node;

	Char(node);
	_Node->AddChars(node);		//��ס����Selection��˵��Chars����Ķ���ȫ�ǳɶԳ��ֵģ����㲻��a-z��������ʽ��Ҳ��a-a������

	if (tokenizer.Match(__R('-')))
	{
		tokenizer.Next();
		Char(node);
		_Node->AddChars(node);
	}
	else
	{
		_Node->AddChars(new ReCharExprNode(*node));
	}
	if (!tokenizer.Match(__R(']')))
	{
		SelectionContent(_Node);
	}

	return;
}
ReSyntaxExprNode* ReSyntaxTreeBuilder::CapturedGroup()
{
	ReCapturedGroupExprNode* res = ReNodeFactory<ReCapturedGroupExprNode>().Create();
	ReCapturedGroupExprNode* tmpCapturedGroup = currentCaturedGroup;
	String_t::iterator reBegin, reEnd;

	if (!tokenizer.NextIfMatch(__R('('))){ ReportError("No ] in a noselection"); delete res; return nullptr; }
	CapturedGroupTypeConstraint(res);
	reBegin = tokenizer.CurrentIterator();	//�������������ʽ�Ŀ�ʼ������

	currentCaturedGroup = res;		//�����µ�CaptureGroup

	res->AddLeftChild(Re());	//�����������ʽ

	currentCaturedGroup = tmpCapturedGroup;		//�ָ�����һ��

	if (!tokenizer.NextIfMatch(__R(')'))){ ReportError("Leak parentheses"); delete res; return nullptr; }
	//�������Ѿ������������������õ�����deʱ���ȥ
	reEnd = tokenizer.CurrentIterator() - 1;					// �������Capture�����������ʽ�ַ����Ľ�β

	res->regularExpression = tokenizer.str.Sub(reBegin, reEnd);	//���ô����������ʽ

	currentCaturedGroup->childCaptured.PushBack(res);	//�������ٱ�������µ�Capturedgroup����������ݹ�����

	return res;
}
void ReSyntaxTreeBuilder::CapturedGroupTypeConstraint(ReCapturedGroupExprNode* _Node)
{
	if (!tokenizer.Match(__R('<')) || (tokenizer.Match(__R('>'), 1)))
	{
		AnonymousGroupConstraint(_Node);
	}
	else
	{
		NamedGroupConstraint(_Node);
	}
}
void ReSyntaxTreeBuilder::NamedGroupConstraint(ReCapturedGroupExprNode* _Node)
{
	if (!tokenizer.NextIfMatch(__R('<'))){ ReportError("No < in a named group"); return; }

	while (!tokenizer.Finish() && !tokenizer.Match(__R('>')))
	{
		ReCharExprNode* tmp;
		Char(tmp);
		_Node->AddCharsToName(tmp);
		delete tmp;
	}

	if (!tokenizer.NextIfMatch(__R('>'))){ ReportError("No > in a named group"); return; }
}
void ReSyntaxTreeBuilder::AnonymousGroupConstraint(ReCapturedGroupExprNode* _Node)
{
	if (tokenizer.NextIfMatch(__R('<')))
	{
		if (!tokenizer.NextIfMatch(__R('>'))){ ReportError("No > in a anonymous group"); return; }
	}
	//���е�����˵��ƥ��ɹ�,��ʹû��<>Ҳ��ɹ�ƥ��
}

ReBackReferenceGroupExprNode* ReSyntaxTreeBuilder::BackReference()
{
	ReBackReferenceGroupExprNode* res = ReNodeFactory<ReBackReferenceGroupExprNode>().Create();
	if (!tokenizer.NextIfMatch(__R('('))){ ReportError("Leak parentheses in backref"); delete res; return nullptr; }
	if (!tokenizer.NextIfMatch(__R('<'))){ ReportError("Leak parentheses in backref"); delete res; return nullptr; }
	if (!tokenizer.NextIfMatch(__R('#'))){ ReportError("Leak # in backref"); delete res; return nullptr; }

	if (!tokenizer.IsPosAlpha()){ ReportError("BackReference Without Name");  delete res; return nullptr; }

	while (!tokenizer.Finish() && tokenizer.IsPosAlpha())
	{
		ReCharExprNode* tmp;
		Char(tmp);
		res->AddCharsToName(tmp);
		delete tmp;
	}

	//�ҵ���Ӧ��Captured Group
	ReCapturedGroupExprNode* _CNode = FindCapturedGroupByName(res->name, GetRoot());
	if (_CNode == nullptr)//û���ҵ�������
	{
		ReportError("No corresponsible captured group");
		delete res; 
		return nullptr;
	}
	else	//�ҵ��ˣ����ҵ���node��Regular Expression�������﷨������������
	{
		res->AddLeftChild(BuildChildReTree(_CNode->regularExpression));
	}

	if (!tokenizer.NextIfMatch(__R('>'))){ ReportError("Leak > in backref"); delete res; return nullptr; }
	if (!tokenizer.NextIfMatch(__R(')'))){ ReportError("Leak parentheses in backref"); delete res; return nullptr; }

	return res;
}


ReSyntaxExprNode* ReSyntaxTreeBuilder::BackTracedGroup()
{
	ReBackTracedGroupExprNode* res = ReNodeFactory<ReBackTracedGroupExprNode>().Create();
	if (!tokenizer.NextIfMatch(__R('('))){ ReportError("Leak ( in backtrace"); delete res; return nullptr; }
	if (!tokenizer.NextIfMatch(__R('<'))){ ReportError("Leak < in backtrace"); delete res; return nullptr; }

	if (tokenizer.Match(__R('$')))	//������
	{
		tokenizer.Next();
		while (!tokenizer.Finish()&&tokenizer.IsPosAlpha())
		{
			ReCharExprNode* tmp;
			Char(tmp);
			res->AddCharsToName(tmp);
			delete tmp;
		}
		BackTracedGroupInternalNum(res);
	}
	else if (tokenizer.IsPosDigital())		//�����
	{
		Num(res->backTracedNum);
		res->hasNum = true;
	}

	//�ҵ���Ӧ��Captured Group
	if (res->name.Length() != 0)	//������
	{
		ReCapturedGroupExprNode* _CNode = FindCapturedGroupByName(res->name, GetRoot());
		if (_CNode == nullptr)//û���ҵ�������
		{
			ReportError("No corresponsible captured group");
		}
		else	//�ҵ��ˣ����ҵ���node��Regular Expression�������﷨������������
		{
			if (res->hasNum == true)
			{
				if (_CNode->childCaptured.Size() < res->backTracedNum)
					ReportError("Found no captured group in back traced");
				else
				{
					res->correspondCapturedGroup = _CNode->childCaptured[res->backTracedNum-1];
				}
			}
			else
			{
				res->correspondCapturedGroup = _CNode;
			}
		}
	}
	else
	{
		res->correspondCapturedGroup = currentCaturedGroup->childCaptured[res->backTracedNum-1];
	}
	//////////////////////

	if (!tokenizer.NextIfMatch(__R('>'))){ ReportError("Leak > in backtrace"); delete res; return nullptr; }

	res->AddLeftChild(Re());

	if (!tokenizer.NextIfMatch(__R(')'))){ ReportError("Leak ) in backtrace"); delete res; return nullptr; }
	return res;
}

void ReSyntaxTreeBuilder::BackTracedGroupInternalNum(ReBackTracedGroupExprNode* _Node)
{
	if (tokenizer.Match(__R(';')))
	{
		tokenizer.Next();
		Num(_Node->backTracedNum);				//
		_Node->hasNum = true;
	}
	else
	{
		_Node->hasNum = false;
		return;
	}
}

ReSyntaxExprNode* ReSyntaxTreeBuilder::LookAround()
{
	ReLookAroundExprNode* res = ReNodeFactory<ReLookAroundExprNode>().Create();
	if (!tokenizer.NextIfMatch(__R('('))){ ReportError("Leak ( in lookaround"); delete res; return nullptr; }

	LookAroundTypeConstraint(res);

	res->AddLeftChild(Re());

	if (!tokenizer.NextIfMatch(__R(')'))){ ReportError("Leak ( in lookaround"); delete res; return nullptr; }


	CHECK_NULL(res);
	return res;
}

void ReSyntaxTreeBuilder::LookAroundTypeConstraint(ReLookAroundExprNode* _Node)
{
	if (tokenizer.Match(__R('<'))) //Reverse
	{
		tokenizer.Next();
		if (tokenizer.Match(__R('=')))
		{
			_Node->type = ReLookAroundExprNode::REVP;
			tokenizer.Next();
		}
		else if (tokenizer.Match(__R('!')))
		{
			_Node->type = ReLookAroundExprNode::REVN;
			tokenizer.Next();
		}
		else
		{
			ReportError("Lookaround wrong");
		}
	}
	else if (tokenizer.Match(__R('?')))	//Ordered
	{
		tokenizer.Next();
		if (tokenizer.Match(__R('=')))
		{
			_Node->type = ReLookAroundExprNode::ORDP;
			tokenizer.Next();
		}
		else if (tokenizer.Match(__R('!')))
		{
			_Node->type = ReLookAroundExprNode::ORDN;
			tokenizer.Next();
		}
		else
		{
			ReportError("Lookaround wrong");
		}
	}
}
void ReSyntaxTreeBuilder::Quantifier(ReSyntaxExprNode* _Node)
{
	if (!_Node)return;
	_Node->isGreedy = false;
	if (tokenizer.Match(__R('?')))
	{
		_Node->from = 0;
		_Node->to = 1;
		tokenizer.Next();
	}
	else if (tokenizer.Match(__R('*')))
	{
		_Node->from = 0;
		_Node->to = ReSyntaxExprNode::FINITE;
		tokenizer.Next();
	}
	else if (tokenizer.Match(__R('+')))
	{
		_Node->from = 1;
		_Node->to = ReSyntaxExprNode::FINITE;
		tokenizer.Next();
	}
	else if (tokenizer.Match(__R('{')))
	{
		tokenizer.Next();
		Num(_Node->from);
		if (tokenizer.Match(__R(',')))
		{
			tokenizer.Next();
			AlterNum(_Node->to);
		}
		if (!tokenizer.Match(__R('}')))
		{
			ReportError("");
		}
		tokenizer.Next();
	}
	else
	{
		return;
	}
	if (tokenizer.Match(__R('?')))		//��̰��ģʽ
	{
		_Node->isGreedy = false;
		tokenizer.Next();
	}
	else
	{
		_Node->isGreedy = true;
	}
}
void ReSyntaxTreeBuilder::AlterNum(unsigned int& _Num)
{
	if (tokenizer.IsPosDigital())
	{
		Num(_Num);
	}
	else
	{
		_Num = ReSyntaxExprNode::FINITE;
		return;
	}
}
void ReSyntaxTreeBuilder::Char(ReCharExprNode*& _Node)
{
	_Node = ReNodeFactory<ReCharExprNode>().Create();
	if (tokenizer.Match(this->esc))
	{
		tokenizer.Next();
		_Node->isSpecial = tokenizer.isSplashSpecial();
		_Node->c = tokenizer.Current();
		tokenizer.Next();
		return;
	}
	else if (!tokenizer.IsPosSpecial())
	{
		//���û��ǲ�Ҫ���ӿո�ȽϺ�
		/*
		while (tokenizer.Match(__R(' ')))//Ignore whitespace
		{
			tokenizer.Next();
		}*/
		_Node->isSpecial = tokenizer.isNoSplashSpecial();
		_Node->c = tokenizer.Current();
		tokenizer.Next();
		return;
	}
	else
	{
		ReportError("Error when reading chars");
		tokenizer.Next();
	}
}
void ReSyntaxTreeBuilder::ReportError(const char* error, int pos)
{
	errorStr = error;
	hasError = true;
}
void ReSyntaxTreeBuilder::Num(unsigned int& _Num)
{
	int n = 0;
	int index = tokenizer.Iterator() - tokenizer.str.Begin();
	for (; tokenizer.IsPosDigital(); tokenizer.Next(), ++n);

	String_t tmp = tokenizer.str.Sub(index, index + n);
	_Num = tokenizer.ToDigital(tmp);
}

RE_END_INTERNAL_NAMESPACE
