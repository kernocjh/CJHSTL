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

		typedef _vector_iterator<_Ty> iterator;

		pointer_type ptr;
	public:


//		iterator operator =(const )
		_vector_iterator(_Ty* t = 0) : iterator_base<Category, _Ty>(), ptr(t){
			//ptr = t;
		}
		~_vector_iterator(){

		}

		//iterator& operator=(const iterator& x){
		//	return *this;
		//}

		bool operator == (const iterator& x) const{
			return this->ptr == x.ptr;
		}
		bool operator != (const iterator& x) const{
			return this->ptr != x.ptr;
		}

		value_type& operator *() const{ return *ptr; }
		pointer_type operator ->() const{ return ptr; }

		iterator& operator ++(){
			ptr++;
			return *this;
		}

		iterator operator++ (int){
			iterator tmp = *this;
			++*this;
			return tmp;
		}

		iterator& operator --(){
			--ptr;
			return *this;
		}

		iterator operator--(int){
			iterator tmp = *this;
			--*this;
			return tmp;
		}

		iterator operator+(int n){
			/*ptr = ptr + n;
			iterator tmp = *this;
			ptr = ptr - n;*/
			return iterator(ptr + n);
		}

		/*difference_type operator+(const iterator &x){
			return this->ptr  x.ptr;
		}*/

		iterator operator-(int n){
			/*ptr = ptr - n;
			iterator tmp = *this;
			ptr = ptr + n;*/
			return iterator(ptr - n);
		}

		difference_type operator-(const iterator &x){
			return difference_type(this->ptr - x.ptr);
		}

		/*bool operator = (const iterator){
			return ptr != NULL;
		}*/
		
	};

	

	template<class _Ty, class Alloc = allocator<_Ty> >
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
		void deallocate(){
			if (start != end_of_storage){
				Alloc::deallocate(CJH::addressof(*start), (end_of_storage - start)*sizeof(_Ty));
				finish = start = end_of_storage = 0;
			}
		}


		void insert_aux(iterator position, const _Ty& value){
			if (finish != end_of_storage){
				//�ڱ��ÿռ����ʼλ�ã� �����vector���һ��Ԫ�����
				construct(finish, *(finish - 1));
				++finish;
				_Ty value_copy = value;
				CJH::copy_backward(position, finish - 2, (finish - 1));
				(*position) = value_copy;
			}
			else{
				const size_type oldsize = size();
				const size_type newsize = (oldsize == 0) ? 1 : oldsize * 2;
				size_type nbyte = newsize * sizeof(_Ty);
				pointer_type newptr = Alloc::allocate(nbyte);
				iterator newstart(newptr);
				iterator newfinish = newstart;
				try{
					newfinish = CJH::uninitialized_copy(start, position, newstart);
					construct(newfinish, value);
					++newfinish;
					newfinish = CJH::uninitialized_copy(position, finish, newfinish);
				}
				catch (...){
					// һ������ͻع�
					destroy(newstart, newfinish);
					Alloc::deallocate(CJH::addressof(*newstart), nbyte);
					throw;
				}

				destroy(begin(), end());
				deallocate();

				start = newstart;
				finish = newfinish;
				end_of_storage = start + newsize;
			}
		}
	public:
		vector() : start(0), finish(0), end_of_storage(0){
			
		}

		vector(const _Ty& value) :start(Alloc::allocate(sizeof(_Ty))), finish(start + 1), end_of_storage(finish) {
			construct(CJH::addressof(*start), value);
		}

		vector(size_type n, const _Ty& value) :start(Alloc::allocate(sizeof(_Ty)*n)){

			try{
				finish = CJH::uninitialized_fill_n(start, n, value);
			}
			catch (...){
				CJH::destroy(start, finish);
				Alloc::deallocate(CJH::addressof(*start), sizeof(_Ty)*n);
				throw;
			}
			finish = start + n;
			end_of_storage = start + n;
		}

		

		/*template<class Iter,
			typename value = iterator_traits<Iter>::value_type>
		vector(Iter first, Iter last){

		}*/

		vector(const self& v){
			pointer_type ptr;
			size_type newsize(CJH::addressof(*(v.end_of_storage)) - CJH::addressof(*(v.start)));
			size_t nbyte = newsize * sizeof(_Ty);
			try{
				ptr = Alloc::allocate(nbyte);
			}
			catch (...){
				Alloc::deallocate(ptr, nbyte);
				throw;
			}
			
			iterator newstart(ptr);
			iterator newfinish;
			try{
				newfinish = CJH::uninitialized_copy(v.start, v.finish, newstart);
			}
			catch (...){
				CJH::destroy(newstart, newfinish);
				Alloc::deallocate(ptr, nbyte);
				throw;
			}
			start = newstart;
			finish = newfinish;
			end_of_storage = start + newsize;
		}

		self& operator=(const self& v){
			if (this == &v) return *this;


			pointer_type ptr;
			size_type newsize(CJH::addressof(*(v.end_of_storage)) - CJH::addressof(*(v.start)));
			size_t nbyte = newsize * sizeof(_Ty);
			try{
				ptr = Alloc::allocate(nbyte);
			}
			catch (...){
				Alloc::deallocate(ptr, nbyte);
				throw;
			}

			iterator newstart(ptr);
			iterator newfinish;
			try{
				newfinish = CJH::uninitialized_copy(v.start, v.finish, newstart);
			}
			catch (...){
				CJH::destroy(newstart, newfinish);
				Alloc::deallocate(ptr, nbyte);
				throw;
			}
			clear();
			deallocate();
			start = newstart;
			finish = newfinish;
			end_of_storage = start + newsize;
			return *this;
		}

		~vector(){
			clear();
			deallocate();
		}


		iterator begin() {   // ���
			return start;
		}

		iterator end(){  // ���
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

		
		reference_type operator[](int n){ // ���
			return *(begin() + n);
		}
		reference_type front(){ //���
			return *start;
		}
		reference_type back(){  //���
			return *(end() - 1);
		}

		void push_back(const _Ty& x){
			if (finish != end_of_storage){
				construct(finish, x);
				++finish;
			}
			else{
				insert_aux(end(), x);
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
					pointer_type newptr = Alloc::allocate(nbyte);
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
				pointer_type newptr = Alloc::allocate(nbyte);
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
			iterator tmpstart = v.start;
			iterator tmpfinish = v.finish;
			iterator tmpend_of_storage = v.end_of_storage;
			v.start = start;
			v.finish = finish;
			v.end_of_storage = end_of_storage;
			start = tmpstart;
			finish = tmpfinish;
			end_of_storage = tmpend_of_storage;
		}
		//vector() : start(0), finish(0), end_of_storage(0){}
	
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