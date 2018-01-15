#ifndef __XMEMORY_H__
#define __XMEMORY_H__

#include "config.h"
__CJH_BEGIN


	template<class _Ty>
	_Ty* addressof(_Ty &value){
		return reinterpret_cast<_Ty*>(
			&const_cast<char&>(
			reinterpret_cast<const volatile char &>(value)));
	}
	template<class _Ty1, class _Ty2>
	inline void construct(_Ty1 *p, const _Ty2& value){
	//	new(static_cast<void*>(CJH::addressof(p)))_Ty1(value);
		new(p)_Ty1(value);
	}

	template<class Forward_iterator, class _Ty>
	inline void construct(Forward_iterator position, const _Ty& value){
		typedef typename iterator_traits<Forward_iterator>::pointer_type pointer_type;
		pointer_type p;
		p = &*position;
	//	new(p)_Ty(value);
		new(static_cast<void*>(CJH::addressof(*position)))_Ty(value);
	}

	template<class _Ty>
	inline void destroy(_Ty *p){
		p->~_Ty();
	}
	//template<int>
	//inline void destory(int* x){
	//
	//}

	template<class Forward_iterator>
	inline void destroy(Forward_iterator first, Forward_iterator last){
		typedef typename iterator_traits<Forward_iterator>::pointer_type pointer_type;
		for (first; first != last; ++first){
			CJH::destroy(CJH::addressof(*first));
		}
	}



	template<class Bidirectional_iterator1, class Bidirectional_iterator2>
	inline Bidirectional_iterator2 copy_backward(Bidirectional_iterator1 first, Bidirectional_iterator1 last,
		Bidirectional_iterator2 dist){
		while (last != first)
			*(--dist) = *(--last);
		return dist;
	}

	template<class Input_iterator, class Output_iterator>
	inline Output_iterator copy(Input_iterator first, Input_iterator last, Output_iterator dist){
		while (first != last){
			*dist = *first;
			++dist;
			++first;
		}
		return dist;
	}
	

	template<class Input_iterator, class Forward_iterator>
	inline Forward_iterator uninitialized_copy(Input_iterator first, Input_iterator last, Forward_iterator dist){
		typedef typename CJH::iterator_traits<Forward_iterator>::value_type value_type;
		Forward_iterator current = dist;
		try{
			for (; first != last; ++first, (void)++current){
				new(static_cast<void*>(CJH::addressof(*current)))value_type(*first);
			}
			return current;
		}
		catch (...){
			for (; dist != current; ++dist){
				destroy(CJH::addressof(*dist));
			}
			throw;
		}
	}

	template<class Forward_iterator, class _Ty>
	inline void fill(Forward_iterator first, Forward_iterator last, const _Ty &value){
		for (; first != last; ++first){
			*first = value;
		}
	}

	template<class Forward_iterator,class Size, class _Ty>
	inline Forward_iterator uninitialized_fill_n(Forward_iterator first, Size n, const _Ty &value){
		typedef typename CJH::iterator_traits<Forward_iterator>::value_type value_type;
		Forward_iterator current = first;
		try{
			for (; n > 0; (void)++current, --n){
				new(static_cast<void*>(CJH::addressof(*current))) value_type(value);
			}

			return current;
		}
		catch (...){
			for (; current != first; ++first){
				CJH::destroy(CJH::addressof(*first));
			}
			throw;
		}
	}



__CJH_END
#endif