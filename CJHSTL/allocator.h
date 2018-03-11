#pragma once
#ifndef __ALLOCATOR_H__
#define __ALLOCATOR_H__

#include "memorypool.h"
#include "config.h"
#include "1jjalloc.h"
#include "xmemory.h"

__CJH_BEGIN

	extern Memorypool Mp;
	

	template<class _Ty>
	struct _Alloctor_base{
		typedef _Ty value_type;
	};

	template<class _Ty>
	struct _Alloctor_base<const _Ty>{
		typedef _Ty value_type;
	};

	

	template<class _Ty>
	class allocator : public _Alloctor_base<_Ty>{
	public:
		typedef _Alloctor_base<_Ty> _myBase;
		typedef typename _myBase::value_type value_type;
		
		typedef value_type* pointer;
		typedef const value_type* const_pointer;

		typedef value_type& reference;
		typedef const value_type* const_reference;

		typedef size_t size_type;
		typedef int different_type;

	public:

		static alloc_default _Alloc_default;
		static alloc_malloc _Alloc_malloc;
	public:
		static pointer allocate(size_type n){
			if (0 == n || n > __MAX_BYTES){
				return (pointer)_Alloc_malloc.allocate(n);
			}
			else {
				return (pointer)_Alloc_default.allocate(n);
			}
		}

		/*pointer allocate(size_type n, const void *){
			if (0 == n){
				return (pointer)_Alloc_malloc.allocate(n);
			}
			else if (n <= __MAX_BYTES){
				return (pointer)_Alloc_default.allocate(n);
			}
			else{
				return (pointer)_Alloc_malloc.allocate(n);
			}
		}*/

		static void deallocate(pointer p, size_type n = 0){
			if (0 == n || n > __MAX_BYTES){
				_Alloc_malloc.deallocate(p, n);
			}
			else {
				_Alloc_default.deallocate(p, n);
			}
		}

	/*	pointer reallocate(pointer p, size_type oldsize, size_type newsize){
			if (0 == oldsize || oldsize > __MAX_BYTES){
				return (pointer)_Alloc_malloc.reallocate(p, oldsize, newsize);
			}
			else {
				return (pointer)_Alloc_default.allocate(n);
			}
		}
		*/
		/*size_tye maxsize() const{

		}*/

		static void construct(_Ty *ptr);
		static void construct(_Ty *ptr, const _Ty& value){
			_Construct(ptr, value);
		}
//		void construct(_Ty *ptr);

		static pointer address(reference value){
			return (pointer)&value;
		}
	private:
		allocator(){
			
		}
	};

	template<class _Ty>
	CJH::alloc_default allocator<_Ty>::_Alloc_default = alloc_default();

	template<class _Ty>
	CJH::alloc_malloc allocator<_Ty>::_Alloc_malloc = alloc_malloc();
	/*
	template<class _Ty>
	static Memorypool allocator<_Ty>::_Mp = Mp;
	*/
__CJH_END



#endif





//#define NUM 20000
//#define PTRNUM 200
//using namespace CJH;


//≤‚ ‘¥˙¬Î
//class A{
//public:
//	A(){
//		cout << "Dada\n";
//	}
//};
//int main(){
//
//	CJH::allocator<A> alloc;
//	cout << endl;
//	cout << alloc._Alloc_default.size();
//	alloc._Alloc_default.showMemorypool();
//	//	cout << "-----------------" << alloc._Alloc_malloc.__default_alloc_oom_handler;
//	cout << endl;
//	A **array = new A*[PTRNUM];
//	int *tempsize = new int[PTRNUM];
//	for (int i = 0; i < NUM; ++i){
//
//		int n = rand() % PTRNUM;
//		if (n > 20) {
//			n = n;
//		}
//
//		int size = rand() % PTRNUM + 129;
//		if (size > 96 && size <= 104){
//			int ddd;
//			ddd = 0;;
//		}
//		for (int j = 0; j < n; ++j){
//			array[j] = alloc.allocate(size);
//			tempsize[j] = size;
//			if (array[j] == NULL){
//				return 1;
//			}
//		}
//		for (int j = 0; j < n; ++j){
//			//int size = rand() % 128 + 1;  //1 - 128
//			alloc.deallocate(array[j], tempsize[j]);
//		}
//		//	cout << ++counter<<"\t"<< size<< endl;
//		/*p = malloc(size);
//		free(p);*/
//
//	}
//	alloc._Alloc_default.showMemorypool();
//	delete[]tempsize;
//	delete[] array;
//	cout << alloc._Alloc_default.size();
//	return 0;
//}
