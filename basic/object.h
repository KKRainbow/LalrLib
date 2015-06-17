#pragma once
#ifndef _LLIB_BASIC_OBJECT
#define _LLIB_BASIC_OBJECT
#include"basic.h"
__LLIB_BEGIN_NAMESPACE

class Object		//超类
{
	__LLIB_CLASS_DECLARE(Object, Object)
public:
	virtual ~Object();
	Object(){};
};

template<class T>
class ObjectBox:
	public Object
{
private:
	T obj;
public:
	ObjectBox(T mObj)
	{
		obj = mObj;
	}
	T UnBox()
	{
		return obj;
	}
	virtual ~ObjectBox()
	{

	}
};

//不可复制的类
class NoCopyable
{
private:
	NoCopyable(const NoCopyable&);						//不用实现，因为需要调用它就是一个错误~
	NoCopyable& operator =(const NoCopyable&);
public:
	NoCopyable(){};
};

//接口类
class Interface
{
public:
	virtual ~Interface() = 0;
};

struct TrueType
{};
struct FalseType
{};

template<class T>
struct TypeTraits
{
	typedef FalseType is_POD_type;
	typedef FalseType has_trival_destructor;
	typedef FalseType has_trival_assignment_operator;
	typedef FalseType has_trival_copy_constructor;
	typedef FalseType has_trival_default_constructor;
};

template<> 
struct TypeTraits<bool>
{
	typedef TrueType is_POD_type;
	typedef TrueType has_trival_destructor;
	typedef TrueType has_trival_assignment_operator;
	typedef TrueType has_trival_copy_constructor;
	typedef TrueType has_trival_default_constructor;
};

template<> 
struct TypeTraits<char>
{
	typedef TrueType is_POD_type;
	typedef TrueType has_trival_destructor;
	typedef TrueType has_trival_assignment_operator;
	typedef TrueType has_trival_copy_constructor;
	typedef TrueType has_trival_default_constructor;
};

template<> 
struct TypeTraits<short>
{
	typedef TrueType is_POD_type;
	typedef TrueType has_trival_destructor;
	typedef TrueType has_trival_assignment_operator;
	typedef TrueType has_trival_copy_constructor;
	typedef TrueType has_trival_default_constructor;
};

template<> 
struct TypeTraits<int>
{
	typedef TrueType is_POD_type;
	typedef TrueType has_trival_destructor;
	typedef TrueType has_trival_assignment_operator;
	typedef TrueType has_trival_copy_constructor;
	typedef TrueType has_trival_default_constructor;
};

template<>
struct TypeTraits<long>
{
	typedef TrueType is_POD_type;
	typedef TrueType has_trival_destructor;
	typedef TrueType has_trival_assignment_operator;
	typedef TrueType has_trival_copy_constructor;
	typedef TrueType has_trival_default_constructor;
};
template<> 
struct TypeTraits<double>
{
	typedef TrueType is_POD_type;
	typedef TrueType has_trival_destructor;
	typedef TrueType has_trival_assignment_operator;
	typedef TrueType has_trival_copy_constructor;
	typedef TrueType has_trival_default_constructor;
};
template<> 
struct TypeTraits<long long>
{
	typedef TrueType is_POD_type;
	typedef TrueType has_trival_destructor;
	typedef TrueType has_trival_assignment_operator;
	typedef TrueType has_trival_copy_constructor;
	typedef TrueType has_trival_default_constructor;
};
template<> 
struct TypeTraits<float>
{
	typedef TrueType is_POD_type;
	typedef TrueType has_trival_destructor;
	typedef TrueType has_trival_assignment_operator;
	typedef TrueType has_trival_copy_constructor;
	typedef TrueType has_trival_default_constructor;
};
template<> 
struct TypeTraits<long double>
{
	typedef TrueType is_POD_type;
	typedef TrueType has_trival_destructor;
	typedef TrueType has_trival_assignment_operator;
	typedef TrueType has_trival_copy_constructor;
	typedef TrueType has_trival_default_constructor;
};

template<> 
struct TypeTraits<unsigned char>
{
	typedef TrueType is_POD_type;
	typedef TrueType has_trival_destructor;
	typedef TrueType has_trival_assignment_operator;
	typedef TrueType has_trival_copy_constructor;
	typedef TrueType has_trival_default_constructor;
};

template<> 
struct TypeTraits<unsigned short>{
	typedef TrueType is_POD_type;
	typedef TrueType has_trival_destructor;
	typedef TrueType has_trival_assignment_operator;
	typedef TrueType has_trival_copy_constructor;
	typedef TrueType has_trival_default_constructor;
};
template<> 
struct TypeTraits<unsigned int>{
	typedef TrueType is_POD_type;
	typedef TrueType has_trival_destructor;
	typedef TrueType has_trival_assignment_operator;
	typedef TrueType has_trival_copy_constructor;
	typedef TrueType has_trival_default_constructor;
};
template<> 
struct TypeTraits<unsigned long>{
	typedef TrueType is_POD_type;
	typedef TrueType has_trival_destructor;
	typedef TrueType has_trival_assignment_operator;
	typedef TrueType has_trival_copy_constructor;
	typedef TrueType has_trival_default_constructor;
};
template<>
struct TypeTraits<unsigned long long>{
	typedef TrueType is_POD_type;
	typedef TrueType has_trival_destructor;
	typedef TrueType has_trival_assignment_operator;
	typedef TrueType has_trival_copy_constructor;
	typedef TrueType has_trival_default_constructor;
};

template<class T> 
struct TypeTraits<T*>{
	typedef TrueType is_POD_type;
	typedef TrueType has_trival_destructor;
	typedef TrueType has_trival_assignment_operator;
	typedef TrueType has_trival_copy_constructor;
	typedef TrueType has_trival_default_constructor;
};

template<typename T>
struct TypeTraits<const T>{
	typedef typename  TypeTraits<T>::is_POD_type is_POD_type;
	typedef typename  TypeTraits<T>::has_trival_destructor has_trival_destructor;
	typedef typename  TypeTraits<T>::has_trival_assignment_operator has_trival_assignment_operator;
	typedef typename  TypeTraits<T>::has_trival_copy_constructor has_trival_copy_constructor;
	typedef typename TypeTraits<T>::has_trival_default_constructor has_trival_default_constructor;
};
template<typename T>
struct TypeTraits<volatile T>
{ 
	typedef typename  TypeTraits<T>::is_POD_type is_POD_type;
	typedef typename  TypeTraits<T>::has_trival_destructor has_trival_destructor;
	typedef typename  TypeTraits<T>::has_trival_assignment_operator has_trival_assignment_operator;
	typedef typename  TypeTraits<T>::has_trival_copy_constructor has_trival_copy_constructor;
	typedef typename  TypeTraits<T>::has_trival_default_constructor has_trival_default_constructor;
};
template<typename T>
struct TypeTraits<const volatile T>
{
	typedef typename  TypeTraits<T>::is_POD_type is_POD_type;
	typedef typename  TypeTraits<T>::has_trival_destructor has_trival_destructor;
	typedef typename  TypeTraits<T>::has_trival_assignment_operator has_trival_assignment_operator;
	typedef typename  TypeTraits<T>::has_trival_copy_constructor has_trival_copy_constructor;
	typedef typename  TypeTraits<T>::has_trival_default_constructor has_trival_default_constructor;
};

////////////////////////////////////////辅助函数/////////////
template<class T>
static inline typename TypeTraits<T>::is_POD_type _type_tarits_isPOD(const T&)
{
	return typename TypeTraits<T>::is_POD_type();
}

template<class T>
static inline typename TypeTraits<T>::has_trival_destructor _type_tarits_has_trival_dtor(const T&)
{
	return typename TypeTraits<T>::has_trival_destructor();
}

template<class T>
static inline typename TypeTraits<T>::has_trival_default_constructor _type_tarits_has_trival_ctor(const T&)
{
	return typename TypeTraits<T>::has_trival_default_constructor();
}

template<class T>
static inline typename TypeTraits<T>::has_trival_assignment_operator _type_tarits_has_trival_assignment(const T&)
{
	return typename TypeTraits<T>::has_trival_assignment_operator();
}

///////////////////////////////////////////////////////
__LLIB_END_NAMESPACE
#endif
