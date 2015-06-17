#pragma once
#ifndef RE_RETOKENIZER_H
#define RE_RETOKENIZER_H

#include"config.h"


RE_BEGIN_INTERNAL_NAMESPACE

class ReTokenizer
{
#ifdef __WIN32
	typedef String_t::value_type char_type;
#elif defined(__LINUX)
	typedef typename String_t::value_type char_type;
#endif
	
private:
	bool isInStr(String_t& _Str, String_t::npos _Pos)
	{
		Char_t _C = LookForward(_Pos);
		if (_Str.Find(_C) == _Str.End())
			return false;
		else
			return true;
	}
public:
	static const Char_t Empty = 0;
	~ReTokenizer(){}
	ReTokenizer(String_t _Str) :str(_Str){ iterator = str.Begin(); }
	String_t reserve = __R("()[]{}<>|*+?");
	String_t splashSpecial = __R("WwDdSsLlb");					//跟在转义字符后面的特殊字符
	String_t noSplashSpecial = __R(".?^$");					//无转义字符后面的特殊字符
#if  __REGEX_ENCODING==__REGEX_ENCODING_UTF16
	
	bool IsPosDigital(String_t::npos _Pos = 0)
	{
		Char_t _C = LookForward(_Pos);
		return iswdigit(_C)!=0;
	}
	bool IsPosAlpha(String_t::npos _Pos = 0)
	{
		Char_t _C = LookForward(_Pos);
		return iswalpha(_C)!=0;
	}
	static int ToDigital(String_t _Str)
	{
		wchar_t* endptr = 0;
		const wchar_t* buf = _Str.Buffer();
		return wcstol(buf, &endptr, 10);
		//return sstl::WStringTool::wtoi64(_Str);
	}
	
#elif  __REGEX_ENCODING==__REGEX_ENCODING_ASCII
	bool IsPosDigital(String_t::npos _Pos= 0)
	{
		Char_t _C = LookForward(_Pos);
		return isdigit(_C);
	}
	bool IsPosAlpha(String_t::npos _Pos= 0)
	{
		Char_t _C = LookForward(_Pos);
		return isalpha(_C);
	}
	static int ToDigital(String_t _Str)
	{
		return sstl::AStringTool::atoi(_Str);
	}
#endif
	bool IsPosSpecial(String_t::npos _Pos = 0)
	{
		return isInStr(reserve, _Pos);
	}
	bool isSplashSpecial(String_t::npos _Pos = 0)
	{
		return isInStr(splashSpecial, _Pos);
	}
	bool isNoSplashSpecial(String_t::npos _Pos = 0)
	{
		return isInStr(noSplashSpecial, _Pos);
	}
	char_type Next()
	{
		printf("Current:\t%C\n", *iterator);
		return iterator >= str.End() ? String_t::zero : *iterator++;
	}
	char_type Current()
	{
		return iterator >= str.End() ? String_t::zero : *iterator;
	}
	String_t::iterator CurrentIterator()
	{
		return iterator >= str.End() ? str.End() : iterator;
	}
	Char_t LookForward(String_t::npos _Pos)
	{
		auto tmp = iterator + _Pos;
		return tmp >= str.End() ? String_t::zero : *tmp;
	}

	bool Finish(){ return iterator >= str.End(); }
	bool Match(Char_t _C, String_t::npos _Pos = 0){ return iterator+_Pos >= str.End() ? false : *(iterator+_Pos) == _C; }

	bool NextIfMatch(Char_t _C)
	{
		if (!Finish() && *iterator == _C)
		{
			Next();
			return true;
		}
		else
			return false;
	}

	String_t::iterator Iterator(){ return iterator; }
	String_t str;
private:
	String_t::iterator iterator;
};

RE_END_INTERNAL_NAMESPACE


#endif 
