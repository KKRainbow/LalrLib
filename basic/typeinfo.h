#pragma once
#ifndef __LLIB_BASIC_TYPEINFO
#define __LLIB_BASIC_TYPEINFO

#ifdef __LLIB_USE_INTERNAL_RTTI
#include"string.h"

namespace lr{
	class Object;
	class Typeinfo
	{
	private:
		const char* classname;
		Typeinfo (*father_get_type_info)();
		
	public:
		Typeinfo(const Typeinfo& info);
		Typeinfo& operator=(const Typeinfo& info);
		Typeinfo(const char* pclassname, Typeinfo(*pfather)());
		virtual ~Typeinfo();
		int operator==(const Typeinfo& info) const;
		int operator!=(const Typeinfo& info) const;
		const char* name() const;
		const Typeinfo get_father()const;
		bool before(Typeinfo type) const;
	};

};
#endif
#endif
