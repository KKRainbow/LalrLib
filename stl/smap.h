#pragma once
#include"sstl/sstl_rbtree.h"
#include"sstl/sstl_functors.h"
#include"sstl/sstl_alloc.h"

SSTL_BEGIN_NAMESPACE



template<class Key,class Value, class Compare = Greater<Key>
	, class Alloc = Allocator<RBTreeNode<Pair<Key, Value>>>
	, class Container = RBTree<Key, Pair<Key, Value>, Select1st<Pair<Key, Value>>, Compare, Alloc >>
class Map
{
public:
	typedef Key key_type;
	typedef Value data_type;
	typedef Value mapped_type;
	typedef Pair<const Key, Value> value_type;
	typedef Compare key_compare;
	typedef PairCompare1st<key_compare, typename value_type::first_type, typename value_type::second_type> value_compare;

protected:
	typedef Container rep_type;
	rep_type rep; //Representation
public:
	typedef typename rep_type::const_pointer pointer;
	typedef typename rep_type::const_pointer const_pointer;
	typedef typename rep_type::reference reference;
	typedef typename rep_type::const_reference const_reference;
	typedef typename rep_type::iterator iterator;
	typedef const typename rep_type::iterator const_iterator;

	typedef typename rep_type::size_type size_type;
	typedef typename rep_type::difference_type difference_type;

	//Ctors
	Map() :rep(){}

	template<class InputIterator>
	Map(InputIterator first, InputIterator last) : Map()
	{
		Insert(first, last);
	}
	Map(const Map& x) :Map()
	{
		rep = x.rep;
	}
	Map& operator=(const Map& x)
	{
		rep = x.rep;
		return *this;
	}

	//Accessors
	key_compare KeyComp()const
	{
		return rep.KeyComp();
	}

	value_compare ValueComp()const
	{
		return value_compare();
	}

	iterator Begin()const{ return rep.Begin(); }
	iterator begin()const{ return rep.Begin(); }
	iterator End()const{ return rep.End(); }
	iterator end()const{ return rep.End(); }

	bool Empty()const{ return rep.Empty(); }
	size_type Size()const { return rep.Size(); }
	void Swap(Map& x){ Map tmp = x; x = *this; *this = tmp; }

	Pair<iterator, bool> Insert(const_reference x){ return rep.InsertUnique(x); }

	template<class InputIterator>
	void Insert(InputIterator first, InputIterator last)
	{
		while (first != last)
		{
			rep.InsertUnique(*first);
			first++;
		}
	}

	void Erase(iterator pos)
	{
		rep.Delete(pos);
	}

	void Erase(const key_type& x)
	{
		rep.Delete(rep.Find(x));
	}

	void Clear()
	{
		rep.Clear();
	}

	iterator Find(const key_type& x)const{ return rep.Find(x); }

	size_type Count(const key_type& x){ return rep.Count(x); }

	Pair<iterator, iterator> EqualRange(const key_type& x){ return rep.EqualRange(x); }

	iterator UpperBound(const key_type& x)
	{
		iterator res = Find(x);
		for (; x != (*res).first; ++res);
		return res;
	}
	iterator LowerBound(const key_type& x)
	{
		iterator res = Find(x);
		for (; x != (*res).first; --res);
		return ++res;
	}

	Value& operator[](const key_type& x)
	{
		return (rep[x]).second;
	}
};



template<class Key, class Value, class Compare = GreaterEqual<Key>
	, class Alloc = Allocator<RBTreeNode<Pair<Key, Value>>>
		, class Container = RBTree<Key, Pair<Key, Value>, Select1st<Pair<Key, Value>>, Compare, Alloc >>
class MultiMap :public Map<Key, Value, Compare, Alloc, Container>
{
private:
	typedef Map<Key, Value, Compare, Alloc, Container> MapBase;
public:
	typedef Key key_type;
	typedef Value data_type;
	typedef Value mapped_type;
	typedef Pair<const Key, Value> value_type;
	typedef Compare key_compare;
	typedef PairCompare1st<key_compare, typename value_type::first_type, typename value_type::second_type> value_compare;

	using Map<Key, Value, Compare, Alloc, Container>::Erase;
	using typename Map<Key, Value, Compare, Alloc, Container>::rep_type;
	typedef typename rep_type::const_pointer pointer;
	typedef typename rep_type::const_pointer const_pointer;
	typedef typename rep_type::reference reference;
	typedef typename rep_type::const_reference const_reference;
	typedef typename rep_type::iterator iterator;
	typedef const typename rep_type::iterator const_iterator;

	typedef typename rep_type::size_type size_type;
	typedef typename rep_type::difference_type difference_type;
	template<class InputIterator>
	void Insert(InputIterator first, InputIterator last)
	{
		while (first != last)
		{
			this->rep.InsertEqual(*first);
			first++;
		}
	}
	typename MapBase::iterator Insert(typename MapBase::const_reference x){ return this->rep.InsertEqual(x); }

	template<class InputIterator>
	MultiMap(InputIterator first, InputIterator last) : Map<Key, Value, Compare, Alloc, Container>()
	{
		Insert(first, last);
	}

	MultiMap() :Map<Key, Value, Compare, Alloc, Container>(){};

	void Erase(const typename MapBase::key_type& x)
	{
		typename MapBase::iterator a;
		while ((a = Find(x)) != this->End())Erase(a);
	}

};




SSTL_END_NAMESPACE
