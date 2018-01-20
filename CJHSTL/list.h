#ifndef __LIST_H__
#define __LIST_H__


#include "iterator.h"
#include "config.h"
#include "allocator.h"
#include "operator.h"
#include <assert.h>

// 定义节点的类型

__CJH_BEGIN
	template<class _Ty>
	struct __list_node{
		typedef struct __list_node<_Ty>* list_pointer;
		list_pointer prev;
		list_pointer next;
		_Ty data;
		__list_node(const _Ty& x) :data(x), prev(NULL), next(NULL){

		}
		__list_node() :prev(NULL), next(NULL){

		}

	};


	/**
	若node为NULL
	关闭其迭代器的检验操作
	这是一个很重要的知识点
	*/
	template<class _Ty, class Alloc = CJH::allocator<_Ty>, 
		class Category = bidirectinal_iterator_tag >
	class _list_iterator :public iterator_base<bidirectinal_iterator_tag, _Ty>{
	public:
		typedef iterator_base<bidirectinal_iterator_tag, _Ty> _Mybase;
		typedef typename _Mybase::pointer_type pointer_type;
		typedef typename _Mybase::value_type value_type;
		typedef typename _Mybase::difference_type difference_type;
		typedef typename _Mybase::reference_type reference_type;
		typedef typename _Mybase::iterator_category iterator_category;


		//typedef _list_iterator<_Ty> iterator;  探究一下为什么这样声明会发生错误
		typedef _list_iterator<_Ty, Alloc, Category> iterator;

		typedef __list_node<_Ty>* list_type;

	private:
		list_type node;  //这是一个指针

	public:
		_list_iterator(list_type t = 0) :iterator_base<Category, _Ty>(), node(t){
		}

		_list_iterator(const iterator& x) :node(x.node){

		}

		~_list_iterator(){
			//Alloc::deallocate(sizeof(__list_node<_Ty>));
		}

		bool operator ==(const iterator& x){
			return this->node == x.node;
		}

		bool operator != (const iterator &x){
			return this->node != x.node;
		}

		reference_type operator *(){ // 若node为NULL
			return (this->node->data);
		}

		pointer_type operator ->(){
			return (&(operator*()));
		}

		iterator& operator++(){
			node = node->next;
			return *this;
		}

		iterator operator++(int){
			iterator tmp;
			tmp = *this;
			++(*this);
			return tmp;
		}

		iterator& operator--(){
			node = node->prev;
			return *this;
		}

		iterator operator--(int){
			iterator tmp;
			tmp = *this;
			--(*this);
			return tmp;
		}


		list_type _MyNode(){
			return node;
		}
	};



	template<class _Ty, class list_node = struct __list_node<_Ty>, 
		class Alloc = CJH::allocator<list_node> >
	class listnode_allocator{
	public:
		typedef typename Alloc::value_type value_type;
		typedef typename Alloc::pointer pointer_type;
		static pointer_type allocate(){
			return Alloc::allocate(sizeof(value_type));
		}
		
		static void deallocate(pointer_type ptr){
			Alloc::deallocate(ptr, sizeof(value_type));
		}
	};


	template<class _Ty, class Alloc = listnode_allocator<_Ty> >
	class list{
	public:
		typedef _Ty value_type;
		typedef value_type* pointer_type;
		typedef value_type& reference_type;
		typedef size_t size_type;
		typedef _list_iterator<_Ty, Alloc, bidirectinal_iterator_tag> iterator;
		typedef typename iterator::difference_type difference_type;
		typedef struct __list_node<_Ty> node_type;
		typedef node_type* list_type;
		typedef list<_Ty, Alloc> self;
	private:
	protected:
		list_type node;
		size_type _Mycount;
		list_type alloc_node(){
			// allocate the space
			return Alloc::allocate();
		}
		
		list_type create_node(const _Ty& value){
			// construct a node
			++_Mycount;
			list_type ptr = alloc_node();
			CJH::construct(CJH::addressof((ptr->data)), value);
			return ptr;
		}

		void free_node(list_type ptr){
			--_Mycount;
			//deallocate the space of nodes
			CJH::destroy(CJH::addressof(ptr->data));
			Alloc::deallocate(ptr);
		}


	protected:
		void init_list(){
			node = alloc_node();
			node->prev = node;
			node->next = node;
			_Mycount = 0;
		}

		void transfer(iterator position, iterator first, iterator last){
			// 迁移操作  仅供内部操作  把[first, last)的元素迁移到特定元素之前
			if (first != last){
				list_type prev_position = (list_type)position._MyNode()->prev;
				list_type prev_first = (list_type)first._MyNode()->prev;
				list_type prev_last = (list_type)last._MyNode()->prev;

				prev_position->next = (list_type)first._MyNode();
				prev_position->next->prev = prev_position;

				prev_last->next = (list_type)position._MyNode();
				prev_last->next->prev = prev_last;

				prev_first->next = (list_type)last._MyNode();
				prev_first->next->prev = prev_first;
			}
		}

		void _Mergesort(iterator first, iterator last){
			if (first == last || first._MyNode()->next == last._MyNode()) return;
			iterator fast, slow;
			fast = first;
			slow = first;
			while (fast != last){
				++slow;
				++fast;
				if (fast == last) break;
				++fast;
			}

			self temp;

			if (last != slow)transfer(temp.begin(), slow, last);

			_Mergesort(first, last);
			_Mergesort(temp.begin(), temp.end());
/*CJH::copy(begin(), end(), ostream_iterator<int>(cout, " "));
std::cout << std::endl;
CJH::copy(temp.begin(), temp.end(), ostream_iterator<int>(cout, " "));
std::cout << std::endl;*/
			merge(temp);
/*CJH::copy(begin(), end(), ostream_iterator<int>(cout, " "));
cout << endl<< std::endl;*/
		}
	public:
		list() :_Mycount(0){
			init_list();
		}

		list(size_type n, const _Ty& value) :list(){
			try{
				for (size_type i = 0; i < n;++i)
					insert(end(), value);
			}
			catch (...){
				clear();
				throw;
			}
		}
		template<class Iter, typename value = CJH::iterator_traits<Iter>::value_type>
		list(Iter first, Iter last):list(){
			try{
				while (first != last){
					insert(end(), *first);
					++first;
				}
			}
			catch (...){
				clear();
				throw;
			}
		}

		list(const self& list){
			try{

				init_list();
				self::iterator first = list.begin();
				self::iterator last = list.end();
				while (first != last){
					insert(end(), *first);
					++first;
				}
				this->_Mycount = list._Mycount;
			}
			catch (...){
				clear();
				throw;
			}
		}
		self& operator=(const self& list){
			if (this == &list) return this;
			try{
				self::iterator first = list.begin();
				self::iterator last = list.end();
				while (first != last){
					insert(end(), *first);
					++first;
				}
				this->_Mycount = list._Mycount;
				
			}
			catch (...){
				clear();
				throw;
			}
		}

		iterator begin(){
			return iterator(node->next);
		}
		iterator end(){
			return iterator(node);
		}
		//iterator rbegin();
		//iterator rend();
		size_type size(){
			return size_type(_Mycount);
		}
		bool empty(){
			return node->next == node;
		}
		iterator insert(iterator position, const _Ty &value){
			list_type ptr = create_node(value);
			list_type cur = (list_type)position._MyNode();
			list_type pre = (list_type)cur->prev;
			ptr->next = cur;
			cur->prev = ptr;
			ptr->prev = pre;
			pre->next = ptr;
			return iterator(ptr);
		}
		void push_back(const _Ty& value){
			insert(this->end(), value);
		}
		void push_front(const _Ty& value){
			insert(this->begin(), value);
		}
		void pop_back(){
			erase(--(this->end()));
		}
		void pop_front(){
			erase(this->begin());
		}

		iterator erase(iterator position){
			list_type ptr = (list_type)position._MyNode();
			list_type prev_node = (list_type)ptr->prev;
			list_type next_node = (list_type)ptr->next;
			prev_node->next = next_node;
			next_node->prev = prev_node;
			free_node(ptr);
			return next_node;
		}
		iterator erase(iterator first, iterator last){
			list_type cur = (list_type)first._MyNode();
			list_type prev_node = cur->prev;
			while (first != last){
				cur = (list_type)first._MyNode();
				++first;
				free_node(cur);
			}
			cur = (list_type)first._MyNode();
			cur->prev = prev_node;
			prev_node->next = cur;
			return iterator(cur);
		}

		void clear(){
			erase(this->begin(), this->end());
		}

		void remove(const _Ty& value){
			iterator first = this->begin();
			iterator last = this->end();
			while (first != last){
				if (*first == value){
					first = erase(first);
					continue;
				}
				++first;
			}
		}



		void unique(){
			iterator first = this->begin();
			iterator last = this->end();
			iterator next = first;
			while (++next != last){
				if (*first == *next){
					first = erase(first);
				}
				else{
					first = next;
				}
				next = first;
			}

//以下编程方式具有大bug  因为++next导致  next指向end指针和first做比较
/*while (first != last){
	iterator next = first;
	if (*first == *(++next)){
		first = erase(first);
	}
	++first;
}*/
		}

		void resize(const size_type n){

		}
		void reverse(){
			if (node->next == node || node->next->next == node) return;
			iterator position = begin();
			iterator first = position;
			++first;
			while (first != this->end()){
				iterator old = first;
				++first;
				transfer(position, old, first);
				position = old;
			}

		}
		void splice(iterator position, self& list){
			if (list.empty() || this != &list) return;
			size_type tmpcount = list._Mycount;
			transfer(position, list.begin(), list.end());
			this->_Mycount += tmpcount;
			list._Mycount = 0;
		}
		/**
		一个BUG检验  若it等于了list的end()  应该报错  但是在本题中任然可以	
		另一个bug就是it迭代器和list不相容
		*/
		void splice(iterator position, self& list, iterator it){
			if (list.empty()) return;
			iterator last = it;
			++last;
			if (position == it || position == last)return;
			transfer(position, it, last);
			if (this != &list){
				++(this->_Mycount);
				--(list._Mycount);
			}
		}

		void swap(self& list){
			if (this == &list) return;
			list_type tmpnode = list.node;
			size_type tmpcount = list._Mycount;
			list.node = this->node;
			list._Mycount = this->_Mycount;
			this->node = tmpnode;
			this->_Mycount = tmpcount;
		}
		

		template<class Compare>
		void sort(Compare& pred){
			if (1 < this->_Mycount){
				const size_t _MAXBIN = 64;
				self templist;
				self binlist[_MAXBIN];
				size_t maxbin = 0;
				while (!empty()){
					templist.splice(templist.begin(), *this, this->begin());
					size_t bin = 0;
					for (bin = 0; bin < maxbin && !binlist[bin].empty(); ++bin){
						binlist[bin].merge(templist, pred);
						templist.swap(binlist[bin]);
					}

					templist.swap(binlist[bin]);
					if (bin == maxbin)++maxbin;
				}
				for (size_t bin = 1; bin < maxbin; ++bin){
					binlist[bin].merge(binlist[bin - 1], pred);
				}
				this->swap(binlist[maxbin - 1]);
			}
		}
		void sort(){

			sort(std::less<_Ty>());
		}
		template<class Compare>
		void merge(self& list, Compare compare){
			if (this == &list) return;
			size_type tmpcount = list.size();
			iterator first1 = this->begin();
			iterator first2 = list.begin();
			iterator last1 = this->end();
			iterator last2 = list.end();
			while (first1 != last1 && first2 != last2){
				if (compare(*first1, *first2)){
					++first1;
				}
				else{
					iterator old = first2;
					transfer(first1, old, ++first2);
				}
			}
			if (first2 != last2){
				transfer(first1, first2, last2);
			}

			this->_Mycount += tmpcount;
			list._Mycount = 0;
		}
		void merge(self& list){
			merge(list, CJH::less<_Ty>());
		}

		iterator find(iterator first, iterator last, const _Ty& value){
			while (first != last){
				if (*first == value) return first;
			}
			return this->end();
		}

		iterator find(const _Ty& value){
			iterator first = this->begin();
			iterator last = this->end();
			while (first != last){
				if (*first == value) return first;
			}

			return last;
		}

		~list(){
			clear();
			free_node(node);
		}
		
	};
__CJH_END		
#endif


//list综合敏捷测试代码
//std::list<int> t;
//CJH::list<int> l;
//CJH::list<int> l2;
//l.clear();
//l2.push_back(-1);
//l2.push_back(1);
//l2.push_back(250);
//CJH::list<int>::iterator it, end;
////l.push_front(0);
////l.push_front(1);
////l.pop_back();
////l.push_front(0);
////l.push_back(2);
//l.merge(l2);
////	l.splice(l.end(), l2, l.end());
////	cout << *ite;
////	l.erase(l.begin());
//CJH::copy(l.begin(), l.end(), ostream_iterator<int>(cout, " "));;
//std::cout << std::endl;
//l.unique();
//CJH::copy(l2.begin(), l2.end(), ostream_iterator<int>(cout, " "));
//std::cout << std::endl;
//
//l.reverse();
//CJH::copy(l.begin(), l.end(), ostream_iterator<int>(cout, " "));
//std::cout << std::endl;
//std::cout << sizeof(CJH::__list_node<int>);





//BUG1 
//CJH::list<int> l;
//l.pop_back();
//CJH::list<int>::iterator ite, end;
//ite != l.begin();
//l.push_back(1);
//ite = l.begin();
//l.push_back(7);
//l.push_back(250);
//l.push_front(250);
//l.push_front(250);
//
//l.push_back(250);
////	cout << *ite;
////	l.erase(l.begin());
//CJH::copy(l.begin(), l.end(), ostream_iterator<int>(cout, " "));
//std::cout << std::endl;
//std::cout << sizeof(CJH::__list_node<int>);