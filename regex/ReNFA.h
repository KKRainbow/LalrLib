#pragma once
#ifndef RE_NFA_H
#define RE_NFA_H
#include"config.h"
#include"../stl/sset.h"
#include"ReGroups.h"

RE_BEGIN_INTERNAL_NAMESPACE

class ReNFASimulator;
class ReNFACommand
{
public:
	virtual bool Execute(ReNFASimulator* _Simulator) = 0;				//返回值代表是否执行成功
	virtual bool BacktraceExecute(ReNFASimulator* _Simulator) = 0;		//
	virtual ~ReNFACommand(){};
};

class ReNFAState
{
public:
	enum StateType{ ORDINARY, START, TERMINAL };
protected:
	typedef int StateNum;
	friend class ReNFA;

	//不管出还是入都是多对多关系
	sstl::Set<StateNum>								nextStates;								//出边的集合
	sstl::Set<StateNum>								selfStates;								//代表自己
	StateType										type = ORDINARY;						//状态的类型
	void AddEpsilonNextState(StateNum _State)
	{
		nextStates.Insert(_State);
	}
	
public:
	sstl::Vector<Ptr<ReNFACommand>>													command;					//状态的附加命令
	ReNFAState(){}
	ReNFAState(StateType _Type, StateNum _Num) :type(_Type){ selfStates.Insert(_Num); }

	sstl::Set<StateNum>& NextEpsilonStates() 
	{
		return nextStates;
	}
	sstl::Set<StateNum>& SelfState()
	{
		return selfStates;
	}
};

class ReNFA
{
public:
	typedef int StateNum;
private:
	StateNum stateID = 0;
protected:
//	sstl::Map<StateNum, ReNFAState> states;	//使用Map便于删除
	sstl::Vector<ReNFAState> states;
	StateNum startState;
	StateNum terminalState;
public:
	static const StateNum INVALID_STATE_NUM = -1;
	//ReNFA();
	ReNFA();
	~ReNFA();
	StateNum StartState(){ return startState; }
	StateNum TerminalState(){ return terminalState; }
	StateNum CreateState(ReNFAState::StateType _Type = ReNFAState::ORDINARY)
	{
		//states.Insert(sstl::MakePair((StateNum)stateID, ReNFAState(_Type)));
		states.PushBack(ReNFAState(_Type,stateID));	
		if (_Type == ReNFAState::START)
		{
			startState = stateID;
		}
		else if (_Type == ReNFAState::TERMINAL)
		{
			terminalState = stateID;
		}
		return stateID++;
	}
	
	ReNFAState& GetState(StateNum num)
	{ 
		/*
		auto state = states.Find(num);
		__LLIB_ASSERT(state != states.End());
		return states.Find(num)->second; 
		*/
		return states[num];
	}

	void ChangeStateType(StateNum num, ReNFAState::StateType _Type)
	{
		GetState(num).type = _Type;

		if (_Type == ReNFAState::START)
		{
			startState = num;
		}
		else if (_Type == ReNFAState::TERMINAL)
		{
			terminalState = num;
		}
	}

	bool AddEpsilonEdge(StateNum _From, StateNum _To);

	void GetNextEpsilonStates(StateNum _From, sstl::Set<StateNum>& res)
	{
		ReNFAState& state = GetState(_From);
		res += state.NextEpsilonStates();
	}
};


RE_END_INTERNAL_NAMESPACE



#endif
