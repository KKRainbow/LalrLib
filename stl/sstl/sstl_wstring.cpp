#include"sstl_wstring.h"
#include <stdlib.h>
#ifdef __WIN32
#include<Windows.h>
#endif
#ifdef __LINUX
#include<stdio.h>
#endif

#ifndef __CROS

SSTL_BEGIN_NAMESPACE

int AStringTool::atoi(const AString& string)
{
	char* endptr = 0;
	return strtol(string.Buffer(), &endptr, 10);
}

int WStringTool::wtoi(const WString& string)
{
	wchar_t* endptr = 0;
	const wchar_t* buf = string.Buffer();
	return wcstol(buf, &endptr, 10);
}

lrint64 AStringTool::atoi64(const AString& string)
{
	char* endptr = 0;
#ifdef __WIN32
	return _strtoi64(string.Buffer(), &endptr, 10);
#elif defined(__LINUX)
	return strtoll(string.Buffer(), &endptr, 10);
#endif
}

lrint64 WStringTool::wtoi64(const WString& string)
{
	wchar_t* endptr = 0;
#ifdef __WIN32
	return _wcstoi64(string.Buffer(), &endptr, 10);
#elif defined(__LINUX)
	return wcstoll(string.Buffer(), &endptr, 10);
#endif
}

unsigned int AStringTool::atou(const AString& string)
{
	char* endptr = 0;
	return strtoul(string.Buffer(), &endptr, 10);
}

unsigned int WStringTool::wtou(const WString& string)
{
	wchar_t* endptr = 0;
	return wcstoul(string.Buffer(), &endptr, 10);
}

lruint64 AStringTool::atou64(const AString& string)
{
	char* endptr = 0;
#ifdef __WIN32
	return _strtoui64(string.Buffer(), &endptr, 10);
#elif defined(__LINUX)
	return strtoul(string.Buffer(), &endptr, 10);
#endif
}

lruint64 WStringTool::wtou64(const WString& string)
{
	wchar_t* endptr = 0;
#ifdef __WIN32
	return _wcstoui64(string.Buffer(), &endptr, 10);
#elif defined(__LINUX)
	return wcstoul(string.Buffer(), &endptr, 10);
#endif
}

double AStringTool::atof(const AString& string)
{
	char* endptr = 0;
	return strtod(string.Buffer(), &endptr);
}

double WStringTool::wtof(const WString& string)
{
	wchar_t* endptr = 0;
	return wcstod(string.Buffer(), &endptr);
}

AString AStringTool::itoa(int number)
{
	char buffer[100];
#ifdef __WIN32
	_itoa_s(number, buffer, sizeof(buffer) / sizeof(*buffer), 10);
#elif defined(__LINUX)
	sprintf(buffer,"%d",number);
#endif
	return buffer;
}

WString WStringTool::itow(int number)
{
	wchar_t buffer[100];
#ifdef __WIN32
	_itow_s(number, buffer, sizeof(buffer) / sizeof(*buffer), 10);
#elif defined(__LINUX)
	char bufAnsi[100];
	sprintf(bufAnsi,"%d",number);
	mbstowcs(buffer,bufAnsi,sizeof(buffer));
#endif
	return buffer;
}

AString AStringTool::i64toa(lrint64 number)
{
	char buffer[100];
#ifdef __WIN32
	_i64toa_s(number, buffer, sizeof(buffer) / sizeof(*buffer), 10);
#elif defined(__LINUX)
	sprintf(buffer,"%lld",number);
#endif
	return buffer;
}

WString WStringTool::i64tow(lrint64 number)
{
	wchar_t buffer[100];
#ifdef __WIN32
	_i64tow_s(number, buffer, sizeof(buffer) / sizeof(*buffer), 10);
#elif defined(__LINUX)
	char bufAnsi[100];
	sprintf(bufAnsi,"%lld",number);
	mbstowcs(buffer,bufAnsi,sizeof(buffer));
#endif
	return buffer;
}

AString AStringTool::utoa(unsigned int number)
{
	char buffer[100];
#ifdef __WIN32
	_ui64toa_s(number, buffer, sizeof(buffer) / sizeof(*buffer), 10);
#elif defined(__LINUX)
	sprintf(buffer,"%u",number);
#endif
	return buffer;
}

WString WStringTool::utow(unsigned int number)
{
	wchar_t buffer[100];
#ifdef __WIN32
	_i64tow_s(number, buffer, sizeof(buffer) / sizeof(*buffer), 10);
#elif defined(__LINUX)
	char bufAnsi[100];
	sprintf(bufAnsi,"%u",number);
	mbstowcs(buffer,bufAnsi,sizeof(buffer));
#endif
	return buffer;
}

AString AStringTool::u64toa(lruint64 number)
{
	char buffer[100];
#ifdef __WIN32
	_ui64toa_s(number, buffer, sizeof(buffer) / sizeof(*buffer), 10);
#elif defined(__LINUX)
	sprintf(buffer,"%llu",number);
#endif
	return buffer;
}

WString WStringTool::u64tow(lruint64 number)
{
	wchar_t buffer[100];
#ifdef __WIN32
	_i64tow_s(number, buffer, sizeof(buffer) / sizeof(*buffer), 10);
#elif defined(__LINUX)
	char bufAnsi[100];
	sprintf(bufAnsi,"%llu",number);
	mbstowcs(buffer,bufAnsi,sizeof(buffer));
#endif
	return buffer;
}

AString AStringTool::ftoa(double number)
{
	char buffer[100];
#ifdef __WIN32
	_gcvt_s(buffer, 100, number, 30);
#elif defined(__LINUX)
	gcvt(number,10,buffer);
#endif
	int len = (int)strlen(buffer);
	if (buffer[len - 1] == '.')
	{
		buffer[len - 1] = '\0';
	}
	return buffer;
}

WString WStringTool::ftow(double number)
{
	return atow(AStringTool::ftoa(number));
}

int AStringTool::_wtoa(const wchar_t* w, char* a, int chars)
{
#ifdef __WIN32
	return WideCharToMultiByte(CP_THREAD_ACP, 0, w, -1, a, (int)(a ? chars : 0), 0, 0);
#elif defined(__LINUX)
	return wcstombs(a,w,chars);
#endif
}

AString AStringTool::wtoa(const WString& string)
{
	int len = _wtoa(string.Buffer(), 0, 0);
	char* buffer = new char[len];
	_wtoa(string.Buffer(), buffer, (int)len);
	AString s = buffer;
	delete[] buffer;
	return s;
}

int WStringTool::_atow(const char* a, wchar_t* w, int chars)
{
#ifdef __WIN32
	return MultiByteToWideChar(CP_THREAD_ACP, 0, a, -1, w, (int)(w ? chars : 0));
#elif defined(__LINUX)
	return	mbstowcs(w,a,chars);
#endif
}

WString WStringTool::atow(const AString& string)
{
	int len = _atow(string.Buffer(), 0, 0);
	wchar_t* buffer = new wchar_t[len];
	_atow(string.Buffer(), buffer, (int)len);
	WString s = buffer;
	delete[] buffer;
	return s;
}

AString AStringTool::alower(const AString& string)
{
	AString result = string.Buffer();
#ifdef __WIN32
	_strlwr_s((char*)result.Buffer(), result.Length() + 1);
#elif defined(__LINUX)
	char* res = const_cast<char*>(result.Begin());
	const char* orig = string.Buffer();
	for ( ; *orig != '\0'; res++,orig++)
		*res = tolower(*orig);
#endif
	return result;
}

WString WStringTool::wlower(const WString& string)
{
	WString result = string.Buffer();
#ifdef __WIN32
	_wcslwr_s((wchar_t*)result.Buffer(), result.Length() + 1);
#elif defined(__LINUX)
	wchar_t* res = const_cast<wchar_t*>(result.Begin());
	const wchar_t* orig = string.Buffer();
	for ( ; *orig != '\0'; res++,orig++)
		*res = tolower(*orig);
#endif
	return result;
}

AString AStringTool::aupper(const AString& string)
{
	AString result = string.Buffer();
#ifdef __WIN32
	_strupr_s((char*)result.Buffer(), result.Length() + 1);
#elif defined(__LINUX)
	char* res = const_cast<char*>(result.Begin());
	const char* orig = string.Buffer();
	for ( ; *orig != '\0'; res++,orig++)
		*res = toupper(*orig);
#endif

	return result;
}

WString WStringTool::wupper(const WString& string)
{
	WString result = string.Buffer();
#ifdef __WIN32
	_wcsupr_s((wchar_t*)result.Buffer(), result.Length() + 1);
#elif defined(__LINUX)
	wchar_t* res = const_cast<wchar_t*>(result.Begin());
	const wchar_t* orig = string.Buffer();
	for ( ; *orig != '\0'; res++,orig++)
		*res = toupper(*orig);
#endif
	return result;
}


SSTL_END_NAMESPACE

#endif
