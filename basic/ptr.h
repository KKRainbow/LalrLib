#pragma once
#ifndef __LLIB_BASIC_PTR
#define __LLIB_BASIC_PTR

#include"object.h"
__LLIB_BEGIN_NAMESPACE

template<class T>
class Ptr : public Object
{
	__LLIB_CLASS_DECLARE(Ptr, Object)
public:
	typedef T value_type;
	typedef value_type* pointer;
	typedef value_type& reference;
protected:
	typedef unsigned int counter_type;
	mutable counter_type* counter = nullptr;
	mutable pointer objptr = nullptr;
	void Inc()
	{
		__LLIB_ASSERT(!((counter == nullptr&&objptr != nullptr) || (counter&&*counter == 0 && objptr != nullptr)));
		if (counter)
		{
			(*counter)++;
		}
	}
	void Dec()
	{
		//这种情况不应该出现
		__LLIB_ASSERT(!((counter == nullptr&&objptr != nullptr) || (counter&&*counter == 0 && objptr != nullptr)));
		if (counter)
		{	
			if (--(*counter) == 0)
			{
				delete objptr;
				delete counter;

				counter = nullptr;
				objptr = nullptr;
			}
		}
		else
		{
			objptr = nullptr;
		}
	}
	counter_type* Counter() const
	{
		return counter;
	}
	pointer Pointer() const
	{
		return objptr;
	}

public:
	explicit Ptr(counter_type* c, T* ptr) :counter(c), objptr(ptr)
	{
		Inc();
	}

	Ptr(T* p) :objptr(p)
	{
		if (p)
		{
			counter = new counter_type;
			(*counter) = 1;
		}
		else
		{
			counter = nullptr;
		}
	}
	Ptr()
	{
	}
	Ptr(const Ptr<T>& x)
	{
		counter = nullptr;
		objptr = nullptr;
		*this = x;
	}

	~Ptr()
	{
		this->Dec();
	}

	//从原生指针赋值
	Ptr<T>& operator =(T* pointer)
	{
		Dec();
		if (pointer)
		{
			counter = new counter_type;
			*counter = 1;
			objptr = pointer;
		}
		else
		{
			*counter = 0;
			objptr = 0;
		}
		return *this;
	}

	//从原生指针赋值
	Ptr<T>& operator =(const T* pointer)
	{
		Dec();
		if (pointer)
		{
			counter = new counter_type;
			*counter = 1;
			objptr = const_cast<T*>(pointer);
		}
		else
		{
			*counter = 0;
			objptr = 0;
		}
		return *this;
	}
	
	//动态类型转换
	template<class C>
	Ptr<C> Cast()const
	{
		typedef C mytype;
		mytype* converted = __LLIB_DYNAMIC_CAST(mytype*,objptr);
		return Ptr<C>((converted ? counter : nullptr), converted);
	}
	//从不同类型的智能指针转换
	template<class C>
	Ptr<T>& operator =(const Ptr<C>& ptr)
	{
		*this = ptr.Cast<T>();
		return *this;
	}
	
	
	//从同类型智能指针转换
	Ptr& operator =(const Ptr& ptr)
	{
		if (this != &ptr)
		{
			Dec();
			counter = ptr.counter;
			objptr = ptr.objptr;
			Inc();
		}
		return *this;
	}



	//关系运算
	bool operator==(const T* pointer)const
	{
		return objptr == pointer;
	}

	bool operator!=(const T* pointer)const
	{
		return objptr != pointer;
	}

	bool operator>(const T* pointer)const
	{
		return objptr>pointer;
	}

	bool operator>=(const T* pointer)const
	{
		return objptr >= pointer;
	}

	bool operator<(const T* pointer)const
	{
		return objptr<pointer;
	}

	bool operator<=(const T* pointer)const
	{
		return objptr <= pointer;
	}

	bool operator==(const Ptr<T>& pointer)const
	{
		return objptr == pointer.objptr;
	}

	bool operator!=(const Ptr<T>& pointer)const
	{
		return objptr != pointer.objptr;
	}

	bool operator>(const Ptr<T>& pointer)const
	{
		return objptr>pointer.objptr;
	}

	bool operator>=(const Ptr<T>& pointer)const
	{
		return objptr >= pointer.objptr;
	}

	bool operator<(const Ptr<T>& pointer)const
	{
		return objptr<pointer.objptr;
	}

	bool operator<=(const Ptr<T>& pointer)const
	{
		return objptr <= pointer.objptr;
	}

	operator bool()const
	{
		return objptr != 0;
	}

	pointer Obj()const
	{
		return objptr;
	}

	pointer operator->()const
	{
		return objptr;
	}

	T& operator*()const
	{
		return *objptr;
	}

	value_type& operator[](int index)const
	{
		return objptr[index];
	}

	counter_type UseCount() const
	{
		return counter ? *counter : 0;
	}
};


__LLIB_END_NAMESPACE
#endif
