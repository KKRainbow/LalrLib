#pragma once
#include"sstl_vector.h"

SSTL_BEGIN_NAMESPACE

template<class T>
class Array:protected Vector<T>
{
	public:
		using typename Vector<T>::iterator;
		using typename Vector<T>::reference;
		using typename Vector<T>::size_type;
		using typename Vector<T>::const_reference;
	public:
		Array(T _x[],size_t length)
		{
			Initialize(_x,length);
		}
		Array()
		{}
		void Initialize(T _x[],size_t length)
		{
			this->start = this->finish = _x;
			this->end_of_storage = _x + length;
		}
		iterator Erase(iterator position)
		{
			Vector<T>::Erase(position);
		}
		iterator Erase(iterator from,iterator to)
		{
			Vector<T>::Erase(from,to);
		}
		reference Front(){return Vector<T>::Front();}
		reference Back(){
			return Vector<T>::Back();
		}
		size_type Size()
		{
			return Vector<T>::Size();
		}
		size_type Capacity()
		{
			return Vector<T>::Capacity();
		}
		void PopBack()
		{
			return Vector<T>::PopBack(); 
		}
		bool Judge(size_type n)
		{
			if(Capacity() - Size() <= n)
			{
				return false;
			}
			return true;
		}
		bool PushBack(const_reference _x)
		{
			if(!Judge(1))
			{
				return false;
			}
			Vector<T>::PushBack(_x);
			return true;
		}
		bool Insert(iterator position,size_type n,const_reference x)
		{
			if(!Judege(n))
			{
				return false;
			}
			Vector<T>::Insert(position,n,x);
			return true;
		}
		bool Insert(iterator position,const_reference x)
		{
			if(!Judge(1))
			{
				return false;
			}
			Vector<T>::Insert(position,x);
			return true;
		}
		reference operator[](int n)
		{
			return Vector<T>::operator[](n);
		}
		iterator Begin()
		{
			return Vector<T>::Begin();
		}
		iterator begin()
		{
			return Vector<T>::begin();
		}
		iterator End()
		{
			return Vector<T>::End();
		}
		iterator end()
		{
			return Vector<T>::end();
		}
};


SSTL_END_NAMESPACE
