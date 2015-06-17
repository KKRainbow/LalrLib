//º¯Êý¶ÔÏó 
#ifndef SSTL_FUNCTOR_H
#define SSTL_FUNCTOR_H
#include"sstl_config.h"

SSTL_BEGIN_NAMESPACE


template<class Arg, class Result>
class UnaryFunction
{
public:
	typedef Arg argument_type;
	typedef Result result_type;
};

template<class Arg1, class Arg2, class Result>
class BinaryFunction
{
public:
	typedef Arg1 argument1_type;
	typedef Arg2 argument2_type;
	typedef Result result_type;
};

template<class T>
class Greater :public BinaryFunction<T, T, bool>
{
public:
	bool operator()(const T& x, const T& y)const { return x > y; }
};

template<class T>
class Less :public BinaryFunction<T, T, bool>
{
public:
	bool operator()(const T& x, const T& y)const { return x < y; }
};

template<class T>
class GreaterEqual :public BinaryFunction<T, T, bool>
{
public:
	bool operator()(const T& x, const T& y)const { return x >= y; }
};

template<class T>
class LessEqual :public BinaryFunction<T, T, bool>
{
public:
	bool operator()(const T& x, const T& y)const { return x <= y; }
};

template<class T>
class Equal :public BinaryFunction<T, T, bool>
{
public:
	bool operator()(const T& x, const T& y)const { return x == y; }
};

template<class T>
class NotEqual :public BinaryFunction<T, T, bool>
{
public:
	bool operator()(const T& x, const T& y)const { return x != y; }
};

template<class T>
class AlwaysTrue :public BinaryFunction<T, T, bool>
{
public:
	bool operator()(const T& x, const T& y)const { return true; }
};

template<class T>
class AlwaysFalse :public BinaryFunction<T, T, bool>
{
public:
	bool operator()(const T& x, const T& y)const { return false; }
};


template<class T>
class Identity :public UnaryFunction<T, T>
{
public:
	const T& operator()(const T& x)const { return x; }
};


template<class Arg1, class  Arg2, class  Result>
class PointerToBinaryFunc :public BinaryFunction<Arg1, Arg2, Result>
{
protected:
	typedef Result(*Myfunc)(Arg1, Arg2);
	Myfunc func = nullptr;
public:
	Result operator()(Arg1 arg1, Arg2 arg2)
	{
		return func(arg1, arg2);
	}
	PointerToBinaryFunc(Myfunc _Func) :func(_Func){}
	PointerToBinaryFunc() = default;
	PointerToBinaryFunc& operator=(Myfunc _Func)
	{
		func = _Func;
		return *this;
	}
};

template<class Arg1, class  Result>
class PointerToUnaryFunc :public UnaryFunction<Arg1, Result>
{
protected:
#ifdef __WIN32
	typedef Result(_cdecl*Myfunc)(Arg1);
#elif defined(__LINUX)||defined(__CROS)
	typedef Result(*Myfunc)(Arg1);
#endif
public:
	Myfunc func = nullptr;
public:
	Result operator()(Arg1 arg1)
	{
		return func(arg1);
	}
	PointerToUnaryFunc(Myfunc _Func) :func(_Func){}
	PointerToUnaryFunc() = default;
	PointerToUnaryFunc& operator=(Myfunc _Func)
	{
		func = _Func;
		return *this;
	}
};

template<class Result, class...  Args>
class PointerToFunc
{
public:
	typedef Result result_type;
protected:
	typedef Result(*Myfunc)(Args...);
	Myfunc func = nullptr;
public:
	Result operator()(Args... arg)
	{
		return func(arg...);
	}
	PointerToFunc(Myfunc _Func) :func(_Func){}
	PointerToFunc() = default;
	PointerToFunc& operator=(Myfunc _Func)
	{
		func = _Func;
		return *this;
	}
};

template<class T>
class Ref
{
private:
	T& ref;
public:
	Ref():ref(){}
	Ref(T& x) :ref(x){}
	T& Get(){ return ref; }
};

SSTL_END_NAMESPACE

#endif
