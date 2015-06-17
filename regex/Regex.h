#pragma once
#include"config.h"
#include"ReNFASimulator.h"
#include"ReSyntaxTreeBuilder.h"
#include"ReGenEpsilonNFAVistor.h"
#include"ReMatch.h"

RE_BEGIN_NAMESPACE

class Regex
{
private:
	inner::ReNFA nfa;
	String_t destStr;
	Ptr<inner::ReNFASimulator> simulator;
	Ptr<inner::ReSyntaxTreeBuilder> syntaxTreeBuilder;
	Ptr<inner::ReGenEpsilonNFAVistor> genEpsilonNFA;

public:
	Regex(String_t _Str);
	~Regex();
	bool Compile();

	ReMatch Match(String_t _Content);
};

RE_END_NAMESPACE