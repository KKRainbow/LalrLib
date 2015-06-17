#ifndef RE_CONFIG_H
#define RE_CONFIG_H
#include"../basic/basic.h"
#include"../stl/sstl/sstl_wstring.h"

#define __REGEX_ENCODING_UTF16 1		//编码集影响自动机转换表
#define __REGEX_ENCODING_ASCII 2

//选择字符集
#define __REGEX_ENCODING			__REGEX_ENCODING_UTF16 //ASCII


#ifdef __LLIB_USE_NAMESPACE
#	define RE_NAMESPACE							regex
#	define RE_BEGIN_NAMESPACE					__LLIB_BEGIN_NAMESPACE namespace RE_NAMESPACE{
#	define RE_END_NAMESPACE						}}

#	define RE_INTERNAL_NAMESPACE				inner
#	define RE_BEGIN_INTERNAL_NAMESPACE			RE_BEGIN_NAMESPACE namespace RE_INTERNAL_NAMESPACE{
#	define RE_END_INTERNAL_NAMESPACE			}}}
#else
#	define RE_NAMESPACE				
#	define RE_BEGIN_NAMESPACE		
#	define RE_END_NAMESPACE			

#	define RE_INTERNAL_NAMESPACE				
#	define RE_BEGIN_INTERNAL_NAMESPACE			
#	define RE_END_INTERNAL_NAMESPACE			
#endif

//使用的字符集
RE_BEGIN_NAMESPACE

struct RE_UTF8{};
struct RE_ANSI{};

template<class T>
struct ReEncoding
{

};

template<>
struct ReEncoding<RE_UTF8>
{
	typedef RE_UTF8 Encoding;
	using String_t = sstl::WString;			//It just for fun to use this syntax.It also can be typedef
	using Char_t = wchar_t;
	const int CharSetSize = 65536;				//状态机字符集的大小
};

template<>
struct ReEncoding<RE_ANSI>
{
	typedef RE_ANSI Encoding;
	using String_t = sstl::AString;
	const int CharSetSize = 256;
	using Char_t = char;
};

#if  __REGEX_ENCODING==__REGEX_ENCODING_UTF16
using String_t = sstl::WString;			//It just for fun to use this syntax.It also can be typedef
using Char_t = wchar_t;
const int CharSetSize = 65536;				//状态机字符集的大小
#	define __R(c)	L##c
#elif  __REGEX_ENCODING==__REGEX_ENCODING_ASCII
using String_t = lr::sstl::AString;
const int CharSetSize = 256;
using Char_t = char;
#	define __R(c)	c

#endif //  __REGEX_ENCODING==__REGEX_ENCODING_UTF16



RE_END_NAMESPACE

#endif
