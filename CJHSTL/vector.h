#ifndef __VECTOR_H__
#define __VECTOR_H__

#include "allocator.h"
#include "iterator.h"
#include "config.h"

__CJH_BEGIN
using namespace CJH;

	//��������ʵ��
	
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
		void deallocate(){   // Recycling space ��@notes:all element must called destructor��
			if (start != end_of_storage){ // free space
				Alloc::deallocate(CJH::addressof(*start), (end_of_storage - start)*sizeof(_Ty));

				//update the iterator
				finish = start = end_of_storage = iterator((pointer_type)0);
			}
		}

		/***
		ֵ����һ������, ��Ҫ�����쳣��ȫ�ĵڶ����ȼ��� ���ɹ��ͳɹ��� ���ɹ��ͻָ�����������֮ǰ��״̬
		�����ǣ�if�еĴ��벻�ܱ�֤���ǻ���Ҫ��һ���������ܱ�֤�쳣��ȫ�ȼ�  "ǿ�ұ�֤"
		�ֽ׶ε��쳣��ȫ�ȼ��ǣ�  ������ŵ

		Ŀǰ������Ϊinsert_aux��Ϊ�쳣��ȫ�ȼ�  "ǿ�ұ�֤"�ĺ���
		*/
		void insert_aux(iterator position, const _Ty& value){
			if (finish != end_of_storage){
				//�ڱ��ÿռ����ʼλ�ã� �����vector���һ��Ԫ�����
				try{
					construct(finish, *(finish - 1));
				}
				catch (...){
					throw;
				}
				//_Ty value_copy = value;// ����ɾ�����ٲ���Ҫ���쳣�׳��� //����Ϊ�׳��쳣��   
				try{
					CJH::copy_backward(position, finish - 1, finish);
				}
				catch (...){
					CJH::destroy(CJH::addressof(*finish));
					throw;
				}
				(*position) = value;	//���ܴ����쳣�׳��㣬 ������copy_backward��û���׳��쳣��˵�����ƹ��캯
										//�����õõ������׳��쳣�� ���Ǵ˽����ڶ��̵߳����޷��õ�����
				//last, update finish iterator
				++finish;
			}
			else{  //�˴����ܱ�֤�쳣��ȫ�еȼ�Ϊ   ǿ�ұ�֤
				const size_type oldsize = size();
				const size_type newsize = (oldsize == 0) ? 1 : oldsize * 2; //����Ҫ���ǵ�ǰ�ռ�Ϊ0�����
				size_type nbyte = newsize * sizeof(_Ty);
				pointer_type newptr = NULL;
				try{                      //���Ǽ��bug����Ĵ�����ΪҪ��֤��ȫ��
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
					destroy(newstart, newfinish); //��Ȼuninitialized_copy�е���ʼ�������ʱ���лع����Ѿ���ʼ���ռ�����ã� ���������������γ�ʼ��
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
				CJH::destroy(start, finish);  //���п�û��  ��Ϊ�ú���һ���׳��쳣 ���Ѿ���ʼ���Ŀռ���Ѿ���ԭ
				Alloc::deallocate(CJH::addressof(*start), sizeof(_Ty)*n);
				throw;
			}
			//����ָ��ˮλ
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
			iterator newfinish = newstart;   // must init newfinish, ����2��20������22.15��������bug�� newfinish û�г�ʼ����
			try{
				newfinish = CJH::uninitialized_copy(v.start, v.finish, newstart);
			}
			catch (...){
				CJH::destroy(newstart, newfinish);//���п�û��  ��Ϊ�ú���һ���׳��쳣 ���Ѿ���ʼ���Ŀռ���Ѿ���ԭ
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


		iterator begin() const{   // ���
			return start;
		}

		iterator end() const{  // ���
			return finish;
		}

		size_type size() const{  // ���
			return size_type(CJH::addressof(*finish) - CJH::addressof(*start));
		}
		size_type capacity() { // ���
			return size_type(CJH::addressof(*end_of_storage) - CJH::addressof(*start));
		}
		bool empty() const{  // ���
			return start == finish;
		}

		
		reference_type operator[](int n) const{ // ���
			iterator it = begin();
			return *(it + n);
		}
		reference_type front(){ //���
			return *start;
		}
		reference_type back(){  //���
			return *(end() - 1);
		}
		//�ṩ�쳣��ȫ
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
			finish = finish - (last - first);  //���ﵽ��ʹ����һ�����п���
				//finish = it;
			return first;
		}


		void insert(iterator positon, size_type n, const _Ty& value){
			if (n != 0){
				if (capacity() - size() >= n){ //���ÿռ��㹻
					const size_type element_after(finish - positon);
					iterator oldfinish = finish;
					if (element_after > n){ //������Ժ��Ԫ�ش���n
						//���ƶ�������Ժ��Ԫ���ڳ�ʼ��
						
						//�Ƚ�finishi������ڴ��ʼ��n��
						CJH::uninitialized_copy(finish - n, finish, finish);//�����ڴ����
						finish = finish + n;
						//���ƶ�ԭ��ʣ�µ�Ԫ��
						CJH::copy_backward(positon, oldfinish - n, oldfinish); // ���ƶ���
						CJH::fill(positon, positon + n, value);
					}
					else{// �����֮�������Ԫ�ظ���С�ڵ�������Ԫ�ظ���
						CJH::uninitialized_fill_n(finish, n - element_after, value);
						finish = finish + (n - element_after);
						CJH::uninitialized_copy(positon, oldfinish, finish);
						finish = finish + element_after;
						CJH::fill(positon, oldfinish, value);
					}
				}
				else{//���ÿռ䲻��

					//�¿ռ�����Ϊ��С  ����Ϊԭ����������С����n
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


					//�������һ��֪ʶ������ڴ�ĸ����У� һ�����ڴ��ʼ��ʧ��
					//�����׳��쳣�� ���µ��ڴ��������������������� Ȼ���ڻ����ڴ�

					try{
						//��start��position��Ԫ�س�ʼ�����µ��ڴ�
						newfinish = CJH::uninitialized_copy(start, positon, newstart);
						//�������ڴ������n��Ԫ��
						newfinish = CJH::uninitialized_fill_n(newfinish, n, value);
						//�ٽ�ʣ�µ�Ԫ�س�ʼ�����ڴ浱��ȥ
						newfinish = CJH::uninitialized_copy(positon, finish, newfinish);
					}
					catch (...){
						CJH::destroy(newstart, newfinish);
						Alloc::deallocate(CJH::addressof(*newstart), nbyte);
						throw;
					}

					//���վɿռ�
					CJH::destroy(start, finish);
					deallocate();

					//�����¿ռ�ˮλ
					start = newstart;
					finish = newfinish;
					end_of_storage = newstart + newsize;
				}
			}
		}
		void insert(iterator position, const _Ty& value){ // ���
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
		void resize(size_type newsize){  //���
			resize(newsize, value_type());
		}

		void reserve(const size_type newsize){
			/*
				�ú���ֻ�ܵ���Ԥ���ռ�
			*/
			if (capacity() >= newsize){
				return;
			}
			else{

				//�ڴ������֪ʶ��
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
					// һ������ͻع�
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

		void clear(){ // ���
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






//����  �汾��
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







//���԰汾һ  ��Ҫ�������pop_back    push_back   size    capacity �Ĳ���
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
//cout << "ʱ��:" << end - start << endl;
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
