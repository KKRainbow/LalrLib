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
	currentState = stateNum;	//�ŵ�ִ��command֮ǰ������command��ʹ������currentstateҲ����Ĭ��ֵ

	//Ҳ����û��Command
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

bool ReNFASimulator::Simulate()	//����ֵ�����Ƿ����
{
	NumSet sset;	//��ʼ����;
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
			//��Command�Ļ���ѹջ���ȴ�ChooseCurrentState��ʱ��ִ��
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

bool ReNFASimulator::BackTrace()	//����
{
	while (stateStack.Size() != 0)
	{
		auto& iteOfEpsilon = GetTopIteratorFromStack();
		auto& setOfEpsilon = GetTopSetFromStack();
		__LLIB_ASSERT(iteOfEpsilon != setOfEpsilon.End());
		//���ջ����Epsilon���ϵ�Size����һ����˵���϶��Ƿ�֧��ɵĶ�������Ϊ��Command��ɵ�
		//��������ȱһ���� ��1.����Command��2.�����ж����Command��״̬
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
				currentState = *GetTopIteratorFromStack();		//ǧ������˱���Ҫ����������״̬
				currIter = GetCurrIterFromStack();
				return true;
			}
			else
			{
				stateStack.Pop();		//�Ѿ�û�п���̽����·����
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
