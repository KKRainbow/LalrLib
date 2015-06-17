#ifndef SSTL_VECTOR_H
#define SSTL_VECTOR_H

#include"sstl_config.h"
#include"sstl_iterator.h"
#include"sstl_alloc.h"
#include"sstl_algobase.h"
#include<stddef.h>

SSTL_BEGIN_NAMESPACE

template<class T, class Alloc = ALLOC(T)>
class Vector :public Object
{
	__LLIB_CLASS_DECLARE(Vector, Object);
public:
	typedef T value_type;
	typedef value_type* pointer;
	typedef value_type& reference;
	typedef const T* const_pointer;
	typedef const T& const_reference;

	typedef size_t size_type;
	typedef ptrdiff_t difference_type;

	typedef value_type* iterator;
	typedef Alloc allocator_type;

protected:
	iterator start;
	iterator finish;
	iterator end_of_storage;

	allocator_type alloc;

	void Deallocate()
	{
		alloc.Destroy(start, finish);
		alloc.Deallocate(start,sizeof(value_type));
	}


public:
	iterator Begin()const{ return start; }
	iterator End()const{ return finish; }
	size_type Size() const{ return finish - start; }
	size_type Capacity() const{ return end_of_storage - start; }
	bool Empty() { return Begin() == End(); }
	reference operator [](size_type n)const{ return *(Begin() + n); }

	void Insert(iterator position, size_type n, const_reference x);
	void Insert(iterator position, const_reference x)
	{
		Insert(position,1, x);
	}
	void PushBack(const_reference x)
	{
		Insert(End(),1, x);
	}
	void PopBack()
	{
		--finish;
		alloc.Destroy(finish);
	}
	reference Front(){ return *Begin(); }
	reference Back(){ return *(End() - 1); }
	iterator Erase(iterator position)
	{
		sstl::copy(position + 1, End(), position);
		finish--;
		alloc.Destroy(finish);
		return position;
	}
	iterator Erase(iterator from, iterator to)
	{
		if (to <= from)return from;
		difference_type dif = to - from;
		to = sstl::copy(to, End(), from);
		alloc.Destroy(to,finish);
		finish = finish - dif;
		return from;
	}
	void Clear()
	{
		Erase(Begin(), End());
	}
	void Resize(size_type new_size, const_reference x)
	{
		if (new_size < Size())
		{
			Erase(Begin() + new_size,End());
		}
		else
		{
			Insert(finish, new_size - Size(), x);
		}
	}
	void Resize(size_type new_size)
	{
		Resize(new_size, T());
	}
public: //构造函数
	~Vector()
	{
		Deallocate();
	}
	explicit Vector(const allocator_type& ma = allocator_type()) :alloc(ma)
	{
		start = finish = end_of_storage = nullptr;
	}
	explicit Vector(const size_type n) :Vector()
	{
		Resize(n);
	}
	Vector(const size_type n, const_reference val, const allocator_type& alloc = allocator_type()) :Vector(alloc)
	{
		Resize(n, val);
	}
	template <class InputIterator>
	Vector(InputIterator first, InputIterator last, const allocator_type& alloc = allocator_type()) : Vector(alloc)
	{
		for (; first != last; first++)
		{
			PushBack(*first);
		}
	}
	Vector(const Vector& x) :Vector(x.alloc)
	{
		start = alloc.Allocate(x.Size());
		_uninitialized_fill_n(start, x.Size(), T());
		end_of_storage = finish = sstl::copy(x.start, x.finish, start);
	}
	Vector(const Vector& x, const allocator_type& ma) :Vector(x)
	{
		alloc = ma;
	}
	//Vector(Vector&& x);
	//Vector(Vector&& x, const allocator_type& alloc);
	//Vector(initializer_list<value_type> il, const allocator_type& alloc = allocator_type());
	Vector<T>& operator = (const Vector<T>& x)
	{
		if (this == &x)return *this;	//防止自己给自己赋值
		Deallocate();
		start = alloc.Allocate(x.Size());
		_uninitialized_fill_n(start, x.Size(), T());
		end_of_storage = finish = sstl::copy(x.start, x.finish, start);

		alloc = x.alloc;

		return *this;
	}

	void Reserve(size_type n);

	iterator begin()const{ return start; }
	iterator end()const{ return finish; }
};
template<class T, class Alloc>
void Vector<T, Alloc>::Insert(iterator position, size_type n, const_reference x)
{
	if (n <= 0)
	{
		return;
	}
	if (Capacity() - Size() >= n) //剩余空间足够
	{
		iterator tmp = position;
		
		sstl::copy(position, finish, position + 1);
		_uninitialized_fill_n(position, n, x);
		finish += n;
	}
	else //空间不足
	{
		const size_type old_size = Size();
		const size_type new_size = max(Size() * 2, Size() + n);
		const iterator new_start = alloc.Allocate(new_size);
		iterator new_finish = new_start;

		new_finish = _uninitialized_copy(start, position, new_start);


		_uninitialized_fill_n(new_finish, n, x);
		new_finish += n;

		new_finish = _uninitialized_copy(position, finish, new_finish);

		//_uninitialized_fill(new_finish, new_start+new_size, T());
		//此处可能抛出异常

		Deallocate();

		start = new_start;
		finish = new_finish;
		end_of_storage = new_start + new_size;
	}
}


template<class T, class Alloc>
void Vector<T, Alloc>::Reserve(size_type n)
{
	size_type size = Size();
	if(n<size)return;

	iterator new_start = alloc.Allocate(n);
	_uninitialized_copy(start,finish,new_start);

	Deallocate();

	start = new_start;
	end_of_storage = start+n;
	finish = start+size;
}


SSTL_END_NAMESPACE
#endif
