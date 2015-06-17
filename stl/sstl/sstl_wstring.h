#pragma once
#ifndef __SSTL_WSTRING
#define __SSTL_WSTRING

#include"sstl_config.h"
#include"sstl_alloc.h"
#include"sstl_algobase.h"
#include<stddef.h>

SSTL_BEGIN_NAMESPACE

template <class CharType/*, class Traits = std::char_traits<CharType>*/, class Allocator = ALLOC(CharType)>
class BasicString
{
public:
	typedef CharType value_type;
	typedef Allocator allocator_type;
	typedef const value_type* const_iterator;
	typedef const value_type* const_pointer;
	typedef const value_type& const_reference;
	typedef ptrdiff_t difference_type;
	typedef const_pointer	iterator;
	typedef int	npos;
	typedef value_type& reference;
	typedef int	size_type;
	//typedef Traits traits_type;

private:
	
	allocator_type				alloc;
	
	mutable Ptr<value_type>		buffer;
	mutable size_type			start;
	mutable size_type			length;	//有效字符长度（不包括结尾字符）
	mutable size_type			realLength;

public:
	static BasicString Empty;	//空串
	static const value_type		zero = 0;

	static size_type CalculateLength(const value_type* buffer)
	{
		size_type result = 0;
		while (*buffer++)result++;
		return result;
	}
	static int Compare(const value_type* bufA, const BasicString& strB)
	{
		const value_type* bufB = strB.Buffer();
		const value_type* bufAOld = bufA;
		size_type length = strB.length;
		while (length-- && *bufA)
		{
			int diff = *bufA++ - *bufB++;
			if (diff != 0)
			{
				return diff;
			}
		};
		return CalculateLength(bufAOld) - strB.length;
	}
public:
	static int Compare(const BasicString& strA, const BasicString& strB)
	{
		const value_type* bufB = strB.Buffer();
		const value_type* bufA = strA.Buffer();

		size_type length = strA.length < strB.length ? strA.length : strB.length;
		while (length--)
		{
			int diff = *bufA++ - *bufB++;
			if (diff != 0)
			{
				return diff;
			}
		};
		return strA.length - strB.length;
	}

private:
	//取另一个String的某一部分来构造自己
	BasicString(const BasicString& src, npos _Index, size_type _Length) :buffer()
	{
		if (_Length == 0)
		{
			buffer = alloc.Allocate(1);
			*buffer = zero;
			start = 0;
			length = 0;
			realLength = 1;
		}
		else
		{
			buffer = src.buffer;
			start = _Index;
			length = _Length;
			realLength = src.realLength;
			//这里最后应该改成抛出异常。
			if (start + length >= realLength)length = realLength - start - 1;
		}

	}
	//把resource中的字符串的一部分用另一个字符串完全代替
	BasicString(const BasicString& des, const BasicString& src, npos _Index, size_type _Length)
	{
		if (_Index > (npos)des.length)_Index = des.length;
		if (_Index + _Length > (npos)des.length)_Length = 0;
		size_type resLength = des.length - _Length + src.length + 1;//别忘了结尾符号

		buffer = alloc.Allocate(resLength);
		memset(buffer.Obj(), 0, resLength*sizeof(value_type));
		length = resLength - 1;
		realLength = resLength;
		start = 0;
		//开始复制
		auto ite = copy(des.Begin(), des.Begin() + _Index, buffer.Obj());
		ite = copy(src.Begin(), src.End(), ite);
		copy(des.Begin() + _Index + _Length, des.End(), ite);
	}
public:
	BasicString() :BasicString(*this, 0, 0){} //默认构造函数产生一个1字符的String
	BasicString(const value_type* _buffer, size_type _length)
	{
		if (_length == 0)
		{
			buffer = alloc.Allocate(1);
			start = 0;
			length = 0;
			realLength = 1;
			*buffer = zero;
		}
		else
		{
			buffer = alloc.Allocate(_length + 1);
			start = 0;
			length = _length;
			realLength = _length + 1;
			memset(buffer.Obj(), 0, realLength*sizeof(value_type));
			copy(_buffer, _buffer + _length, this->buffer.Obj());
		}

	}
	BasicString(const value_type& _char) :BasicString(&_char, (size_type)1){}
	BasicString(const value_type* _buffer, bool _copy = true)
	{
		__LLIB_ASSERT(_buffer != nullptr);
		size_type len = CalculateLength(_buffer);
		if (len == 0)
		{
			buffer = alloc.Allocate(1);
			*buffer = zero;
			start = 0;
			length = 0;
			realLength = 1;
		}
		if (_copy)
		{
			buffer = alloc.Allocate(len + 1);
			memset(buffer.Obj(), 0, (len + 1)*sizeof(value_type));
			copy(_buffer, _buffer + len, buffer.Obj());
		}
		else
		{
			buffer = _buffer;
		}
		start = 0;
		length = len;
		realLength = len + 1;
	}
	BasicString(const BasicString& _string)
	{
		buffer = _string.buffer;
		start = _string.start;
		length = _string.length;
		realLength = _string.realLength;
	}
	iterator Begin()const{ return Buffer(); }
	iterator begin()const{ return Buffer(); }
	iterator End()const{ return Buffer() + length; }
	iterator end()const{ return Buffer() + length; }
	const_pointer Buffer()const
	{
		return buffer.Obj() + start;
	}
	BasicString Sub(npos _BeginIndex, npos _EndIndex)
	{
		return BasicString(*this, _BeginIndex, _EndIndex - _BeginIndex);
	}
	BasicString Sub(iterator _BeginIter, iterator _EndIter)
	{
		return Sub(npos(_BeginIter - buffer.Obj()), npos(_EndIter - buffer.Obj()));
	}
	CharType CharAt(npos _Pos)
	{
		return *(Buffer() + _Pos);
	}
	BasicString Cat(const BasicString& _Str, npos _Pos)const//把_Str连在这个字符串的_Pos处
	{
		return BasicString(*this, _Str, _Pos, 0);
	}
	void CStr(char* _Buf)const
	{
		for(auto& c : *this)
		{
			*_Buf++ = c;
		}
		*_Buf = '\0';
	}
	BasicString Cat(const BasicString& _Str)const
	{
		return Cat(_Str, Length());
	}
	iterator Find(value_type& _C)
	{
		iterator index = Begin();
		while (index!=End()&&*index != _C)index++;
		return index;
	}
	template<class T>
	int Split(T& _Container,value_type _Delimiter = ' ')
	{
		auto currStart = this->Begin();
		auto currEnd = this->Find(_Delimiter);
		int size = 0;
		while(1)
		{
			while(*currStart == _Delimiter && currStart != this->End())
				currStart++;
			if(currStart == this->End())return size;
			
			currEnd = currStart + 1;
			while(*currEnd != _Delimiter && currEnd != this->End())
				currEnd++;
			
			_Container.PushBack(this->Sub(currStart,currEnd));
			size++;
			currStart = currEnd;
		}
		return size;
	}
	BasicString Trim(value_type c = ' ')
	{
		auto start = this->Begin();
		auto end = this->End();
		while(*start == c && start != this->End())start++;
		
		while(*(end - 1) == c && end != start)end--;
		
		return this->Sub(start,end);
	}
	size_type Length()const
	{
		return length;
	}
	BasicString& operator=(const BasicString& string)
	{
		if (this != &string)
		{
			buffer = string.buffer;
			start = string.start;
			length = string.length;
			realLength = string.realLength;
		}
		return *this;
	}

	BasicString& operator+=(const BasicString& string)
	{
		return *this = *this + string;
	}

	BasicString operator+(const BasicString& string)const
	{
		return Cat(string);
	}

	bool operator==(const BasicString& string)const
	{
		return Compare(*this, string) == 0;
	}

	bool operator!=(const BasicString& string)const
	{
		return Compare(*this, string) != 0;
	}

	bool operator>(const BasicString& string)const
	{
		return Compare(*this, string) > 0;
	}

	bool operator>=(const BasicString& string)const
	{
		return Compare(*this, string) >= 0;
	}

	bool operator<(const BasicString& string)const
	{
		return Compare(*this, string) < 0;
	}

	bool operator<=(BasicString& string)const
	{
		return Compare(*this, string) <= 0;
	}

	bool operator==(const value_type* buffer)const
	{
		return Compare(buffer, *this) == 0;
	}

	bool operator!=(const value_type* buffer)const
	{
		return Compare(buffer, *this) != 0;
	}

	bool operator>(const value_type* buffer)const
	{
		return Compare(buffer, *this) < 0;
	}

	bool operator>=(const value_type* buffer)const
	{
		return Compare(buffer, *this) <= 0;
	}

	bool operator<(const value_type* buffer)const
	{
		return Compare(buffer, *this)>0;
	}

	bool operator<=(const value_type* buffer)const
	{
		return Compare(buffer, *this) >= 0;
	}


	friend bool operator<(const value_type* left, const BasicString& right)
	{
		return Compare(left, right)<0;
	}

	friend bool operator<=(const value_type* left, const BasicString& right)
	{
		return Compare(left, right) <= 0;
	}

	friend bool operator>(const value_type* left, const BasicString& right)
	{
		return Compare(left, right) > 0;
	}

	friend bool operator>=(const value_type* left, const BasicString& right)
	{
		return Compare(left, right) >= 0;
	}

	friend bool operator==(const value_type* left, const BasicString& right)
	{
		return Compare(left, right) == 0;
	}

	friend bool operator!=(const value_type* left, const BasicString& right)
	{
		return Compare(left, right) != 0;
	}

	friend BasicString operator+(const value_type* left, const BasicString& right)
	{
		return WString(left, false) + right;
	}



};


template <class CharType, class Allocator>
BasicString<CharType,  Allocator> BasicString<CharType, Allocator>::Empty = BasicString<CharType, Allocator>();

typedef BasicString<char,ALLOC(char)>		AString;
typedef BasicString<wchar_t,ALLOC(wchar_t)>		WString;
#ifndef __CROS

class WStringTool
{
public:
	static int					wtoi(const WString& string);

	static lrint64				wtoi64(const WString& string);

	static unsigned int			wtou(const WString& string);

	static lruint64				wtou64(const WString& string);

	static double				wtof(const WString& string);

	static WString				itow(int number);

	static WString				i64tow(lrint64 number);

	static WString				utow(unsigned int	 number);

	static WString				u64tow(lruint64 number);

	static WString				ftow(double number);
	
	static int					_atow(const char* a, wchar_t* w, int chars);
	static WString				atow(const AString& string);

	static WString				wlower(const WString& string);

	static WString				wupper(const WString& string);
};

class AStringTool
{
public:
	static int					atoi(const AString& string);
	static lrint64				atoi64(const AString& string);
	static unsigned int			atou(const AString& string);
	static lruint64				atou64(const AString& string);
	static double				atof(const AString& string);
	static AString				itoa(int number);
	static AString				i64toa(lrint64 number);
	static AString				utoa(unsigned int	 number);
	static AString				u64toa(lruint64 number);
	static AString				ftoa(double number);
	static int					_wtoa(const wchar_t* w, char* a, int chars);
	static AString				wtoa(const WString& string);
	
	static AString				alower(const AString& string);
	static AString				aupper(const AString& string);
};

class StringTool :public WStringTool, public AStringTool
{

};

#endif

SSTL_END_NAMESPACE
#endif
