#include "memorypool.h"
#include "1jjalloc.h"

#include <stdlib.h>
#include <iostream>



using namespace CJH;


size_t Memorypool::totalSpace = 0;
struct memtable Memorypool::table[__NMEMTABLE] = {	{ 0, 0, 8 },{ 0, 0, 16 },
													{ 0, 0, 24 },{ 0, 0, 32 },
													{ 0, 0, 40 }, { 0, 0, 48 }, 
													{ 0, 0, 56 }, { 0, 0, 64 } ,
													{ 0, 0, 72 }, { 0, 0, 80 } ,
													{ 0, 0, 88 }, { 0, 0, 96 } ,
													{ 0, 0, 104 }, { 0, 0, 112 } ,
													{ 0, 0, 120 }, { 0, 0, 128 } };
Memorypool::Memorypool(){
	for (unsigned int i = 0; i < __NMEMTABLE; ++i){
		table[i].size = (const size_t)(i + 1)*__ALIGN;
		table[i].count = 0;
		table[i].free_link_node = (union obj*)0;
		totalSpace = 0; 
	//	refill((i + 1)*__ALIGN);
	}
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
	union obj *obj = NULL;
	size_t index = _locate_index(n);
	size_t nbytes = _ROUND_UP(n);
	if (n > __MAX_BYTES){
		return CJH::alloc_malloc::allocate(n);
	}
	else{

		obj = table[index].free_link_node;
		if (nbytes != table[index].size) return (void *)0;
		if (obj == NULL){  // 内存池空间不足，重新填充内存池空间
			refill(n);
			return allocate(n);
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
	size_t nbyte;
	nbyte = _ROUND_UP(n);
	char *cptr = (char*)ptr;
	if (n > __MAX_BYTES){
		CJH::alloc_malloc::deallocate(ptr, n);
		//free(ptr);
	}
	else{
		size_t index = _locate_index(nbyte);
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