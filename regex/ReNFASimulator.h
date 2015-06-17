#pragma once
#ifndef RE_NFASIMULATOR_H
#define RE_NFASIMULATOR_H

#include"config.h"
#include"ReNFA.h"
#include"../stl/sstack.h"
#include"ReGroups.h"

RE_BEGIN_INTERNAL_NAMESPACE


class ReNFASimulator
{
private:
	typedef sstl::Set<ReNFA::StateNum> NumSet;
public:
	
	Ptr<ReGroups> capturedGroups;
	sstl::Stack<sstl::Tuple<NumSet*, NumSet::iterator,	//epsilon边
		String_t::iterator>> stateStack;
	ReNFA::StateNum currentState;
	String_t desString;
	ReNFA& nfa;
	String_t::iterator strIterator;
	String_t::iterator currIter;

	ReNFA::StateNum startState;
	ReNFA::StateNum terminalState;
private:
	bool BackTrace();
	bool ChooseCurrentState();
public:
	ReNFASimulator(String_t& _DesStr, ReNFA& _NFA);

	~ReNFASimulator();
	
	void PushStateStack(NumSet& s2, NumSet::iterator i2,	//epsilon边
		String_t::iterator stri)
	{
		stateStack.Push(sstl::MakeTuple(&s2, s2.Begin(), currIter));
		GetTopIteratorFromStack() = GetTopSetFromStack().Begin();
	}

	inline NumSet& GetTopSetFromStack(){ return *sstl::Get<0>(stateStack.Top()); }
	inline NumSet::iterator& GetTopIteratorFromStack(){ return sstl::Get<1>(stateStack.Top()); }
	inline String_t::iterator& GetCurrIterFromStack(){ return sstl::Get<2>(stateStack.Top()); }
	bool Simulate();	//返回值代表是否接受
	void Reset();

};


RE_END_INTERNAL_NAMESPACE
#endif
