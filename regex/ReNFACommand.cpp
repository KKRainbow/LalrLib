#include"ReNFACommand.h"
#include"ReGroups.h"
RE_BEGIN_INTERNAL_NAMESPACE
//���ǣ��������ʵĴ��ڣ��������Ҫά���Լ��ڲ�ͬ�ε�ѭ���е������ġ���ֹ���ѭ�����½��������Ⱦ

//Quantifier//////////////////////////////////////////
bool ReNFACommandQuantifier::Execute(ReNFASimulator* _Simulator)
{
	if (mystackList.Top().Size() != 0 && mystackList.Top().Top() == reEnd)	//��һ�����е��⣬Ҫ����һ�������ģ�����context��top��end��ҲҪ�����µ�������
	{
		mystackList.Push(Context());
		mystackList.Top().Reserve(min+100);
	}
	Context& mystack = mystackList.Top();
	if (mystack.Size() < min)
	{
		mystack.Push(reStart);
		_Simulator->currentState = reStart;
		return true;
	}
	else if (mystack.Size() >= max)
	{
		mystack.Push(reEnd);
		_Simulator->currentState = reEnd;
		return true;
	}
	else
	{
		if (isGreedy)
		{
			mystack.Push(reStart);
			_Simulator->currentState = reStart;
		}
		else
		{
			mystack.Push(reEnd);
			_Simulator->currentState = reEnd;
		}
		return true;
	}
}
inline void ReNFACommandQuantifier::TestContext()	//�����Ƿ�Ӧ�õ���Context
{
	if (mystackList.Top().Size() == 0 && mystackList.Size() >1)
	{
		mystackList.Pop();
	}
}
bool ReNFACommandQuantifier::BacktraceExecute(ReNFASimulator* _Simulator)
{
	Context& mystack = mystackList.Top();
	if (mystack.Top() == reEnd)	//�Ǵӱհ����ʽ��������������
	{
		if (mystack.Size() >= max)
		{
			mystack.Pop();
			_Simulator->stateStack.Pop();
			TestContext();
			return false;
		}
		else //�����浯�����Ļ��϶��Ƿ��ϴ���Ҫ���
		{
			if (isGreedy)
			{
				mystack.Pop();
				_Simulator->stateStack.Pop();
				TestContext();
				return false;
			}
			else
			{
				mystack.Top() = reStart;
				_Simulator->currentState = reStart;
				return true;
			}
		}
	}
	else					//���ڲ�������
	{
		if (mystack.Size() <= min)
		{
			mystack.Pop();
			_Simulator->stateStack.Pop();
			TestContext();
			return false;
		}
		//�ڲ���תsize�϶�С��max
		if (isGreedy)
		{
			mystack.Top() = reEnd;
			_Simulator->currentState = reEnd;
			return true;
		}
		else
		{
			mystack.Pop();
			_Simulator->stateStack.Pop();
			TestContext();
			return false;
		}
	}
}
//////////////////////////////////////////////////////

//Capture//////////////////////////////////////////
bool ReNFACommandCapturedStart::Execute(ReNFASimulator* _Simulator)
{
	group->result.PushBack(String_t::Empty);
	start = _Simulator->currIter;
	_Simulator->currentState = nextState;
	return true;
}
bool ReNFACommandCapturedStart::BacktraceExecute(ReNFASimulator* _Simulator)
{
	group->result.PopBack();
	//group->result = String_t::Empty;
	_Simulator->stateStack.Pop();
	return false;
}
bool ReNFACommandCapturedEnd::Execute(ReNFASimulator* _Simulator)
{
	commandStart->group->result[commandStart->group->result.Size() - 1] = _Simulator->desString.Sub(commandStart->start, _Simulator->currIter);
	_Simulator->currentState = reEnd;
	return true;
}
bool ReNFACommandCapturedEnd::BacktraceExecute(ReNFASimulator* _Simulator)
{
	commandStart->group->result[commandStart->group->result.Size() - 1] = String_t::Empty;
	_Simulator->stateStack.Pop();
	return false;
}
//////////////////////////////////////////////////////

//Char//////////////////////////////////////////////
bool ReNFACommandCharStart::Execute(ReNFASimulator* _Simulator)
{
	if (_Simulator->currIter == _Simulator->desString.End())return false;
	if (isComplimentary)//����ǲ�����ÿһ��judger��Ҫ���У��������true�򷵻�false
	{
		for (auto& func : this->judgeFunctors)
		{
			if (func(*(_Simulator->currIter), _Simulator))
			{
				return false;
			}
		}
		_Simulator->currentState = nextState;
		++(_Simulator->currIter);
		return true;
	}
	else
	{
		for (auto& func : this->judgeFunctors)
		{
			if (func(*(_Simulator->currIter), _Simulator))
			{
				_Simulator->currentState = nextState;
				++(_Simulator->currIter);
				return true;
			}
		}
		return false;
	}

}
bool ReNFACommandCharStart::BacktraceExecute(ReNFASimulator* _Simulator)
{
	_Simulator->stateStack.Pop();
	return false;	//��������
}
//////////////////////////////////////////////////////


//LookAround////////////////////////////////////////
bool ReNFACommandLookaround::Execute(ReNFASimulator* _Simulator)
{
	ReNFASimulator mySimulator(_Simulator->desString, _Simulator->nfa);
	mySimulator.startState = this->reStart;
	mySimulator.terminalState = this->reTerminal;
	mySimulator.strIterator = _Simulator->strIterator;

	bool result = mySimulator.Simulate();
	switch (this->type)
	{
	case ReLookAroundExprNode::REVP:				//����϶�����,���٣���֪������ôʵ��
		if (result)
		{
			_Simulator->currentState = this->itsTerminal;
			return true;
		}
		else
		{
			return false;
		}

	case ReLookAroundExprNode::REVN:				//
		if (!result)
		{
			_Simulator->currentState = this->itsTerminal;
			return true;
		}
		else
		{
			return false;
		}

	case ReLookAroundExprNode::ORDP:				//����϶�����,
		if (result)
		{
			_Simulator->currentState = this->itsTerminal;
			return true;
		}
		else
		{
			return false;
		}
	case ReLookAroundExprNode::ORDN:				//����񶨻���,
		if (!result)
		{
			_Simulator->currentState = this->itsTerminal;
			return true;
		}
		else
		{
			return false;
		}
	}
	
	return true;

}
bool ReNFACommandLookaround::BacktraceExecute(ReNFASimulator* _Simulator)
{
	_Simulator->stateStack.Pop();
	return false;
}
//////////////////////////////////////////////////////


//Backtrace////////////////////////////////////////
bool ReNFACommanBacktrace::Execute(ReNFASimulator* _Simulator)
{
	String_t& srcstr = this->group->result[0];
	int len = srcstr.Length();
	String_t desstr = _Simulator->desString.Sub(_Simulator->currIter, _Simulator->currIter + len);

	if (desstr == srcstr)
	{
		_Simulator->currIter += len;	//��������ı�
		_Simulator->currentState = reTerminal;
		return true;
	}
	else
	{
		return false;
	}
}
bool ReNFACommanBacktrace::BacktraceExecute(ReNFASimulator* _Simulator)
{
	_Simulator->stateStack.Pop();
	return false;
}
//////////////////////////////////////////////////////

RE_END_INTERNAL_NAMESPACE