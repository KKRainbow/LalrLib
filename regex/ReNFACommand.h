#pragma once
#ifndef RE_NFACOMMAND_H
#define RE_NFACOMMAND_H
#include"config.h"
#include"ReNFASimulator.h"
RE_BEGIN_INTERNAL_NAMESPACE

class ReNFASimulator;
class ReGroups;



class ReNFACommandQuantifier :public ReNFACommand
{
public:
	ReNFA::StateNum quantifierStart;		//这个command所在的状态
	ReNFA::StateNum reEnd;				//跳转到结尾
	ReNFA::StateNum reStart;			//跳转到开头
	unsigned int min;
	unsigned int max;
	bool isGreedy;

	typedef sstl::Stack<ReNFA::StateNum> Context;
	sstl::Stack<Context> mystackList;	//用Stack装着，因为这个quantifier可能有不同的context
	virtual bool Execute(ReNFASimulator* _Simulator);
	inline void TestContext();
	virtual bool BacktraceExecute(ReNFASimulator* _Simulator);
	ReNFACommandQuantifier(ReNFA::StateNum _QS, ReNFA::StateNum _RE, ReNFA::StateNum _RS, const int _Min, const int _Max, bool _IsGreedy)
		:quantifierStart(_QS), reEnd(_RE), reStart(_RS), min(_Min), max(_Max), isGreedy(_IsGreedy)
	{
		mystackList.Reserve(30);
		mystackList.Push(Context());
		mystackList.Top().Reserve(min + 100);
	}
	~ReNFACommandQuantifier()
	{
		mystackList.Clear();
	}
};

class ReNFACommandCapturedStart :public ReNFACommand
{
public:
	ReNFA::StateNum nextState;
	ReGroups* group = nullptr;
	String_t::iterator start = nullptr;
public:
	virtual bool Execute(ReNFASimulator* _Simulator);
	virtual bool BacktraceExecute(ReNFASimulator* _Simulator);

	ReNFACommandCapturedStart(ReNFA::StateNum _NextState, ReGroups* _Group)
		:nextState(_NextState), group(_Group)
	{
	}
};

class ReNFACommandCapturedEnd :public ReNFACommand
{
	ReNFACommandCapturedStart* commandStart;
	ReNFA::StateNum reEnd;
public:
	virtual bool Execute(ReNFASimulator* _Simulator);
	virtual bool BacktraceExecute(ReNFASimulator* _Simulator);

	ReNFACommandCapturedEnd(ReNFACommandCapturedStart* _CommandStart, ReNFA::StateNum _ReEnd) 
		:commandStart(_CommandStart), reEnd(_ReEnd)
	{}
};

class ReNFACommandCharJudger :public sstl::BinaryFunction<Char_t, ReNFASimulator*, bool>
{
private:
	Char_t from = 0;
	Char_t to = 0;
	bool isSpecial = false;
	bool SpecialChar(Char_t _C, ReNFASimulator* _Simulator)
	{
		switch (this->from)
		{
		case 's':
			return iswspace(_C) != 0;
		case 'S':
			return iswspace(_C) == 0;

		case 'w':
			return iswalpha(_C) != 0;
		case 'W':
			return iswalpha(_C) == 0;
		case 'd':
			return iswdigit(_C) != 0;
		case 'D':
			return iswdigit(_C) == 0;
		case 'l':
			return int(_C <= L'z'&&_C >= L'a') || (_C <= L'Z'&&_C >= L'A') != 0;
		case 'L':
			return int(_C <= L'z'&&_C >= L'a') || (_C <= L'Z'&&_C >= L'A') == 0;
		case '.':
			return _C != __R('\n');

		case 'b':	//匹配单词前或后的空格
			if (_Simulator->currIter == _Simulator->desString.Begin()){ return true; }
			else
			{
				bool res = iswspace(*(_Simulator->currIter - 1)) != 0 || iswspace(*(_Simulator->currIter + 1)) != 0;
				if(res)_Simulator->currIter--;	//这个不占用字符，因此回退一个
				return res;
			}
		default:
			__LLIB_ASSERT(false);
			return false;
		}
	}
	bool NormalChar(Char_t _C, ReNFASimulator* _Simulator)
	{
		return _C >= from&&_C <= to;
	}
public:
	bool operator()(Char_t _C, ReNFASimulator* _Simulator)
	{
		return (isSpecial ? SpecialChar(_C, _Simulator) : NormalChar(_C, _Simulator));
	}
	ReNFACommandCharJudger(Char_t _From, Char_t _To, bool _IsSpecial)
		:from(_From), to(_To), isSpecial(_IsSpecial){}
	ReNFACommandCharJudger(){}
};

class ReNFACommandCharStart :public ReNFACommand
{
private:
	ReNFA::StateNum nextState;
	bool isComplimentary;
public:
	sstl::Vector<ReNFACommandCharJudger> judgeFunctors;
public:
	virtual bool Execute(ReNFASimulator* _Simulator);
	virtual bool BacktraceExecute(ReNFASimulator* _Simulator);

	ReNFACommandCharStart(ReNFA::StateNum _NextState, bool _IsComplimentary) :nextState(_NextState), isComplimentary(_IsComplimentary)
	{}
};

class ReNFACommandLookaround :public ReNFACommand
{
public:
	ReNFA::StateNum reStart;
	ReNFA::StateNum reTerminal;
	ReNFA::StateNum itsTerminal;
	int type;	//环视的类型
	virtual bool Execute(ReNFASimulator* _Simulator);				//返回值代表是否执行成功
	virtual bool BacktraceExecute(ReNFASimulator* _Simulator);		//
	ReNFACommandLookaround(ReNFA::StateNum _ReStart, ReNFA::StateNum _ReTer,ReNFA::StateNum _ItsTerminal,int _Type)
		:reStart(_ReStart), reTerminal(_ReTer), itsTerminal(_ItsTerminal), type(_Type)
	{}
};

class ReNFACommanBacktrace :public ReNFACommand
{
public:
	ReGroups* group;
	ReNFA::StateNum reTerminal;
	virtual bool Execute(ReNFASimulator* _Simulator);				//返回值代表是否执行成功
	virtual bool BacktraceExecute(ReNFASimulator* _Simulator);		//
	ReNFACommanBacktrace(ReGroups* _Group, ReNFA::StateNum _ReTerminal)
		:group(_Group), reTerminal(_ReTerminal)
	{}
};

RE_END_INTERNAL_NAMESPACE

#endif
