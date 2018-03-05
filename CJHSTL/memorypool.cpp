#include "memorypool.h"
#include "1jjalloc.h"

#include <stdlib.h>
#include <iostream>



using namespace CJH;

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
	
	std::cout << "Memorypool 构造\n";
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
	obj = (char*) CJH::alloc_malloc::allocate(nobj * nbytes);
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
		if(n - nbyte > 0) recover(cptr + nbyte, n - nbyte);
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

void* Memorypool::locate8(void *ptr,size_t n){
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

//Memorypool memory;
//Memorypool mp;
//Memorypool _Mp;
















//内存池测试代码
//clock_t start, end;
//	start = clock();
//	/*std::allocator<int> s;
//	s.destroy()*/
//	/**
//	
//		内存池的计数 有大问题
//	*/
//	mp.showMemorypool();
//	cout << "-----------------------" << endl;
//	srand(time(0));
//	for (int i = 0; i < __NMEMTABLE; ++i){
//		int n = rand() % 20;
//		int nbyte = (i + 1)*__ALIGN;
//		for (int j = 0; j < n; ++j){
//			void *ptr;
//			ptr = mp.allocate(nbyte);
//			memset(ptr, 0, nbyte);
//			mp.recover(ptr, mp._ROUND_UP(nbyte));
//		}
//		printf("%dbyte\t内存池%d号\t使用%2d个\t剩余%d个\n",
//			nbyte, i + 1, n, mp.getcount_fixed(nbyte));
//	}
//
//
//	cout << "-----------------------" << endl;
//	mp.showMemorypool();
//	end = clock();
//	cout << "时间:" << end - start << endl;
//
//	cout << "last" << endl;
//	start = clock();
//	int counter = 0;
//
//void **array = new void*[PTRNUM];
//int *tempsize = new int[PTRNUM];
//for (int i = 0; i < NUM; ++i){
//	
//	int n = rand() % PTRNUM;
//	if (n > 20) {
//		n = n;
//	}

//	int size = rand() % 128 + 1;  //1 - 128
//	for (int j = 0; j < n; ++j){
//		array[j] = mp.allocate(size);
//		tempsize[j] = size;
//		if (array[j] == NULL){
//			return 1;
//		}
//	}
//	for (int j = 0; j < n; ++j){
//		//int size = rand() % 128 + 1;  //1 - 128
//		mp.recover(array[j],mp._ROUND_UP(tempsize[j]));
//	}
////	cout << ++counter<<"\t"<< size<< endl;
//	/*p = malloc(size);
//	free(p);*/
//	
//}
//
//delete[] array;
//end = clock();
//mp.showMemorypool();
//cout << end - start << endl;