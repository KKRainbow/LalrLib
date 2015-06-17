#include"basic.h"

#ifdef __LLIB_USE_INTERNAL_RTTI

__LLIB_BEGIN_NAMESPACE

Typeinfo::Typeinfo(const Typeinfo& info)
{
	classname = info.classname;
	father_get_type_info = info.father_get_type_info;
}
Typeinfo& Typeinfo::operator = (const Typeinfo& info)
{
	classname = info.classname; father_get_type_info = info.father_get_type_info;
	return *this;
};
Typeinfo::Typeinfo(const char* pclassname, Typeinfo(*pfather)()) :classname(pclassname), father_get_type_info(pfather)
{}
Typeinfo::~Typeinfo(){  };
int Typeinfo::operator == (const Typeinfo& info) const
{
	return strcmp(classname, info.classname) == 0;
}
int Typeinfo::operator != (const Typeinfo& info) const
{
	return !(*this == info);
}
const char* Typeinfo::name() const
{
	return classname;
}
const Typeinfo Typeinfo::get_father()const
{
	return father_get_type_info();
}
bool Typeinfo::before(Typeinfo type) const
{
	Typeinfo step(type);
	if (step == *this)
	{
		return true;
	}
	do
	{
		step = step.get_father();
		if (step == *this)
		{
			return true;
		}
	} while (step.get_father() != step);
	return false;
}

__LLIB_END_NAMESPACE

#endif