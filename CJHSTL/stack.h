#ifndef __STACK_H__
#define __STACK_H__
#include "operator.h"
#include "config.h"
#include "xmemory.h"
#include "allocator.h"
#include "deque.h"
__CJH_BEGIN
	
template<class _Ty, 
	class Container = CJH::deque<_Ty> >
	class stack{
	public:
		typedef stack<_Ty, Container> self;
		typedef Container container_type;
		typedef typename Container::value_type value_type;
		typedef typename Container::size_type size_type;
		typedef typename Container::reference_type reference_type;

	public:

		stack():c(){

		}

		explicit stack(const Container& _c):c(_c){

		}

		stack(const self& x):c(x.c){

		}

		self& operator=(const self& x){
			c = x.c;
			return *this;
		}
		
		bool empty() const{
			return c.empty();
		}

		bool empty() {
			return c.empty();
		}


		size_type size() const{
			return c.size();
		}

		reference_type top(){
			return c.back();
		}


		void push(const _Ty& value){
			c.push_back(value);
		}

		void pop(){
			c.pop_back();
		}

		void swap(const self& x){
			c.swap(x.c);
		}
	private:
		Container c;
	};



__CJH_END
#endif
