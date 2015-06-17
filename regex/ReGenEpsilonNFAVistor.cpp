#include "ReGenEpsilonNFAVistor.h"
#include"ReNFA.h"
#include"ReNFACommand.h"

RE_BEGIN_INTERNAL_NAMESPACE



void ReGenEpsilonNFAVistor::ConstructQulifiedRe(ReSyntaxExprNode* _Node)
{
	if (_Node->from == 1 && _Node->to == 1)return;
	
	StateNum start = nfa.CreateState(ReNFAState::START);
	StateNum terminal = nfa.CreateState(ReNFAState::TERMINAL);

	StateNum start1 = _Node->FAStartNum;
	StateNum terminal1 =_Node->FATerminalNum;

	nfa.ChangeStateType(start1, ReNFAState::ORDINARY);
	nfa.ChangeStateType(terminal1, ReNFAState::ORDINARY);

	nfa.AddEpsilonEdge(start, start1);
	//nfa.AddEpsilonEdge(terminal1, terminal);		//不能有这条边
	nfa.AddEpsilonEdge(terminal1, start);

	_Node->FAStartNum = start;
	_Node->FATerminalNum = terminal;
	//Add Quantifier Command here
	nfa.GetState(start).command.PushBack(new ReNFACommandQuantifier(start, terminal, start1, _Node->from, _Node->to, _Node->isGreedy));
}

void ReGenEpsilonNFAVistor::VisitReOrExprNode(ReOrExprNode* _Node)
{
	__LLIB_ASSERT(_Node->Left() != nullptr&&_Node->Right() != nullptr);

	StateNum start = nfa.CreateState(ReNFAState::START);
	StateNum terminal = nfa.CreateState(ReNFAState::TERMINAL);

	StateNum start1 = _Node->Left()->FAStartNum;
	StateNum terminal1 = _Node->Left()->FATerminalNum;

	StateNum start2 = _Node->Right()->FAStartNum;
	StateNum terminal2 = _Node->Right()->FATerminalNum;

	nfa.ChangeStateType(start1, ReNFAState::ORDINARY);
	nfa.ChangeStateType(terminal1, ReNFAState::ORDINARY);
	nfa.ChangeStateType(start2, ReNFAState::ORDINARY);
	nfa.ChangeStateType(terminal2, ReNFAState::ORDINARY);

	nfa.AddEpsilonEdge(start, start1);
	nfa.AddEpsilonEdge(start, start2);
	nfa.AddEpsilonEdge(terminal1, terminal);
	nfa.AddEpsilonEdge(terminal2, terminal);

	_Node->FAStartNum = start;
	_Node->FATerminalNum = terminal;

	ConstructQulifiedRe(_Node);
}		//Or Node
void ReGenEpsilonNFAVistor::VisitReConcatExprNode(ReConcatExprNode* _Node)
{
	__LLIB_ASSERT(_Node->Left() != nullptr&&_Node->Right() != nullptr);

//	StateNum start = nfa.CreateState(ReNFAState::START);
//	StateNum terminal = nfa.CreateState(ReNFAState::TERMINAL);
	
	StateNum start1 = _Node->Left()->FAStartNum;
	StateNum terminal1 = _Node->Left()->FATerminalNum;

	StateNum start2 = _Node->Right()->FAStartNum;
	StateNum terminal2 = _Node->Right()->FATerminalNum;

	nfa.ChangeStateType(start1, ReNFAState::START);
	nfa.ChangeStateType(terminal1, ReNFAState::ORDINARY);
	nfa.ChangeStateType(start2, ReNFAState::ORDINARY);
	nfa.ChangeStateType(terminal2, ReNFAState::TERMINAL);

	//nfa.AddEpsilonEdge(start, start1);
	nfa.AddEpsilonEdge(terminal1, start2);
	//nfa.AddEpsilonEdge(terminal2, terminal);

	_Node->FAStartNum = start1;
	_Node->FATerminalNum = terminal2;
	

	ConstructQulifiedRe(_Node);
}		//Cat Node
void ReGenEpsilonNFAVistor::VisitReCharExprNode(ReCharExprNode* _Node)	
{
	
	StateNum start = nfa.CreateState(ReNFAState::START);
	StateNum terminal = nfa.CreateState(ReNFAState::TERMINAL);
	_Node->FAStartNum = start;
	_Node->FATerminalNum = terminal;
	
	ReNFACommandCharJudger judger(_Node->c,_Node->c,_Node->isSpecial);
	ReNFACommandCharStart* com = new ReNFACommandCharStart(terminal, false);
	com->judgeFunctors.PushBack(judger);

	nfa.GetState(start).command.PushBack(com);

	ConstructQulifiedRe(_Node);
}		//Char Node
void ReGenEpsilonNFAVistor::VisitReSelectionExprNode(ReSelectionExprNode* _Node)
{
	StateNum start = nfa.CreateState(ReNFAState::START);
	StateNum terminal = nfa.CreateState(ReNFAState::TERMINAL);

	ReNFACommandCharStart* com = new ReNFACommandCharStart(terminal ,_Node->isSelectNot);

	for (Ptr<ReCharExprNode>* ite = _Node->chars.Begin(); ite != _Node->chars.End(); ite += 2)
	{
		ReNFACommandCharJudger judger((*ite)->c, (*(ite + 1))->c, (*ite)->isSpecial);
		com->judgeFunctors.PushBack(judger);
	}

	_Node->FAStartNum = start;
	_Node->FATerminalNum = terminal;

	nfa.GetState(start).command.PushBack(com);

	ConstructQulifiedRe(_Node);
}


void ReGenEpsilonNFAVistor::VisitReCapturedGroupExprNode(ReCapturedGroupExprNode* _Node)
{
	if (_Node->Left() == nullptr)
	{
		_Node->FAStartNum = nfa.CreateState(ReNFAState::START);
		_Node->FATerminalNum = nfa.CreateState(ReNFAState::TERMINAL);
		nfa.AddEpsilonEdge(_Node->FAStartNum, _Node->FATerminalNum);
		return;
	}

	StateNum start = nfa.CreateState(ReNFAState::START);
	StateNum terminalforcap = nfa.CreateState(ReNFAState::ORDINARY);
	StateNum terminal = nfa.CreateState(ReNFAState::TERMINAL);

	StateNum start1 = _Node->Left()->FAStartNum;
	StateNum terminal1 = _Node->Left()->FATerminalNum;

	nfa.ChangeStateType(start1, ReNFAState::ORDINARY);
	nfa.ChangeStateType(terminal1, ReNFAState::ORDINARY);

	nfa.AddEpsilonEdge(start, start1);
	nfa.AddEpsilonEdge(terminal1, terminalforcap);
	nfa.AddEpsilonEdge(terminalforcap, terminal);

	_Node->FAStartNum = start;
	_Node->FATerminalNum = terminal;
	//Add Captured Command here

	ReNFACommandCapturedStart* cmdstart = new ReNFACommandCapturedStart(start1, _Node->group);
	nfa.GetState(start).command.PushBack(cmdstart);
	ReNFACommandCapturedEnd* cmdend = new ReNFACommandCapturedEnd(cmdstart,terminal);
	nfa.GetState(terminalforcap).command.PushBack(cmdend);

	ConstructQulifiedRe(_Node);
	return;
}		//Captured group

void ReGenEpsilonNFAVistor::VisitReBackTracedGroupExprNode(ReBackTracedGroupExprNode* _Node)
{
	StateNum start = nfa.CreateState(ReNFAState::START);
	StateNum terminal = nfa.CreateState(ReNFAState::TERMINAL);

	nfa.AddEpsilonEdge(start, terminal);

	_Node->FAStartNum = start;
	_Node->FATerminalNum = terminal;
	//Add BackTraced Command here
	ReNFACommanBacktrace* cmdstart = new ReNFACommanBacktrace(_Node->correspondCapturedGroup->group, terminal);
	nfa.GetState(start).command.PushBack(cmdstart);

	ConstructQulifiedRe(_Node);
	return;
}		//BackTraced goup

void ReGenEpsilonNFAVistor::VisitReLookAroundExprNode(ReLookAroundExprNode* _Node)
{
	if (_Node->Left() == nullptr)
	{
		_Node->FAStartNum = nfa.CreateState(ReNFAState::START);
		_Node->FATerminalNum = nfa.CreateState(ReNFAState::TERMINAL);
		nfa.AddEpsilonEdge(_Node->FAStartNum, _Node->FATerminalNum);
		return;
	}

	StateNum start = nfa.CreateState(ReNFAState::START);
	StateNum terminal = nfa.CreateState(ReNFAState::TERMINAL);

	StateNum start1 = _Node->Left()->FAStartNum;
	StateNum terminal1 = _Node->Left()->FATerminalNum;

	nfa.ChangeStateType(start1, ReNFAState::ORDINARY);
	nfa.ChangeStateType(terminal1, ReNFAState::ORDINARY);

	_Node->FAStartNum = start;
	_Node->FATerminalNum = terminal;
	//Add Lookaround Command here
	ReNFACommandLookaround* lookaround = new ReNFACommandLookaround(start1, terminal1, terminal,_Node->type);

	nfa.GetState(start).command.PushBack(lookaround);
	return;
}

void ReGenEpsilonNFAVistor::VisitReBackReferenceGroupExprNode(ReBackReferenceGroupExprNode* _Node)
{
	if (_Node->Left() == nullptr)
	{
		_Node->FAStartNum = nfa.CreateState(ReNFAState::START);
		_Node->FATerminalNum = nfa.CreateState(ReNFAState::TERMINAL);
		nfa.AddEpsilonEdge(_Node->FAStartNum, _Node->FATerminalNum);
		return;
	}
	_Node->FAStartNum = _Node->Left()->FAStartNum;
	_Node->FATerminalNum = _Node->Left()->FATerminalNum;
}

RE_END_INTERNAL_NAMESPACE
