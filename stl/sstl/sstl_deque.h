#ifndef SSTL_DEQUE_H
#define SSTL_DEQUE_H

#include"sstl_config.h"
#include"sstl_iterator.h"
#include"sstl_alloc.h"
#include"sstl_algobase.h"
#include"sstl_vector.h"
#include<stddef.h>

SSTL_BEGIN_NAMESPACE

template<class T, class Ref = T&, class Ptr = T*,size_t ElemInBuf = 0>
class __DequeIterator :public Iterator<random_access_iterator_tag, T, ptrdiff_t, Ptr, Ref>
{
public:
	using typename Iterator<random_access_iterator_tag, T, ptrdiff_t, Ptr, Ref>::distance;
	using typename Iterator<random_access_iterator_tag, T, ptrdiff_t, Ptr, Ref>::value_type;
	using typename Iterator<random_access_iterator_tag, T, ptrdiff_t, Ptr, Ref>::reference;
	using typename Iterator<random_access_iterator_tag, T, ptrdiff_t, Ptr, Ref>::iterator_category;
	using typename Iterator<random_access_iterator_tag, T, ptrdiff_t, Ptr, Ref>::pointer;
public:
	typedef __DequeIterator<T, T&, T*, ElemInBuf> iterator;
	typedef __DequeIterator<T,const T&,const T*, ElemInBuf> const_iterator;

	typedef __DequeIterator<T, Ref, Ptr, ElemInBuf> self;

	typedef size_t size_type;
	typedef distance difference_type;

	typedef value_type* data_array;
	typedef data_array* map_pointer;

	static inline size_t ElemNumInBuffer()
	{
		//默认1KB空间
		return ElemInBuf != 0 ? ElemInBuf : (sizeof(T) > 512 ? sizeof(T) : 512 / sizeof(T));
	}
	static inline size_t BufferSize()
	{
		return ElemNumInBuffer()*sizeof(T);
	}

	pointer cur;
	pointer first;
	pointer last;
	map_pointer node;
	//构造函数
	__DequeIterator(map_pointer n, pointer c =nullptr, pointer f = nullptr, pointer l = nullptr)
	{
		node = n;
		cur = c == nullptr ? *n : c;
		first = f == nullptr ? *n : c;
		last = l == nullptr ? *n + ElemNumInBuffer() : l;
	}
	__DequeIterator(const __DequeIterator& x)
	{
		cur = x.cur;
		first = x.first;
		last = x.last;
		node = x.node;
	}

	self& operator=(const __DequeIterator& x)
	{
		if (this == &x)return *this;	//防止自己给自己赋值
		cur = x.cur;
		first = x.first;
		last = x.last;
		node = x.node;
		return *this;
	}

	void SetNode(map_pointer new_node)
	{
		node = new_node;
		first = *new_node;
		last = first + ElemNumInBuffer();
	}

	reference operator*()const { return *cur; }
	pointer operator->()const { return cur; }

	difference_type operator-(const self& x)const
	{
		return (ElemNumInBuffer()*(node - x.node - 1)) + (cur - first) + (x.last - x.cur);
	}

	self& operator++()
	{
		++cur;
		if (cur == last)
		{
			SetNode(node + 1);
			cur = first;
		}
		return *this;
	}
	self operator++(int)
	{
		self tmp = *this;
		++*this;
		return tmp;
	}
	self& operator--()
	{
		if (cur == first)
		{
			SetNode(node - 1);
			cur = last-1;
		}
		else
		{
			cur--;
		}
		return *this;
	}
	self operator--(int)
	{
		self tmp = *this;
		--*this;
		return tmp;
	}
	self& operator+=(difference_type n)
	{
		difference_type offset = n + (cur - first);
		if (offset >= 0 && static_cast<size_type>(offset) < ElemNumInBuffer())
		{
			cur += n;
		}
		else
		{
			auto node_off = offset>0 ? offset / ElemNumInBuffer() :
				-((difference_type)(-offset - 1) / (difference_type)ElemNumInBuffer()) - 1;
			SetNode(node + node_off);
			cur = first + (offset - node_off*ElemNumInBuffer());
		}
		return *this;
	}
	self operator+(difference_type n)
	{
		self tmp = *this;
		return tmp += n;
	}
	self& operator-=(difference_type n)
	{
		return *this += -n;
	}
	self operator-(difference_type n)
	{
		self tmp = *this;
		return tmp -= n;
	}

	reference operator[](difference_type n)const
	{
		return *(*this + n);
	}

	bool operator==(const self& x)const{ return x.cur == cur; }
	bool operator!=(const self& x)const{ return x.cur != cur; }
	bool operator<(const self& x)const
	{
		return node == x.node ? (cur < x.cur) : (node < x.node);
	}
	bool operator<=(const self& x)const{ return *this < x || *this == x; }
	bool operator>(const self& x)const{ return !(*this <= x); }
	bool operator>=(const self& x)const{ return !(*this < x); }
};

template<class T, class Alloc = ALLOC(T),size_t BufSize = 0>
class Deque
{
	__LLIB_CLASS_DECLARE(Deque, Object);
public:
	typedef T value_type;
	typedef value_type* pointer;
	typedef value_type& reference;
	typedef const T* const_pointer;
	typedef const T& const_reference;

	typedef size_t size_type;
	typedef ptrdiff_t difference_type;

	typedef __DequeIterator<T, reference, pointer, BufSize> iterator;
	
protected:
	typedef typename __DequeIterator<T, reference, pointer, BufSize>::data_array data_array;
	typedef typename __DequeIterator<T, reference, pointer, BufSize>::map_pointer map_pointer;
	typedef Alloc allocator_type;

	allocator_type alloc;
	
	static inline size_type ElemNumInBuf()
	{
		return iterator::ElemNumInBuffer();
	}

protected:
	Vector<data_array> map;
	iterator start_of_storage;	//容器存储空间的开始
	iterator end_of_storage;

	iterator start; //容器的开始 
	iterator finish;

	void reserve_map_at_front(size_type n = 1);//在前方预留n个缓冲区
	void reserve_map_at_back(size_type n = 1);
	//在position处预留n个元素的空间,并返回这段空间起始的iterator
	iterator reserve_buf_at(iterator position, size_type n);
	void Deallocate()
	{
		alloc.Destroy(Begin(), End());
		for (auto b = map.Begin();b!=map.End();b++)
		{
			if (*b != nullptr)
			{
				alloc.Deallocate(*b, ElemNumInBuf());
			}
		}
	}
public:
	//Iterator
	iterator Begin(){ return start; }
	iterator End(){ return finish; }
	iterator begin(){ return Begin(); }
	iterator end(){ return End(); }
	size_type Size() const{ return finish - start; }
	size_type Capacity() const{ return end_of_storage - start_of_storage; }
	bool Empty(){ return Begin() == End(); }
	//Access
	reference operator [](size_type n){ return *(Begin() + n); }
	reference Front(){ return *(Begin()); }
	reference Back(){ return *(End()-1); }
	//Modify
	void PushBack(const_reference val)
	{
		if (finish.cur == end_of_storage.last -1) //缓冲区木有备用空间了
		{
			reserve_map_at_back();
		}
		alloc.Construct(&*finish, T());
		*finish = val;
		++finish;
	}
	void PushFront(const_reference val)
	{
		if (start.cur == start_of_storage.first)
		{
			reserve_map_at_front();
		}
		--start;
		alloc.Construct(&*start, T());
		*start = val;
	}
	void PopBack()
	{
		auto tmp = finish.node;
		--finish;
		alloc.Destroy(finish.cur);
		/*
		if (tmp != finish.node)
		{
			alloc.Deallocate(*tmp, ElemNumInBuf());
			*tmp = nullptr;
		}
		*/
	}
	void PopFront()
	{
		auto tmp = start.node;
		alloc.Destroy(start.cur);
		++start;
		/*
		if (tmp != start.node)
		{
			alloc.Deallocate(*tmp, ElemNumInBuf());
			*tmp = nullptr;
		}
		*/
	}
	iterator Insert(iterator position, const_reference val)
	{
		position = reserve_buf_at(position, 1);
		_uninitialized_fill_n(position, 1, val);
		return position;
	}
	void Insert(iterator position, size_type n, const_reference val)
	{
		if (n <= 0)return;
		position = reserve_buf_at(position, n);
		_uninitialized_fill_n(position, n, val);
	}
	template <class InputIterator>
	void Insert(iterator position, InputIterator first, InputIterator last)
	{
		difference_type n = _distance(first, last);
		position = reserve_buf_at(position, n);
		copy(first, last, position);;
	}
	iterator Erase(iterator position)
	{
		copy(position + 1, End(), position);
		finish--;
		alloc.Destroy(finish);
		return position;
	}
	iterator Erase(iterator from, iterator to)
	{
		if (to <= from)return from;
		difference_type dif = to - from;
		to = copy(to, End(), from);
		alloc.Destroy(to, finish);
		finish = finish - dif;
		return from;
	}
	void Clear()
	{
		Erase(Begin(), End());
	}
	//析构函数
	virtual ~Deque()
	{
		Deallocate();
	}
	//构造函数
	Deque(allocator_type ma = allocator_type()) :alloc(ma), map(1, alloc.Allocate(ElemNumInBuf())),
		start_of_storage((map_pointer)map.Begin()), end_of_storage((map_pointer)map.End()-1), start(start_of_storage), finish(start)
	{
		end_of_storage.cur = end_of_storage.last;
		alloc.Construct(start_of_storage.first, end_of_storage.last, T());
	}
	Deque(size_type n, allocator_type ma = allocator_type()) :Deque(ma)
	{
		Insert(End(), n, T());
	}
	Deque(size_type n, const_reference x, allocator_type ma = allocator_type()) :Deque(ma)
	{
		Insert(End(), n, x);
	}
	template <class InputIterator>
	Deque(InputIterator first, InputIterator last, const allocator_type& alloc = allocator_type()) : Deque(alloc)
	{
		Insert(End(), first, last);
	}
	Deque(const Deque& x):Deque(x.alloc)
	{
		Erase(start, finish);
		Insert(start, x.start, x.finish);
	}
	Deque& operator=(const Deque& x)
	{
		if (this == &x)return *this;	//防止自己给自己赋值
		Erase(Begin(), End());
		Insert(start, x.start, x.finish);
		return *this;
	}
	void Reserve(const size_type n)
	{
		//如此实现不知道合适不
		//reserve_map_at_front(n);
		//reserve_map_at_back(n);
	}
};
template<class T, class Alloc, size_t BufSize>
typename Deque<T, Alloc, BufSize>::iterator 
Deque<T, Alloc, BufSize>::reserve_buf_at(iterator position, size_type n)
{
	difference_type index = position - start;
	if (static_cast<size_type>(index) > Size() / 2) //后面的少
	{
		size_type remain = end_of_storage - finish;
		size_type need_more = n <= remain ? 0 : n - remain;

		if (need_more != 0)
		{
			reserve_map_at_back(((need_more - 1) / ElemNumInBuf()) + 1);
			position = start + index;
		}
		copy(position, finish, position + n);
		finish += n;
		return position;
	}
	else //前面的少
	{
		size_type remain = start - start_of_storage;
		size_type need_more = n <= remain ? 0 : n - remain;

		if (need_more != 0)
		{
			reserve_map_at_front(((need_more - 1) / ElemNumInBuf()) + 1);
			position = start + index;
		}
		copy(start, position, start-index);
		start -= n;
		return position - n;
	}

}

template<class T, class Alloc, size_t BufSize>
void Deque<T, Alloc, BufSize>::reserve_map_at_front(size_type n)//在前方预留n个缓冲区
{
	if (n <= 0)return;
	auto finish_node_offset = end_of_storage.node - finish.node;
	auto start_node_offset = finish.node - start.node;//前方预留，startnode与finishnode相对距离不变；
	auto start_cur_offset = start.cur - start.first;
	auto finish_cur_offset = finish.cur - finish.first;

	map.Insert(map.Begin(), n, nullptr);
	auto iter = map.Begin();
	while (n--)
	{
		*iter = (data_array)alloc.Allocate(ElemNumInBuf());
		//alloc.Construct(*iter, *iter + ElemNumInBuf(), T());
		iter++;
	}
	start_of_storage.SetNode((map_pointer)map.Begin());
	start_of_storage.cur = start_of_storage.first;
	end_of_storage.SetNode((map_pointer)map.End() - 1);
	end_of_storage.cur = end_of_storage.last;

	finish.SetNode(end_of_storage.node - finish_node_offset);
	finish.cur = finish.first + finish_cur_offset;

	start.SetNode(finish.node - start_node_offset);
	start.cur = start.first + start_cur_offset;
}

template<class T, class Alloc, size_t BufSize>
void Deque<T, Alloc, BufSize>::reserve_map_at_back(size_type n)
{
	if (n <= 0)return;
	auto start_node_offset = start.node - start_of_storage.node;//后方预留，startnode与finishnode相对距离不变；
	auto finish_node_offset = finish.node - start.node;
	auto start_cur_offset = start.cur - start.first;
	auto finish_cur_offset = finish.cur - finish.first;

	map.Insert(map.End(), n, nullptr);
	auto iter = map.End() - 1;
	while (n--)
	{
		*iter = (data_array)alloc.Allocate(ElemNumInBuf());
		//alloc.Construct(*iter, *iter + ElemNumInBuf(), T());
		--iter;
	}
	start_of_storage.SetNode((map_pointer)map.Begin());
	start_of_storage.cur = start_of_storage.first;
	end_of_storage.SetNode((map_pointer)map.End() - 1);
	end_of_storage.cur = end_of_storage.last;

	start.SetNode(start_of_storage.node + start_node_offset);
	start.cur = start.first + start_cur_offset;

	finish.SetNode(start.node + finish_node_offset);
	finish.cur = finish.first + finish_cur_offset;
}

SSTL_END_NAMESPACE

#endif
