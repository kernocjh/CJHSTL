#ifndef __MEMORYPOOL_H__
#define __MEMORYPOOL_H__

#include "config.h"
#include "memorypool.h"
#include "1jjalloc.h"
#include <cstring>
__CJH_BEGIN

	union obj{
		union obj *free_list_node;
		void *data;
	};

	struct memtable{
		union obj *free_link_node;
		int count;
		const size_t size;

	};

	enum { __ALIGN = 8 };
	enum { __MAX_BYTES = 128 };
	enum { __NMEMTABLE = __MAX_BYTES / __ALIGN };


	class Memorypool{
	private:
		static void init_mm();
		static void init_hash();
		static inline void put(const size_t key, void *const value);
		static inline void* get(const size_t key);
		static struct memtable table[__NMEMTABLE];
		static union obj **hash;
		static size_t totalSpace;
		static size_t _locate_index(size_t n);
		static bool isinit;
		static bool ishash;
	public:
		static size_t _ROUND_UP(size_t n);
		Memorypool();
		static void* allocate(size_t n);
		static void deallocate(void* ptr, size_t n);
		static void* reallocate(void *ptr, size_t oldsize, size_t newsize);
		static void recover(void *ptr, size_t n);
		static size_t size();
		static size_t getsize_fixed(size_t n);
		static int getcount_fixed(size_t n);
		static void refill(size_t n);
		static void free_allmemory();
		static void showMemorypool();
		static size_t get_key(void *ptr);
		static void* locate8(void *ptr, size_t n);
		~Memorypool();
	};

	typedef Memorypool alloc_default;
	

	size_t Memorypool::totalSpace = 0;
	bool Memorypool::isinit = false;
	bool Memorypool::ishash = false;
	union obj** Memorypool::hash = NULL;
	struct memtable Memorypool::table[__NMEMTABLE] =
	{ { NULL, 0, 8 }, { NULL, 0, 16 },
	{ NULL, 0, 24 }, { NULL, 0, 32 },
	{ NULL, 0, 40 }, { NULL, 0, 48 },
	{ NULL, 0, 56 }, { NULL, 0, 64 },
	{ NULL, 0, 72 }, { NULL, 0, 80 },
	{ NULL, 0, 88 }, { NULL, 0, 96 },
	{ NULL, 0, 104 }, { NULL, 0, 112 },
	{ NULL, 0, 120 }, { NULL, 0, 128 } };
	void Memorypool::init_mm(){
		for (unsigned int i = 0; i < __NMEMTABLE; ++i){
			if (i == 0){
				init_hash();
				table[0].count = 0;
				continue;
			}
			refill((i + 1)*__ALIGN);
		}
	}

	void Memorypool::put(const size_t key, void *const value){
		if (value == NULL)
			return;
		*(union obj**)value = hash[key];
		hash[key] = (union obj*)(value);
	}

	void* Memorypool::get(const size_t key) {
		union obj* obj = hash[key];
		if (obj != NULL){
			hash[key] = *(union obj**)obj;
		}
		return (void*)obj;
	}

	void Memorypool::init_hash() throw(){
		try{
			hash = (union obj**)CJH::alloc_malloc::allocate(MM_HASHSIZE*(sizeof(void*)));
			memset((void*)hash, 0, MM_HASHSIZE*(sizeof(void*)));
		}
		catch (...){
			hash = NULL;
			throw;
		}
		ishash = true;
	}


	Memorypool::Memorypool(){

	//	std::cout << "Memorypool 构造\n";
	}
	Memorypool::~Memorypool(){
		free_allmemory();
	}
	size_t Memorypool::_ROUND_UP(size_t n){
		return (n + __ALIGN - 1) & (~(__ALIGN - 1));
	}

	size_t Memorypool::_locate_index(size_t n){
		return _ROUND_UP(n) / __ALIGN - 1;
	}

	size_t Memorypool::getsize_fixed(size_t n){
		size_t index = _locate_index(n);
		return table[index].count*table[index].size;
	}

	size_t Memorypool::size(){
		size_t sum = 0;
		for (unsigned int i = 0; i < __NMEMTABLE; ++i)
			sum += table[i].count*table[i].size;
		return sum;
	}

	int Memorypool::getcount_fixed(size_t n){
		return table[_locate_index(n)].count;
	}

	void Memorypool::free_allmemory(){

	}
	void Memorypool::showMemorypool(){
		for (unsigned int i = 0; i < (__NMEMTABLE); ++i){
			std::cout << table[i].size << "byte\t内存池"
				<< i + 1 << "号\t剩余" << table[i].count << "个\t首地址";
			printf("%x\n", table[i].free_link_node);
		}
		std::cout << "\n";
	}

	void Memorypool::refill(size_t n){
		size_t nbytes = _ROUND_UP(n);
		size_t index = _locate_index(n);
		unsigned int nobj = 20;
		char* obj;
		union obj **temp = NULL;
		obj = (char*)CJH::alloc_malloc::allocate(nobj * nbytes);
		if (NULL == obj){
			return;
		}
		else{
			temp = (union obj**)(obj + (nobj - 1) * nbytes);
			*(union obj**)(obj + (nobj - 1) * nbytes) = table[index].free_link_node;
			for (int i = nobj - 2; i >= 0; --i){
				*(union obj**)(obj + (i)* nbytes) = (union obj*)(obj + (i + 1) * nbytes);
			}
			table[index].free_link_node = (union obj*)obj;
			table[index].count += nobj;
		}

	}

	void* Memorypool::allocate(size_t n){
		if (0 == n)
			return (void*)0;
		if (!isinit){
			isinit = true;
			init_mm();
		}
		union obj *obj = NULL;
		size_t index = _locate_index(n);
		size_t nbytes = _ROUND_UP(n);
		if (n > __MAX_BYTES){
			return CJH::alloc_malloc::allocate(n);
		}
		else{
			if (nbytes == MM_OFFSET && ishash && table[index].count > 0){
				for (size_t i = 0; i < MM_HASHSIZE; ++i){
					if (hash[i] != NULL){
						obj = hash[i];
						hash[i] = *(union obj**)obj;
						--(table[index].count);
						return obj;
					}
				}
			}
			else
				obj = table[index].free_link_node;
			//		if (nbytes != table[index].size) return (void *)0;
			if (obj == NULL){  // 内存池空间不足，重新填充内存池空间

				//原来的方案
				//refill(n);
				//return allocate(n);
				//若当前空间不够就去nbytes+MM_OFFSET对应的桶里面去搜索
				// 然后把最后的答案划分为nbytes + MM_OFFSET两个部分
				void *ptr = allocate(nbytes + MM_OFFSET);
				obj = (union obj*)ptr;
				ptr = (char*)ptr + nbytes;

				index = _locate_index(MM_OFFSET);
				nbytes = _ROUND_UP(MM_OFFSET);

				put(get_key(ptr), ptr);
				/**(union obj**)(ptr) = table[index].free_link_node;
				table[index].free_link_node = (union obj*)ptr;*/

				++(table[index].count);
				return (void*)obj;
			}
			else{
				obj = table[index].free_link_node;
				table[index].free_link_node = obj->free_list_node;
				--(table[index].count);
				return (void*)obj;
			}

		}
	}

	/*
	void* Memorypool::reallocate(void *ptr, size_t oldsize, size_t newsize){
	size_t oldnbyte, newnbyte;
	oldnbyte = _ROUND_UP(oldsize);
	newnbyte = _ROUND_UP(newsize);

	void *result;
	result = allocate(newsize);
	if (NULL == result){
	return (void*)0;
	}
	else{
	memcpy(result, ptr, oldsize);
	deallocate(ptr, oldsize);
	return result;
	}
	}
	*/
	void Memorypool::deallocate(void *ptr, size_t n){
		if (0 == n)  return;
		size_t nbyte = _ROUND_UP(n);
		size_t index = _locate_index(nbyte);
		char *cptr = (char*)ptr;
		if (n > __MAX_BYTES){
			//	CJH::Memorypool::showMemorypool();
			CJH::alloc_malloc::deallocate(ptr, nbyte);
			//free(ptr);
		}
		else{
			void *mm_fragment;
			mm_fragment = locate8(cptr + nbyte, MM_OFFSET);
			if (mm_fragment != NULL){

				deallocate(cptr, nbyte + MM_OFFSET);
				return;
			}
			if (nbyte == MM_OFFSET && ishash){
				put(get_key(ptr), ptr);
				table[index].count += 1;
				return;
			}
			*(union obj**)(cptr) = table[index].free_link_node;
			table[index].free_link_node = (union obj*)cptr;
			table[index].count += 1;
		}
	}
	void Memorypool::recover(void *ptr, size_t n){
		if (0 == n)
			return;
		size_t nbyte;
		if (n % __ALIGN == 0){
			nbyte = _ROUND_UP(n);
		}
		else{
			nbyte = _ROUND_UP(n) - __ALIGN;
		}
		char *cptr = (char*)ptr;
		if (n > __MAX_BYTES){
			/*
			if (n != nbyte){
			free(cptr + nbyte);
			}*/

			int index = 0;
			for (unsigned int i = 1; i < __NMEMTABLE; ++i){
				if (table[index].count < table[i].count){
					index = i;
				}
			}

			size_t byte = (index + 1) * __ALIGN;  // 代表将回收的内存划分为单位为byte的小单元
			unsigned int number = nbyte / byte;// 回收的内存可以划分为number个 byte的小单元
			*(union obj**)(cptr + (number - 1)*byte) = table[index].free_link_node;
			//for (int i = number - 2; i > 0;--i)  这一串代码有大bug
			for (unsigned int i = 0; i < number - 1; ++i){
				*(union obj**)(cptr + (i)*byte) = (union obj*)(cptr + i * nbyte);
			}

			table[index].free_link_node = (union obj*)cptr;
			table[index].count += number;
			if (n - nbyte > 0) recover(cptr + nbyte, n - nbyte);
		}
		else if (n >= __ALIGN){
			/*if (n != nbyte){
			free(cptr + nbyte);
			}*/
			size_t index = _locate_index(nbyte);
			*(union obj**)(cptr) = table[index].free_link_node;
			table[index].free_link_node = (union obj*)cptr;
			table[index].count += 1;
			if (n - nbyte > 0)
				recover(cptr + nbyte, n - nbyte);
		}
		else{
			if (0 != n) CJH::alloc_malloc::deallocate(ptr, n);
		}


	}

	size_t Memorypool::get_key(void *const ptr) {
		size_t num((size_t)ptr);
		return num%MM_HASHSIZE;
	}

	void* Memorypool::locate8(void *ptr, size_t n){
		size_t nbyte = _ROUND_UP(n);
		size_t index = _locate_index(nbyte);
		size_t key = get_key(ptr);
		union obj *cur = NULL;
		union obj *pre = NULL;
		if (hash[key]){
			cur = hash[key];
			for (cur; cur != NULL; cur = *(union obj**)cur){
				if (cur == ptr) break;
				pre = cur;
			}
			if (!cur) return NULL;

			--(table[index].count);
			if (pre == NULL){
				hash[key] = *(union obj**)cur;
				return cur;
			}
			else{
				*(union obj**)pre = *(union obj**)cur;
				return cur;
			}
		}

		return NULL;
	}

__CJH_END




#endif
