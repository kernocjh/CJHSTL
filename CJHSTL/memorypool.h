#ifndef __MEMORYPOOL_H__
#define __MEMORYPOOL_H__

#include "config.h"

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
	

__CJH_END
#endif