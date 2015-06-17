#pragma once
#include"sstl/sstl_rbtree.h"
#include"sstl/sstl_functors.h"

SSTL_BEGIN_NAMESPACE

template<class Key, class Compare = Greater<Key>
	, class Alloc = ALLOC(RBTreeNode<Key>)
	, class Container = RBTree<Key, Key, Identity<Key>, Compare, Alloc >>
class Set
{
public:
	typedef Key key_type;
	typedef Key value_type;
	typedef Compare key_compare;
	typedef Compare value_compare;
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

	Set() :rep(){}

	template<class InputIterator>
	Set(InputIterator first, InputIterator last) : Set()
	{
		Insert(first, last);
	}
	Set(const Set& x) :Set()
	{
		rep = x.rep;
	}

	Set& operator=(const Set& x)
	{
		rep = x.rep;
		return *this;
	}

	key_compare KeyComp()const
	{
		return rep.KeyComp();
	}

	value_compare ValueComp()const
	{
		return rep.KeyComp();
	}

	iterator Begin()const{ return rep.Begin(); }
	iterator begin()const{ return rep.Begin(); }
	iterator End()const{ return rep.End(); }
	iterator end()const{ return rep.End(); }

	bool Empty()const{ return rep.Empty(); }
	size_type Size()const { return rep.Size(); }
	void Swap(Set& x){ Set tmp = x; x = *this; *this = tmp; }

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
		rep.Delete(Find(x));
	}

	void Clear()
	{
		rep.Clear();
	}

	iterator Find(const key_type& x){ return rep.Find(x); }

	size_type Count(const key_type& x){ return rep.Count(x); }

	Pair<iterator, iterator> EqualRange(const key_type& x){ return rep.EqualRange(x); }

	iterator UpperBound(const key_type& x)
	{
		iterator res = Find(x);
		for (; x != *res; ++res);
		return res;
	}
	iterator LowerBound(const key_type& x)
	{
		iterator res = Find(x);
		for (; x != *res; --res);
		return ++res;
	}

	Set operator-(Set& x)const
	{
		Set res;
		for (auto b = this->Begin();b!=this->End();b++)
		{
			if (x.Find(*b) == x.End())
			{
				res.Insert(*b);
			}
		}
		return res;
	}
	Set operator+(const Set& x)const
	{
		Set res;
		for (auto b = this->Begin();b!=this->End();b++)
		{
			res.Insert(*b);
		}
		for (auto& n : x)
		{
			res.Insert(n);
		}
		return res;
	}
	Set& operator+=(const Set& x)
	{
		for (auto& n : x)
		{
			this->Insert(n);
		}
		return *this;
	}
};



template<class Key, class Compare = Greater<Key>
	, class Alloc = ALLOC(RBTreeNode<Key>)
	, class Container = RBTree<Key, Key, Identity<Key>, Compare, Alloc >>
class MultiSet :public Set<Key, Compare, Alloc, Container>
{
private:
	using Set<Key, Compare, Alloc, Container>::rep;
	using typename Set<Key, Compare, Alloc, Container>::iterator;
	using typename Set<Key, Compare, Alloc, Container>::const_reference;
	using typename Set<Key, Compare, Alloc, Container>::key_type;
public:
	template<class InputIterator>
	void Insert(InputIterator first, InputIterator last)
	{
		while (first != last)
		{
			rep.InsertEqual(*first);
			first++;
		}
	}
	iterator Insert(const_reference x){ return rep.InsertEqual(x); }
	template<class InputIterator>
	MultiSet(InputIterator first, InputIterator last) : Set<Key, Compare, Alloc, Container>()
	{
		Insert(first, last);
	}

	void Erase(const key_type& x)
	{
		auto a = this->Find(x);
		while (a != this->End())
		{
			a = this->Find(x);
		}
	}
};


SSTL_END_NAMESPACE
