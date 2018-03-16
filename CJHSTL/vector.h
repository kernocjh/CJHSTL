#ifndef __VECTOR_H__
#define __VECTOR_H__

#include "allocator.h"
#include "iterator.h"
#include "config.h"

__CJH_BEGIN
using namespace CJH;

	//迭代器的实现
	
	template<class _Ty, class Alloc = CJH::allocator<_Ty>, 
			class Category = bidirectinal_iterator_tag>
	class _vector_iterator :public iterator_base<Category, _Ty>{
	public:
		typedef iterator_base<Category, _Ty> _Mybase;
		typedef typename _Mybase::pointer_type pointer_type;
		typedef typename _Mybase::value_type value_type;
		typedef typename _Mybase::difference_type difference_type;
		typedef typename _Mybase::reference_type reference_type;
		typedef typename _Mybase::iterator_category iterator_category;

		typedef _vector_iterator<_Ty, Alloc, Category> iterator;
		typedef _vector_iterator<_Ty, Alloc, Category> self;
		pointer_type ptr;
	public:
		_vector_iterator(const pointer_type t = 0) : iterator_base<Category, _Ty>(), ptr(t){

		}

		_vector_iterator(const self& x) :ptr(x.ptr) {

		}

		~_vector_iterator(){

		}

		iterator& operator=(const iterator& x){
			if (this != &x)  //
				ptr = x.ptr;
			return *this;
		}

		bool operator == (const self& x) const{
			return this->ptr == x.ptr;
		}
		bool operator != (const self& x) const{
			return this->ptr != x.ptr;
		}

		value_type& operator *() const{ return *ptr; }
		pointer_type operator ->() const{ return ptr; }

		self& operator ++(){
			ptr++;
			return *this;
		}

		self operator++ (difference_type){
			self tmp = *this;
			++*this;
			return tmp;
		}

		self& operator --(){
			--ptr;
			return *this;
		}

		self operator--(difference_type){
			self tmp = *this;
			--*this;
			return tmp;
		}

		self operator+(difference_type n){
			/*ptr = ptr + n;
			iterator tmp = *this;
			ptr = ptr - n;*/
			return self(ptr + n);
		}

		self operator-(difference_type n){
			/*ptr = ptr - n;
			iterator tmp = *this;
			ptr = ptr + n;*/
			return self(ptr - n);
		}

		difference_type operator-(const iterator &x){
			return difference_type(ptr - x.ptr);
		}

		void swap(self& x){
			if (this != &x){
				CJH::swap(ptr, x.ptr);
			}
		}
	};

	

	template<class _Ty, class Alloc = CJH::allocator<_Ty> >
	class vector{

	private:

	public:
		typedef _Ty value_type;
		typedef value_type* pointer_type;
		typedef value_type& reference_type;
		typedef size_t size_type;
		typedef _vector_iterator<_Ty, Alloc, bidirectinal_iterator_tag> iterator;
		typedef typename iterator::difference_type difference_type;
		typedef vector<_Ty, Alloc> self;
	protected:
		iterator start;
		iterator finish;
		iterator end_of_storage;
		void deallocate(){   // Recycling space （@notes:all element must called destructor）
			if (start != end_of_storage){ // free space
				Alloc::deallocate(CJH::addressof(*start), (end_of_storage - start)*sizeof(_Ty));

				//update the iterator
				finish = start = end_of_storage = iterator((pointer_type)0);
			}
		}

		/***
		值得新一步考量, 需要满足异常安全的第二个等级， 若成功就成功， 不成功就恢复到函数调用之前的状态
		问题是：if中的代码不能保证但是还需要进一步分析才能保证异常安全等级  "强烈保证"
		现阶段的异常安全等级是：  基本承诺

		目前初步认为insert_aux是为异常安全等级  "强烈保证"的函数
		*/
		void insert_aux(iterator position, const _Ty& value){
			if (finish != end_of_storage){
				//在备用空间的起始位置， 用最后vector最后一个元素填充
				try{
					construct(finish, *(finish - 1));
				}
				catch (...){
					throw;
				}
				//_Ty value_copy = value;// 将其删除减少不必要的异常抛出点 //可能为抛出异常点   
				try{
					CJH::copy_backward(position, finish - 1, finish);
				}
				catch (...){
					CJH::destroy(CJH::addressof(*finish));
					throw;
				}
				(*position) = value;	//可能存在异常抛出点， 但是在copy_backward中没有抛出异常就说明复制构造函
										//数调用得当不会抛出异常， 但是此结论在多线程当中无法得到保障
				//last, update finish iterator
				++finish;
			}
			else{  //此代码能保证异常安全中等级为   强烈保证
				const size_type oldsize = size();
				const size_type newsize = (oldsize == 0) ? 1 : oldsize * 2; //还需要考虑当前空间为0的情况
				size_type nbyte = newsize * sizeof(_Ty);
				pointer_type newptr = NULL;
				try{                      //这是检查bug补充的代码因为要保证安全性
					newptr = Alloc::allocate(nbyte);
				}
				catch (...){
					Alloc::deallocate(newptr, nbyte);
					throw;
				}
				iterator newstart(newptr);
				iterator newfinish = newstart;
				try{
					newfinish = CJH::uninitialized_copy(start, position, newstart);
					construct(newfinish, value);
					++newfinish;
					newfinish = CJH::uninitialized_copy(position, finish, newfinish);
				}
				catch (...){
					// error occur, rollback
					destroy(newstart, newfinish); //虽然uninitialized_copy有当初始化出错的时候有回归其已经初始化空间的作用， 但是在这里有两次初始化
					Alloc::deallocate(CJH::addressof(*newstart), nbyte);
					throw;
				}

				CJH::destroy(begin(), end());
				deallocate();
				//udpate  water level
				start = newstart;
				finish = newfinish;
				end_of_storage = start + newsize;
			}
		}
	public:
		vector() : start(0), finish(0), end_of_storage(0){
			
		}

		explicit vector(size_type n) :start(0), finish(0), 
			end_of_storage(0) {
			pointer_type ptr = NULL;
			try{
				ptr = Alloc::allocate(sizeof(_Ty)*n);
			}
			catch (...){
				Alloc::deallocate(ptr, n * sizeof(_Ty));
				throw;
			}
			start = iterator(ptr);
			finish = start;
			end_of_storage = start + n;
		}

		vector(size_type n, const _Ty& value) :vector(n){

			try{
				finish = CJH::uninitialized_fill_n(start, n, value);
			}
			catch (...){
				CJH::destroy(start, finish);  //可有可没有  因为该函数一旦抛出异常 其已经初始化的空间便已经还原
				Alloc::deallocate(CJH::addressof(*start), sizeof(_Ty)*n);
				throw;
			}
			//更新指针水位
			finish = start + n;
			end_of_storage = start + n;
		}

		

		//template<class Iter,
		//	typename value = iterator_traits<Iter>::value_type>
		//vector(Iter first, Iter last){
		//	//
		//}

		vector(const self& v){
			pointer_type newptr = NULL;//newptr
			size_type newsize(CJH::addressof(*(v.end_of_storage)) - CJH::addressof(*(v.start)));
			size_t nbyte = newsize * sizeof(_Ty);
			try{
				newptr = Alloc::allocate(nbyte); 
			}
			catch (...){
				Alloc::deallocate(newptr, nbyte);
				throw;
			}
			
			iterator newstart(newptr);
			iterator newfinish = newstart;   // must init newfinish, （在2月20日晚上22.15检查出来的bug， newfinish 没有初始化）
			try{
				newfinish = CJH::uninitialized_copy(v.start, v.finish, newstart);
			}
			catch (...){
				CJH::destroy(newstart, newfinish);//可有可没有  因为该函数一旦抛出异常 其已经初始化的空间便已经还原
				Alloc::deallocate(newptr, nbyte);
				throw;
			}
			start = newstart;
			finish = newfinish;
			end_of_storage = start + newsize;
		}

		self& operator=(const self& v){
			if (this != &v) {
				try{
					self tmp(v);
					swap(tmp);
				}
				catch (...){
					throw;
				}
			}
			return *this;

			////1.first  allocate space, if fail, free this space and throw
			//pointer_type newptr = NULL;
			//size_type newsize(CJH::addressof(*(v.end_of_storage)) - CJH::addressof(*(v.start)));
			//size_t nbyte = newsize * sizeof(_Ty);
			//try{
			//	newptr = Alloc::allocate(nbyte);
			//}
			//catch (...){
			//	Alloc::deallocate(newptr, nbyte);
			//	throw;
			//}


			////2. init the space
			//iterator newstart(newptr);
			//iterator newfinish;
			//try{
			//	newfinish = CJH::uninitialized_copy(v.start, v.finish, newstart);
			//}
			//catch (...){
			////	CJH::destroy(newstart, newfinish);
			//	Alloc::deallocate(newptr, nbyte);
			//	throw;
			//}

			////free prev space, and update water level
			//clear();
			//deallocate();
			//start = newstart;
			//finish = newfinish;
			//end_of_storage = start + newsize;
			//return *this;
		}

		~vector(){
			clear();
			deallocate();
		}


		iterator begin() const{   // 完成
			return start;
		}

		iterator end() const{  // 完成
			return finish;
		}

		size_type size() const{  // 完成
			return size_type(CJH::addressof(*finish) - CJH::addressof(*start));
		}
		size_type capacity() { // 完成
			return size_type(CJH::addressof(*end_of_storage) - CJH::addressof(*start));
		}
		bool empty() const{  // 完成
			return start == finish;
		}

		
		reference_type operator[](int n) const{ // 完成
			iterator it = begin();
			return *(it + n);
		}
		reference_type front(){ //完成
			return *start;
		}
		reference_type back(){  //完成
			return *(end() - 1);
		}
		//提供异常安全
		void push_back(const _Ty& x){
			if (finish != end_of_storage){
				construct(finish, x);
				++finish;
			}
			else{
				try{
					insert_aux(end(), x);
				}
				catch (...){
					throw;
				}
			}
		}
		void pop_back(){
			--finish;
			CJH::destroy(CJH::addressof(*finish));
		}

		iterator erase(iterator position){
			if (position + 1 != finish){
				CJH::copy(position + 1, end(), position);
			}

			--finish;
			destroy(CJH::addressof(*finish));
			return position;
		}
		iterator erase(iterator first, iterator last){
			iterator it = CJH::copy(last, finish, first);
			destroy(it, finish);
			finish = finish - (last - first);  //这里到底使用哪一个还有考究
				//finish = it;
			return first;
		}


		void insert(iterator positon, size_type n, const _Ty& value){
			if (n != 0){
				if (capacity() - size() >= n){ //备用空间足够
					const size_type element_after(finish - positon);
					iterator oldfinish = finish;
					if (element_after > n){ //插入点以后的元素大于n
						//先移动插入点以后的元素在初始化
						
						//先将finishi后面的内存初始化n个
						CJH::uninitialized_copy(finish - n, finish, finish);//构建内存对象
						finish = finish + n;
						//在移动原来剩下的元素
						CJH::copy_backward(positon, oldfinish - n, oldfinish); // 复制对象
						CJH::fill(positon, positon + n, value);
					}
					else{// 插入点之后的现有元素个数小于等于新增元素个数
						CJH::uninitialized_fill_n(finish, n - element_after, value);
						finish = finish + (n - element_after);
						CJH::uninitialized_copy(positon, oldfinish, finish);
						finish = finish + element_after;
						CJH::fill(positon, oldfinish, value);
					}
				}
				else{//备用空间不够

					//新空间容量为大小  设置为原来的容量大小加上n
					const size_type oldsize = capacity();
					const size_type newsize = oldsize + (oldsize > n ? (oldsize) : (n));

					const size_t nbyte = newsize * sizeof(_Ty);
					pointer_type newptr = NULL; 
					try{
						newptr = Alloc::allocate(nbyte);
					}
					catch (...){
						Alloc::deallocate(newptr, nbyte);
						throw;
					}
					iterator newstart(newptr);
					iterator newfinish = newstart;


					//这里存在一个知识点关于内存的复制中， 一旦新内存初始化失败
					//将会抛出异常， 将新的内存给中组个调用析构函数， 然后在回收内存

					try{
						//将start到position的元素初始化到新的内存
						newfinish = CJH::uninitialized_copy(start, positon, newstart);
						//再在新内存中填充n个元素
						newfinish = CJH::uninitialized_fill_n(newfinish, n, value);
						//再讲剩下的元素初始化到内存当中去
						newfinish = CJH::uninitialized_copy(positon, finish, newfinish);
					}
					catch (...){
						CJH::destroy(newstart, newfinish);
						Alloc::deallocate(CJH::addressof(*newstart), nbyte);
						throw;
					}

					//回收旧空间
					CJH::destroy(start, finish);
					deallocate();

					//调整新空间水位
					start = newstart;
					finish = newfinish;
					end_of_storage = newstart + newsize;
				}
			}
		}
		void insert(iterator position, const _Ty& value){ // 完成
			insert(position, 1, value);
		}
		void resize(size_type newsize, const _Ty& x){
			if (newsize < size()){
				erase(start + newsize, finish);
			}
			else{
				insert(finish, newsize - size(), x);
			}
		}
		void resize(size_type newsize){  //完成
			resize(newsize, value_type());
		}

		void reserve(const size_type newsize){
			/*
				该函数只能调大预备空间
			*/
			if (capacity() >= newsize){
				return;
			}
			else{

				//内存操作的知识点
				const size_type oldsize = size();
				size_type nbyte = newsize * sizeof(_Ty);
				pointer_type newptr = NULL; 
				try{
					newptr = Alloc::allocate(nbyte);
				}
				catch (...){
					Alloc::deallocate(newptr, nbyte);
					throw;
				}
				iterator newstart(newptr);
				iterator newfinish = newstart;
				try{
					newfinish = CJH::uninitialized_copy(begin(), end(), newstart);
				}
				catch (...){
					// 一旦出错就回滚
					CJH::destroy(newstart, newfinish);
					Alloc::deallocate(CJH::addressof(*newstart), nbyte);
					return;
				}

				destroy(begin(), end());
				deallocate();
				
				start = newstart;
				finish = newfinish;
				end_of_storage = start + newsize;
			}
		}

		void clear(){ // 完成
			destroy(start, finish);
			finish = start;
		}

		void swap(self& v){
			if (this != &v) return;
			start.swap(v.start);
			finish.swap(v.finish);
			end_of_storage.swap(v.end_of_storage);
		}
	};


__CJH_END



#endif






//测试  版本二
//#define CIRNUM 25000000
//srand((unsigned int)time(0));
//int counter = 0;
//for (;;){
//	clock_t start, end;
//	start = clock();
//	CJH::vector<int> v;
//	int n = rand() % CIRNUM + CIRNUM;
//	for (int i = 0; i < n; ++i){
//		v.push_back(i + 1);
//	}
//
//	n = rand() % CIRNUM + CIRNUM;
//	for (int i = 0; i < n&&!v.empty(); ++i){
//		v.pop_back();
//	}
//	end = clock();
//	std::cout << "counter = " << ++counter << "   times = " << end - start << std::endl;
//}







//测试版本一  主要是是针对pop_back    push_back   size    capacity 的测试
//clock_t start, end;
//start = clock();
//std::vector<A> v; A a;
//v.erase(v.begin());
//CJH::vector<int>::iterator it;
//v.clear();
//cout << v.size() << "\t" << v.capacity();
//v.pop_back();
////	v.push_back(A());
//cout << v.size() << "\t" << v.capacity();
////	CJH::vector<string>::iterator it;
//srand(time(0));
///*for (int i = 0; i < CIRNUM; ++i){
//int n = rand() % 1000000 + 1000000;
//for (int j = 0; j < n; ++j){
//v.push_back(j+1);
//
//}
//n = rand() % 1000000 + 1000000;
//for (int j = 0; j < n; ++j){
//v.pop_back();
//}
//}*/
////	CJH::allocator<int>::_Alloc_default.showMemorypool();
//
//end = clock();
//cout << "时间:" << end - start << endl;
//cout << "v.size()=" << v.size() << "\n";
//cout << "v.capacity()=" << v.capacity() << endl;
////	v.pop_back();
//////	v.clear();
////	v.reserve(17);
////cout << "v.front()=" << v.front() << endl;
////cout << "v.back()=" << v.back() << endl;
////cout << "v[last]=" << v[v.size() - 1] << endl;
////	copy(v.begin(), v.end(), ostream_iterator<string>(cout, " "));
//cout << "v.size()=" << v.size() << "\n";
//cout << "v.capacity()=" << v.capacity() << endl;
