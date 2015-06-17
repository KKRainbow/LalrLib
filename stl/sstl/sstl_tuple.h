#ifndef SSTL_TUPLE_H
#define SSTL_TUPLE_H
#include"sstl_config.h"
#include"sstl_functors.h"

SSTL_BEGIN_NAMESPACE

template<class T1, class... Args>
class Tuple
{
public:
	
	typedef T1 return_type;
	typedef T1 tuple_head;
	typedef Tuple<Args...> tuple_tail;
protected:
	tuple_head head;
	tuple_tail tail;

private:
	Tuple(const tuple_head& t1, const tuple_tail& t_tail) :head(t1), tail(t_tail){}

public:
	Tuple() :head(), tail(){}

	template<class A, class ...B>
	Tuple(const A& t1,const B&... args) :head(t1), tail(args...){}

	//必须提供拷贝构造函数
	Tuple(const Tuple& t) :head(t.head),tail(t.tail){}

	Tuple& operator=(const Tuple& t)
	{
		if (this == &t)return *this;	//防止自己给自己赋值
		head = t.head;
		tail = t.tail;
		return *this;
	}
	tuple_head& Head(){ return head; }
	tuple_tail& Tail(){ return tail; }
};

template<class T1>
class Tuple<T1>
{
public:
	typedef T1 return_type;
	typedef T1 tuple_head;
	typedef T1 tuple_tail;
protected:
	tuple_head head;
public:
	Tuple() :head(){}

	template<class A>
	Tuple(const A& t1) :head(t1){}
	//必须提供拷贝构造函数
	Tuple(const Tuple& t) :head(t.head){}
	Tuple& operator=(const Tuple& t)
	{
		if (this == &t)return *this;	//防止自己给自己赋值
		head = t.head;
//		tail = t.tail;
		return *this;
	}
	tuple_head& Head(){ return head; }
	tuple_tail& Tail(){ return head; }
};
//Tuple Element////////////////////////////////////////////////
template<unsigned int Index, class T1, class...Args>
class TupleElement_Traits
{
public:
	typedef typename TupleElement_Traits<Index - 1, Args...>::type type;
};

template<class T1, class...Args>
class TupleElement_Traits<0, T1, Args...>
{
public:
	typedef T1 type;
};
//Get For Tuple/////////////////////////////////////////////////////

template<unsigned int Index,class T1,class...Args>
class TupleGetter
{
public:
	typedef typename TupleElement_Traits<Index, T1, Args...>::type ReturnType;
public:
	ReturnType& operator()(Tuple<T1, Args...>& mtuple)
	{
		typedef TupleGetter<Index-1, Args...> getter;
		return getter()(mtuple.Tail());
	}
};

template<class T1,class...Args>
class TupleGetter<0,T1,Args...>
{
public:
	typedef typename TupleElement_Traits<0, T1, Args...>::type ReturnType;
public:
	T1& operator()(Tuple<T1,Args...>& mtuple)
	{
		return mtuple.Head();
	}
};

template<unsigned int Index, class T1,class...Args>
inline typename TupleElement_Traits<Index, T1, Args...>::type& Get(Tuple<T1, Args...>& mtuple)
{
	typedef TupleGetter<Index, T1,Args...> getter;
	return getter()(mtuple);
}
/////////////////////////////////////////

template<class T1, class...Args>
inline Tuple<T1, Args...> MakeTuple(T1 t1, Args... args)
{
	return Tuple<T1, Args...>(t1, args...);
}



///下面是Pair的实现
template<class T1,class T2>
class Pair:public Tuple<T1,T2>
{
public:
	using Tuple<T1,T2>::Head;
	using Tuple<T1,T2>::Tail;
	typedef T1 first_type;
	typedef T2 second_type;
	first_type& first;
	second_type& second;
	Pair() :Tuple<T1, T2>(), first(Head()), second(Tail().Head()){  }

	//保证const转换
	template<typename A,typename B>
	Pair(const Pair<A,B>& p) :Tuple<T1, T2>(p.first, p.second), first(Head()), second(Tail().Head()){}
	//必须提供拷贝构造函数
	Pair(const Pair& p) :Tuple<T1,T2>(p.first,p.second), first(Head()), second(Tail().Head()){}

	Pair(const first_type& a, const second_type& b) :Tuple<T1, T2>(a, b), first(Head()), second(Tail().Head()){}

	//保证const转换
	Pair<T1, T2>& operator=(const Pair<T1, T2>& p)
	{
		if (this == &p)return *this;	//防止自己给自己赋值
		this->head = p.head;
		this->tail = p.tail;
		return *this;
	}
};

template<class T1, class T2>
Pair<T1, T2> MakePair(T1 a, T2 b)
{
	return Pair<T1, T2>(a, b);
}

//Pair的选择函数
template<class PairType>
class Select1st :public UnaryFunction<PairType, typename PairType::first_type>
{
private:
	typedef UnaryFunction<PairType, typename PairType::first_type> BaseType;
public:
	typename BaseType::result_type& operator()(const PairType& pair)
	{
		return pair.first;
	}
};

template<class PairType>
class Select2nd :public UnaryFunction<PairType, typename PairType::second_type>
{
private:
	typedef UnaryFunction<PairType, typename PairType::second_type> BaseType;
public:
	typename BaseType::result_type& operator()(const PairType& pair)
	{
		return pair.second;
	}
};

template<class Compare, unsigned int Index ,class T1, class...Args>	//通过比较Tuple的第index个值来确定tuple的大小关系
class TupleCompare :public BinaryFunction<Tuple<T1, Args...>, Tuple<T1, Args...>, bool>
{
private:
	typedef BinaryFunction<Tuple<T1, Args...>, Tuple<T1, Args...>, bool> BaseType;
public:
	bool operator()(const typename BaseType::argument1_type& x, const typename BaseType::argument2_type& y)const
	{
		return Compare()(Get<Index>(x), Get<Index>(y));
	}
};

template<class Compare, class T1, class T2>	//通过比较Tuple的第index个值来确定tuple的大小关系
class PairCompare1st :public BinaryFunction<Pair<T1, T2>, Pair<T1, T2>, bool>
{
private:
	typedef BinaryFunction<Pair<T1, T2>, Pair<T1, T2>, bool> BaseType;
public:
	bool operator()(const typename BaseType::argument1_type& x, const typename BaseType::argument2_type& y)const
	{
		return Compare()(x.first, y.first);
	}
};

template<class Compare, class T1, class T2>	//通过比较Tuple的第index个值来确定tuple的大小关系
class PairCompare2nd :public BinaryFunction<Pair<T1, T2>, Pair<T1, T2>, bool>
{
private:
	typedef BinaryFunction<Pair<T1, T2>, Pair<T1, T2>, bool> BaseType;
public:
	bool operator()(const typename BaseType::argument1_type& x, const typename BaseType::argument2_type& y)const
	{
		return Compare()(x.second, y.second);
	}
};

SSTL_END_NAMESPACE

#endif
