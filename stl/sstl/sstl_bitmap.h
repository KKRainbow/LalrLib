#ifndef SSTL_BITMAP_H
#define SSTL_BITMAP_H
#include"sstl_config.h"
#include"sstl_iterator.h"
#include"sstl_alloc.h"
#include"sstl_algobase.h"
SSTL_BEGIN_NAMESPACE
template<class ResType = unsigned,int bitSize = 1,class Alloc=ALLOC(ResType)>
class Bitmap;
template<class ResType,int bitSize = 1>
class __BitmapIterator:public Iterator<bidirectional_iterator_tag,ResType,ptrdiff_t,ResType*,ResType&>
{
	public:
		typedef unsigned int value_type;
	protected:
		friend class Bitmap<ResType,bitSize>;
		static value_type BitsToNum(ResType* _Base,int _Offset)
		{
			value_type resLow = 0,resHigh = 0;
			value_type mask = (1<<(bitSize-1));
			const size_t ResTypeBits = 8*sizeof(ResType);
			mask = mask + (mask-1);
			if(_Offset>=ResTypeBits)
			{
				_Base += (_Offset*bitSize)/ResTypeBits;
				_Offset = (_Offset*bitSize)%ResTypeBits;
			}
			resLow = *_Base; 
			resLow>>=_Offset;
			resLow&= mask;
			if(_Offset+bitSize<=ResTypeBits)
			{
				return resLow;
			}
			else
			{
				int remain = (bitSize - (ResTypeBits-_Offset));
				resHigh = *(_Base+1);
				resHigh<<=(ResTypeBits - remain);
				return resLow|resHigh;
			}
		}
	private:
		mutable ResType* base=nullptr;
		mutable int offset=0; 
		void Normalize() const
		{
			base += (offset)/(sizeof(ResType)*8);
			offset = (offset)%(sizeof(ResType)*8);
		}
	public:
		__BitmapIterator(){}
		__BitmapIterator(const __BitmapIterator& _x)
			:base(_x.base),offset(_x.offset)
		{}
		__BitmapIterator(ResType* _Base,int _Off):base(_Base),offset(_Off){}
		__BitmapIterator& operator =(const __BitmapIterator _x)
		{
			if(this == &_x)
			{
				return *this;
			}
			base = _x.base;
			offset = _x.offset;
		}
		int operator* ()const
		{
			return BitsToNum(base,offset);	
		}
		__BitmapIterator& operator+=(int _n)
		{
			offset +=_n*bitSize;
			Normalize();
			return *this;
		}
		__BitmapIterator& operator-=(int _n)
		{
			return *this += -_n;
		}
		const __BitmapIterator operator+(int _n)
		{
			__BitmapIterator res = *this;
			res+=_n;
			return res;
		}
		const __BitmapIterator operator-(int _n)
		{
			__BitmapIterator res = *this;	
			res+=-_n;
			return res;
		}
		__BitmapIterator& operator++()
		{
			*this+=1;
			return *this;
		}
		__BitmapIterator& operator--()
		{
			*this-=1;
			return *this;
		}
		const __BitmapIterator operator--(int)
		{
			__BitmapIterator res = *this;
			--*this;
			return res;
		}
		const __BitmapIterator operator++(int)
		{
			__BitmapIterator res = *this;
			++*this;
			return res;
		}
		bool operator ==(const __BitmapIterator& _X)
		{
			this->Normalize();
			_X.Normalize();
			if(_X.base == this->base&&_X.offset==this->offset)
			{

				return true;
			}
			else
			{
				return false;
			}
		}
		bool operator !=(const __BitmapIterator& x)
		{
			return !(*this==x);
		}
		ptrdiff_t operator -(const __BitmapIterator& x)
		{
			this->Normalize();
			x.Normalize();
			return (((this->base-x.base)*sizeof(ResType)*8) + (this->offset - x.offset))/bitSize; 
		}
};
template<class ResType ,int bitSize,class Alloc>
class Bitmap
{
	public:
		typedef ResType value_type;
		typedef __BitmapIterator<ResType,bitSize> iterator;
	private:
		iterator start;
		iterator finish;
		Ptr<ResType> map;
		Alloc alloc;
	public:
		static ResType BitsToNum(ResType* _Base,int _Offset)
		{
			return iterator::BitsToNum(_Base,_Offset);
		}
		Bitmap(){}
		Bitmap(const Bitmap& _x)
		{
			start = _x.start;
			finish = _x.finish;
			map = _x.map;
		}
		Bitmap& operator=(const Bitmap& _x)
		{
			if(this ==&_x)
			{
				return *this;
			}
			start = _x.start;
			finish = _x.finish;
			map = _x.map;
		}
		Bitmap(ResType* _Base,size_t _Len,bool _Copy)
			:start(_Base,0)
		{
			if(_Copy == true)
			{
				int size = (_Len*bitSize+8*sizeof(ResType)-1)/(8*sizeof(ResType));
				map = alloc.Allocate(size);
				sstl::copy(start.base,start.base+size,map.Obj());
				start = iterator(map.Obj(),0);
			}
			finish = start + _Len;	
		}
		iterator Begin()
		{
			return start;
		}
		iterator begin()
		{
			return start;
		}
		iterator End()
		{
			return finish;
		}
		iterator end()
		{
			return finish;	
		}
		void SetBitTo(int _BitNum,int _Val)
		{
			iterator it = start+_BitNum;
			unsigned int mask = (1<<(bitSize-1));
			mask = mask + (mask-1);
			_Val &= mask;
			const size_t ResTypeBits = 8*sizeof(ResType);
			ResType* base = it.base;
			int offset = it.offset;
			unsigned low = _Val<<offset;
			*base &= ~(mask<<offset);
			*base|=low;
			if(ResTypeBits<=offset+bitSize)//跨越了边界
			{
				unsigned int high = _Val>>(bitSize-(ResTypeBits-offset));
				++base;
				*base &= ~(mask>>(bitSize-(ResTypeBits-offset)));
				*base|= high;
			}
		}
		void GetBit(int _BitNum)
		{
			return *(start+_BitNum);
		}
		ResType operator[](int _Ind)
		{
			return *(start+_Ind);
		}
	private:
		void SetBitsToVal(int _From,int _To,char _Val)
		{
			if(_To<=_From)return;
			iterator from = start+_From;
			iterator to = start + _To;
			if(to.base - from.base > 1)
			{
				memset(from.base+1,_Val,sizeof(ResType)*(to.base-from.base-1));
				for(int i = _From;i<_From + (sizeof(ResType)*8);i++)
				{
					SetBitTo(i,_Val);
				}
				for(int i = _To - (sizeof(ResType)*8);i<_To;i++)
				{
					SetBitTo(i,_Val);
				}
			}
			else
			{
				for(int i = _From;i<_To;i++)
				{
					SetBitTo(i,_Val);
				}
			}
			return;
		}
	public:
		void SetBitsToOne(int _From,int _To)
		{
			SetBitsToVal(_From,_To,0xff);
		}
		void SetBitsToOne(iterator _From,iterator _To)
		{
			SetBitsToVal(_From - Begin(),_To - Begin(),0xff);
		}
		
		void SetBitsToZero(int _From,int _To)
		{
			SetBitsToVal(_From,_To,0);
		}
		void SetBitsToZero(iterator _From,iterator _To)
		{
			SetBitsToVal(_From - Begin(),_To - Begin(),0);
		}
};
SSTL_END_NAMESPACE
#endif
