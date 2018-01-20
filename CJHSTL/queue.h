#ifndef __QUEUE_H__
#define __QUEUE_H__
#include "operator.h"
#include "config.h"
#include "xmemory.h"
#include "allocator.h"
#include "deque.h"
#include "vector.h"
#include "xmemory.h"

__CJH_BEGIN

	template<class _Ty,
			class Container = deque<_Ty> >
	class queue{
	public:
		typedef queue<_Ty, Container> self;
		typedef Container container_type;
		typedef typename Container::value_type value_type;
		typedef typename Container::reference_type reference_type;
		typedef typename Container::size_type size_type;
	public:
		queue() :c(){

		}

		explicit queue(const Container& _c) :c(_c){

		}

		queue(const self& x) :c(x.c){

		}
		self& operator=(const self& x){
			c = x.c;
			return *this;
		}

		size_type size() const{
			return c.size();
		}

		reference_type front() const{
			return c.front();
		}

		reference_type back() const{
			return c.back();
		}
		bool empty() {
			return c.empty();
		}


		void push(const _Ty& value){
			return c.push_back(value);
		}

		void pop(){
			return c.pop_front();
		}
		void swap(const self& x){
			c.swap(x.c);
		}
	private:
		Container c;
	};

	template<class Rnit, 
			class Compare,
			class difference_type = iterator_traits<Rnit>::difference_type>
	void __adjust_heap(Rnit first, difference_type start, difference_type end, Compare cmp){
		difference_type left_child = start * 2 + 1;
		difference_type right_child = left_child + 1;
		difference_type index = start;
		if (left_child < end && cmp(*(first + index), *(first + left_child))) {
			index = left_child;
		}
		if (right_child < end && cmp(*(first + index), *(first + right_child))) {
			index = right_child;
		}

		if (index != start){
			CJH::swap(*(first + start), *(first + index));
			__adjust_heap(first, index, end, cmp);
		}
	}

	template<class Rnit, 
			class Compare>
	void adjust_heap(Rnit first, Rnit last,Compare cmp){
		typedef typename CJH::iterator_traits<Rnit>::value_type value_type;
		typedef typename CJH::iterator_traits<Rnit>::difference_type difference_type;
		difference_type len = last - first;
		Rnit left_child = first;
		__adjust_heap(first, 0, len, cmp);
	}

	template<class Rnit, 
			class Compare>
	void make_heap(Rnit first, Rnit last,Compare cmp){
		typedef typename CJH::iterator_traits<Rnit>::value_type value_type;
		typedef typename CJH::iterator_traits<Rnit>::difference_type difference_type;
		difference_type len = last - first;
		for (difference_type i = len / 2; i >= 0; --i){
			adjust_heap(first + i, last, cmp);
		}
	}

	template<class Rnit, 
			class Compare>
	void __push_heap(Rnit first, Rnit last, Compare cmp){
		typedef typename CJH::iterator_traits<Rnit>::value_type value_type;
		typedef typename CJH::iterator_traits<Rnit>::difference_type difference_type;
		difference_type len = last - first;
		difference_type last_node = len - 1;
		difference_type parent = (last_node - 1) / 2;
		if (last_node == 0) return;
		else{
			if (cmp(*(first + parent), *(first + last_node))){
				CJH::swap(*(first + parent), *(first + last_node));
				__push_heap(first, first + parent + 1, cmp);
			}
		}
	}

	template<class Rnit, 
			class Compare>
	void push_heap(Rnit first, Rnit last, Compare cmp){
		__push_heap(first, last, cmp);
	}

	

	//template<class Rnit, class Compare = less<iterator_traits<Rnit>::value_type> >
	//void __pop_heap(Rnit first, difference_type start, difference_type end, Compare cmp){
	//	typedef typename CJH::iterator_traits<Rnit>::value_type value_type;
	//	typedef typename CJH::iterator_traits<Rnit>::difference_type difference_type;
	//	difference_type left_child;
	//}
	template<class Rnit, 
			class Compare>
	void pop_heap(Rnit first, Rnit last, Compare cmp){
		typedef typename CJH::iterator_traits<Rnit>::value_type value_type;
		typedef typename CJH::iterator_traits<Rnit>::difference_type difference_type;
		CJH::swap(*first, *--last);
		difference_type len = last - first;
		__adjust_heap(first, 0, len, cmp);
	}

	template<class Rnit, 
			class Compare>
	void sort_heap(Rnit first, Rnit last){

	}


	template<class _Ty, 
			class Container=CJH::vector<_Ty>,
			class Compare=CJH::less<_Ty> >
	class priority_queue{
	public:
		typedef priority_queue<_Ty, Container, Compare> self;
		typedef Container container_type;
		typedef typename Container::value_type value_type;
		typedef typename Container::reference_type reference_type;
		typedef typename Container::size_type size_type;

	public:
		priority_queue():c(){

		}

		priority_queue(const Container& _c):c(_c){

		}

		self& operator=(const self& x){
			if (this == &x) return *this;
			try{
				c = x.c;
			}
			catch (...){
				thorw;
			}
			return *this;
		}


		priority_queue(const self& x) :c(x.c){

		}


		bool empty() const{
			return c.empty();
		}

		size_type size() const{
			return c.size();
		}

		reference_type top(){
			return c.front();
		}

		void push(const _Ty& value){
			c.push_back(value);
			CJH::push_heap(c.begin(), c.end(), cmp);
		}

		void pop() {
			CJH::pop_heap(c.begin(), c.end(), cmp);
			c.pop_back();
		}


		void swap(self& x){
			c.swap(x.c);
			CJH::swap(cmp, x.cmp);
		}
	private:
		Container c;
		Compare cmp;
	};


__CJH_END
#endif //__QUEUE_H__