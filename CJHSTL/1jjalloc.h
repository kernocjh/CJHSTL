#ifndef __1JJALLOC_H__
#define __1JJALLOC_H__

// ptrdiff_t  通常过来保存两个指针相减的值   
//typedef __int64 ptrdiff_t;  // 等效于typedef long int ptrdiff_t


#include <cstdlib>
#include <new>
#include "config.h"

__CJH_BEGIN

#define __THROW_BAD_ALLOC (throw std::bad_alloc())
//#if 0
//#	include <new>
//#	define __THROW_BAD_ALLOC (throw bad_alloc) 
//#elif !defined(__THROW_BAD_ALLOC)
//#	include <iostream>
//#	define __THROW_BAD_ALLOC cerr << "out of memory" << endl;exit(1)
//#endif  
	
	typedef void(*oom_handler)();
	template<int inst>
	class __default_alloc{
		public :
		static void(*__default_alloc_oom_handler)();

	private:
		static void* oom_malloc(size_t n){
			void(*my_handler)();
			void *ptr;
			while (1)
			{
				my_handler = __default_alloc_oom_handler;
				if (0 == my_handler){ 
					__THROW_BAD_ALLOC; 
				}
				(*my_handler)();
				ptr = malloc(n);
				if (ptr) return ptr;
			}
		}
		static void* oom_realloc(void *, size_t n = 0){
			void(*my_handler)();
			void *ptr;
			while (1){
				my_handler = __default_alloc_oom_handler;
				if (0 == my_handler){ __THROW_BAD_ALLOC; }
				(*my_handler)();
				ptr = malloc(n);
				if (ptr) return ptr;
			}
		}
		
		
	public:
		

	public:
		/*__default_alloc(){
			cout << "__default_alloc 构造";
		}*/
		static void *allocate(size_t n){
			void *ptr = NULL;
			
			ptr = malloc(n);
		//	ptr = malloc(n);
			if (NULL == ptr){
				try{
					ptr = oom_malloc(n);
				}
				catch (std::bad_alloc e){
					throw;
				}
			}
			return ptr;
		}
		static void *reallocate(void *p, size_t oldsize, size_t newsize){
			void *ptr;
			ptr = realloc(p, newsize);
			if (NULL == ptr){
				try{
					ptr = oom_realloc(p, newsize);
				}
				catch (std::bad_alloc e){
					throw;
				}
			}
			return ptr;
		}
		static void deallocate(void *p, size_t n= 0){
			free(p);
		}
		
		static void(*set_oom_handler(void(*oom_new_hander)()))() {
			void(*oom_old_handler)() = __default_alloc_oom_handler;
			__default_alloc_oom_handler = oom_new_hander;
			return oom_old_handler;
		}
	};
	template<int inst>
	oom_handler __default_alloc<inst>::__default_alloc_oom_handler = (oom_handler)0;

	typedef __default_alloc<0> alloc_malloc;



__CJH_END
//void(*__default_alloc<inst>::__default_alloc_oom_handler)() = (oom_handler)0;
#endif 