#include "ReNFASimulator.h"
#include"ReNFACommand.h"
#include"ReGroups.h"
RE_BEGIN_INTERNAL_NAMESPACE


ReNFASimulator::~ReNFASimulator()
{
}

bool ReNFASimulator::ChooseCurrentState()
{
	__LLIB_ASSERT(GetTopIteratorFromStack() != GetTopSetFromStack().End());

	ReNFA::StateNum stateNum = *GetTopIteratorFromStack();
	ReNFAState& state = nfa.GetState(stateNum);

	currIter = GetCurrIterFromStack();
	currentState = stateNum;	//放到执行command之前，这样command即使不更改currentstate也会有默认值

	//也可能没有Command
	for (auto& com : state.command)
	{
		if (com->Execute(this))
		{
			return true;
		}
		else
		{
			return BackTrace();
		}
	}
	return true;
}

bool ReNFASimulator::Simulate()	//返回值代表是否接受
{
	NumSet sset;	//开始符号;
	sset.Insert(startState);
	currIter = strIterator;
	PushStateStack(sset, sset.Begin(), currIter);
	bool needToChooseState = true;
	while (true)
	{
		if (needToChooseState&&!ChooseCurrentState())
		{
			return false;
		}

		if (currentState == terminalState)return true;

		auto& state = nfa.GetState(currentState);
		sstl::Set<ReNFA::StateNum>& myNext = nfa.GetState(currentState).NextEpsilonStates();
		sstl::Set<ReNFA::StateNum>& mySelf = nfa.GetState(currentState).SelfState();
		if (!state.command.Empty())
		{
			//mysetEps.Insert(currentState);
			PushStateStack(mySelf, mySelf.Begin(), currIter);
			//有Command的话就压栈，等待ChooseCurrentState的时候执行
			needToChooseState = true;
		}
		else
		{
			if (myNext.Size() == 1)
			{
				needToChooseState = false;
				currentState = *(myNext.Begin());
			}
			else
			{
				PushStateStack(myNext, myNext.Begin(), currIter);
				needToChooseState = true;
			}
			
		}
	}
}

void ReNFASimulator::Reset()
{
	stateStack.Clear();
}

bool ReNFASimulator::BackTrace()	//回溯
{
	while (stateStack.Size() != 0)
	{
		auto& iteOfEpsilon = GetTopIteratorFromStack();
		auto& setOfEpsilon = GetTopSetFromStack();
		__LLIB_ASSERT(iteOfEpsilon != setOfEpsilon.End());
		//如果栈顶的Epsilon集合的Size多于一个，说明肯定是分支造成的而不是因为由Command造成的
		//两个条件缺一不可 ：1.不能Command，2.不能有多个有Command的状态
		if (nfa.GetState(*iteOfEpsilon).command.Size() != 0 && setOfEpsilon.Size() == 1)
		{
			for (auto& com : nfa.GetState(*iteOfEpsilon).command)
			{
				if (com->BacktraceExecute(this))
				{
					currIter = GetCurrIterFromStack();
					return true;
				}

			}
			continue;
		}
		else
		{
			if (setOfEpsilon.Size() != 0 && ++iteOfEpsilon != setOfEpsilon.End())
			{
				currentState = *GetTopIteratorFromStack();		//千万别忘了必须要在这里设置状态
				currIter = GetCurrIterFromStack();
				return true;
			}
			else
			{
				stateStack.Pop();		//已经没有可以探索的路径了
			}
		}
	}
	return false;

}

ReNFASimulator::ReNFASimulator(String_t& _DesStr, ReNFA& _NFA) :desString(_DesStr), nfa(_NFA)
{
	strIterator = desString.Begin();
	currIter = desString.Begin();
	startState = nfa.StartState();
	terminalState = nfa.TerminalState();
	currentState = nfa.StartState();
}

RE_END_INTERNAL_NAMESPACE
