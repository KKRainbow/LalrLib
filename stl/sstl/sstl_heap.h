//堆算法
#ifndef SSTL_HEAP_H
#define SSTL_HEAP_H
#include"sstl_config.h"
#include"sstl_iterator.h"
#include"sstl_functors.h"

SSTL_BEGIN_NAMESPACE

//默认max-heap
template<class RandomIterator, class Rel = Greater<typename iterator_traits<RandomIterator>::value_type>>
class _Heap //不对外开放的类
{
public:
	typedef typename iterator_traits<RandomIterator>::difference_type dif_type;
	typedef IterCmp<RandomIterator> itercmp;
	typedef typename iterator_traits<RandomIterator>::value_type value_type;
protected:
	//取i节点的左孩子，base就是这个heap的root
	static RandomIterator Left(RandomIterator& root, RandomIterator& i)
	{
		RandomIterator tmp = root;
		dif_type dis = _distance(root, i);
		_advance(tmp, 2 * dis + 1);
		return tmp;
	}
	static RandomIterator Right(RandomIterator& root, RandomIterator& i)
	{
		RandomIterator tmp = root;
		dif_type dis = _distance(root, i);
		_advance(tmp, 2 * dis + 2);
		return tmp;
	}
	static RandomIterator Parent(RandomIterator& root, RandomIterator& i)
	{
		RandomIterator tmp = root;
		dif_type dis = _distance(root, i);
		_advance(tmp, (dis - 1) / 2);
		return tmp;
	}
	//左右两子树都是堆，根节点不一定是，这个函数让这种情况的数列成堆
	template<class MRel = Rel>
	static void Heapify(RandomIterator& root, RandomIterator last, RandomIterator node, MRel rel = Rel())
	{
		dif_type heap_size = _distance(root, last);
		RandomIterator largest = node;
		RandomIterator left = Left(root, node), right = Right(root, node);
		if (right < last&&rel(*right,*largest))
		{
			largest = right;
		}
		if (left < last&&rel(*left, *largest))
		{
			largest = left;
		}
		if (largest == node) return;//root是最大值则已经成堆
		else
		{
			swap(largest, node);
			Heapify(root, last,largest , rel);
		}
	}
public:
	template<class MRel = Rel>
	static void BuildHeap(RandomIterator first, RandomIterator last, MRel rel = Rel())
	{
		dif_type heap_size = _distance(first, last);
		for (dif_type node_index = (heap_size - 1) / 2; node_index >= 0; node_index--)
		{
			Heapify(first, last,first + node_index, rel);
		}
	}
	//执行这个操作之后，需要手动把last-1。
	template<class MRel = Rel>
	static value_type& ExtractTop(RandomIterator root, RandomIterator last, MRel rel = Rel())
	{
		dif_type heap_size = _distance(root, last);
		swap(root, (last - 1));
		Heapify(root, last - 1, root, rel);
		return *(last-1);
	}

	template<class MRel = Rel>
	static void ModifyKey(RandomIterator root, RandomIterator last, RandomIterator tochange, value_type new_key, MRel rel = Rel())
	{
		if (!rel(new_key, *tochange))  //没有增加，反而降低了.应该下溯,也就是调用Heapify
		{
			*tochange = new_key;
			Heapify(root,last,tochange, rel);
		}
		else //应该上溯
		{
			RandomIterator parent = Parent(root, tochange);
			while (parent != root)
			{
				if (rel(new_key, *parent))
				{
					*tochange = *parent;
					tochange = parent;
					parent = Parent(root, tochange);
				}
				else
				{
					break;
				}
			}
			*tochange = *parent;
			*parent = new_key;
		}
	}
};


SSTL_END_NAMESPACE

#endif