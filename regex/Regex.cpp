#include "Regex.h"

RE_BEGIN_NAMESPACE



Regex::Regex(String_t _Str) :destStr(_Str)
{
	
}


Regex::~Regex()
{

}

bool Regex::Compile()
{
	syntaxTreeBuilder = new inner::ReSyntaxTreeBuilder(destStr);
	bool result = syntaxTreeBuilder->Compile();
	if (!result)return false;

	genEpsilonNFA = new inner::ReGenEpsilonNFAVistor(nfa);
	syntaxTreeBuilder->GetRoot()->Accept(genEpsilonNFA.Obj());
	return true;
}

ReMatch Regex::Match(String_t _Content)
{
	ReMatch match;
	simulator = new inner::ReNFASimulator(_Content, nfa);
	while (true)
	{
		bool res = simulator->Simulate();
		if (res)
		{
			match.groups.PushBack(*syntaxTreeBuilder->GetRoot()->group);
			simulator->strIterator = simulator->currIter;
		}
		else
		{
			simulator->strIterator++;
			if (simulator->strIterator >= simulator->desString.End())
			{
				break;
			}
		}
		simulator->Reset();
		syntaxTreeBuilder->GetRoot()->group->Clear();
	}
	return match;
}

RE_END_NAMESPACE