#include "ReNFA.h"
#include"ReNFACommand.h"
#include"ReGroups.h"
RE_BEGIN_INTERNAL_NAMESPACE


bool ReNFA::AddEpsilonEdge(StateNum _From, StateNum _To)
{
//	__LLIB_ASSERT(states.Find(_To) != states.End());
	states[_From].AddEpsilonNextState(_To);
	return true;
}

ReNFA::ReNFA()
{
}

ReNFA::~ReNFA()
{
}

RE_END_INTERNAL_NAMESPACE