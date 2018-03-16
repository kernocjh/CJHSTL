#ifndef __HASHTABLE_H__
#define __HASHTABLE_H__


#include "config.h"
#include "allocator.h"
#include "iterator.h"
#include "operator.h"
#include "xmemory.h"
#include "vector.h"
__CJH_BEGIN
	
	template<class _Ty>
	struct __hashtable_node{
		typedef _Ty value_type;
		__hashtable_node* next;
		value_type val;
	};

	template<class _Ty, class Alloc = CJH::alloc_malloc>
	class _node_allocator{
	public:
		typedef _Ty value_type;
		typedef _Ty* pointer_type;
		typedef _Ty& reference_type;
		typedef size_t size_type;
		typedef int different_type;
		static pointer_type allocate(){
			size_type nbyte = sizeof(_Ty);
			return (pointer_type)Alloc::allocate(nbyte);
		}

		static pointer_type allocate(const size_type n){
			size_type nbyte = n*sizeof(_Ty);
			return Alloc::allocate(nbyte);
		}

		static void deallocate(pointer_type ptr){
			size_type nbyte = sizeof(_Ty);
			return Alloc::deallocate(ptr, nbyte);
		}

		static void deallocate(pointer_type ptr, const size_type n){
			size_type nbyte = n*sizeof(_Ty);
			return Alloc::deallocate(ptr, nbyte);
		}
	};

	template<class Key, class _Ty, class HashFun, class Equalkey,	class Alloc>
	class hashtable;
	template<class Key, class _Ty, class HashFun, class Equalkey, class Alloc>
	class hash_iterator :public CJH::iterator_base<CJH::forward_iterator_tag, _Ty>{
	public:
		typedef hashtable<Key, _Ty, HashFun, Equalkey, Alloc> hash_table;
		typedef hash_iterator<Key, _Ty, HashFun, Equalkey, Alloc> self;

		typedef __hashtable_node<_Ty> node;
		typedef iterator_base<forward_iterator_tag, _Ty> _MyBase;
		typedef typename _MyBase::value_type value_type;
		typedef typename _MyBase::pointer_type pointer_type;
		typedef typename _MyBase::reference_type reference_type;
		typedef typename _MyBase::difference_type difference_type;
		typedef typename _MyBase::iterator_category iterator_category;
		typedef size_t size_type;
	public:
		hash_iterator(node* ptr , hash_table *htptr ) :cur(ptr), ht(htptr){

		}

		hash_iterator() :cur((node*)0), ht((hash_table*)0){

		}

		hash_iterator(const self& x) :cur(x.cur), ht(x.ht){

		}

		self& operator=(const self& x){
			if (this != &x){
				self tmp(x);
				swap(tmp);
			}
			return *this;
		}

		reference_type operator*() const{
			return cur->val;
		}


		pointer_type operator->() const{
			return &(operator*());
		}

		bool operator == (const self& x){
			return cur == x.cur;
		}

		bool operator != (const self& x){
			return !(*this == x);
		}

		self& operator++(){
			const node* old = cur;
			cur = cur->next;
			if (!cur){
				size_type index = ht->bkt_num(old->val);
				while (cur == NULL &&++index < ht->get_bucket().size())
					cur = ht->get_bucket()[index];
			}
			return *this;
		}

		self operator++(int){
			self tmp = *this;
			++*this;
			return tmp;
		}

		void swap(self& x){
			if (this != &x){
				CJH::swap(cur, x.cur);
				CJH::swap(ht, x.ht);
			}
		}
	private:
		node* cur;
		hash_table* ht;
	};


	template<class Key, class _Ty, class HashFun = CJH::hash<Key>, 
			 class Equalkey = CJH::equal<Key>, 
			class Alloc = CJH::alloc_malloc >
	class hashtable{
	public:
		typedef HashFun hasher;
		typedef Equalkey key_equal;
		typedef Key key_type;
		typedef _Ty value_type;
		typedef value_type* pointer_type;
		typedef value_type& reference_type;
		typedef int difference_type;
		typedef size_t size_type;
		typedef hashtable<Key, _Ty, HashFun, Equalkey, Alloc> self;
		typedef hash_iterator<Key, _Ty, HashFun, Equalkey, Alloc> iterator;

	private:
		typedef struct __hashtable_node<_Ty> node;
		typedef _node_allocator<node, Alloc> node_allocator;
		typedef CJH::vector<node*> vector;
		size_type element_num;
		vector buckets;


		hasher hash;		//hash function
		key_equal equals;
	//	Extractkey get_key;
	public:
		hashtable() :hash(), equals(), element_num(0){
			initialize_buckets(0);
		}
		hashtable(size_type n, const HashFun& hf, const Equalkey& eql) :
			hash(hf), equals(eql), element_num(0){
			initialize_buckets(n);
		}
		
		hashtable(const self& x) :element_num(x.element_num), buckets(x.bucket_count(), (node*)0),
			hash(x.hash), equals(x.equals) {
			copy_from(x);
		}

		self& operator=(const self& x){
			if (this != &x){
				self tmp(x);
				swap(tmp);
			}
			
			return *this;
		}
		CJH::pair<iterator, bool> insert_unique(const value_type& value){
			resize(element_num + 1);
			return insert_unique_noresize(value);
		}
		template<class InputIter>
		void insert_unique(InputIter first, InputIter last){
			while (first != last){
				insert_unique(*first);
				++first;
			}
		}

		iterator insert_unique(iterator position, const value_type& value){
			return insert_unique(value).first;
		}
		iterator insert_equal(const value_type& value){
			resize(element_num + 1);
			return insert_equal_noresize(value);
		}

		template<class InputIter>
		void insert_equal(InputIter first, InputIter last){
			while (first != last){
				insert_equal(*first);
				++first;
			}
		}

		iterator insert_equal(iterator position, const value_type& value){
			return insert_equal(value);
		}


		iterator erase(iterator position){
			size_type index = bkt_num(*position);
			node* cur = buckets[index];
			node* prev = (node*)0;
			iterator tmp(position);
			++tmp;
			while (cur != NULL){
				if (equals(get_key(cur->val), get_key(*position))){
					if (prev == (node*)0){
						buckets[index] = cur->next;
						destory_node(cur);
					}else{
						prev->next = cur->next;
						destory_node(cur);
					}
					--element_num;
					return tmp;
				}
				
				prev = cur;
				cur = cur->next;
			}
			return end();
		}

		size_type erase(const key_type& key){
			size_type count = 0;
			size_type index = bkt_num_key(key);
			node* cur = buckets[index];
			node* prev = (node*)0;
			iterator tmp(cur, this);
			++tmp;
			while (cur != NULL){
				if (equals(get_key(cur->val), key)){
					node*next = cur->next;
					if (prev == (node*)0){
						buckets[index] = next;
						destory_node(cur);
					}
					else{
						prev->next = next;
						destory_node(cur);
					}
					count++;
					cur = next;
					continue;
				}

				prev = cur;
				cur = cur->next;
			}
			element_num -= count;
			return count;
		}

		iterator erase(iterator first, iterator last){
			while (first != last){
				static int i= 0;
			//	cout <<++i<< "\n";
				erase(first++);
			}
			return last;
		}
		void resize(size_type newsize) {
			const size_type oldsize = buckets.size();
			if (newsize > oldsize){
				const size_type n = next_size(newsize);// find next prime
				if (n > oldsize){
					vector tmp(n, (node*)0);
					try{
						for (size_type bucket = 0; bucket < oldsize; ++bucket){
							node* first = buckets[bucket];  //取旧桶节点中的第一个元素
							while (first != NULL){
								//定位first指向的元素在新桶中的位置
								size_type new_bucket = bkt_num(first->val, n);

								//以下四步操作巧妙的冲旧桶向新桶转移
								//first已经取出了旧桶节点的元素,更新旧桶节点
								buckets[bucket] = buckets[bucket]->next;
								//将first加入新桶开链中
								first->next = tmp[new_bucket];
								//更新新桶节点
								tmp[new_bucket] = first;
								//重新去得旧桶节点元素
								first = buckets[bucket];
							}

						}
						buckets.swap(tmp);
					}
					catch (...){
						throw;
					}

				}
			}
			
		}

		void clear(){
			for (size_type index = 0; index < buckets.size(); ++index){
				node* cur = buckets[index];
				while (cur != NULL){
					node* tmp = cur->next;
					destory_node(cur);
					cur = tmp;
				}
				buckets[index] = (node*)0;
			}
			element_num = 0;
			//bucket任然保持原空间大小
		}
		
		void copy_from(const hashtable& ht){
			buckets.clear();
			buckets.reserve(ht.buckets.size());
			buckets.insert(buckets.begin(), ht.buckets.size(), (node*)0);
			try{
				for (size_type index = 0; index < ht.buckets.size(); ++index){
					if (const node* cur = ht.buckets[index]){
						node* ptr = create_node(cur->val);
						buckets[index] = ptr;
						cur = cur->next;
						while (cur != NULL){
							ptr->next = create_node(cur->val);
							cur = cur->next;
							ptr = ptr->next;
						}
					}
				}
				element_num = ht.element_num;
			}
			catch (...){
				clear();
				throw;
			}

		}

		size_type next_size(size_type n){
			return __stl_next_prime(n);
		}
		bool empty() const{
			return element_num == 0;
		}
		size_type size() const{
			return element_num;
		}

		size_type bucket_count() const{
			return buckets.size();
		}

		size_type max_bucket_count() const{
			return __stl_prime_list[__stl_nun_primes - 1];
		}

		size_type elems_in_bucket(const size_type n) const{
			size_type count = 0;
			if (n < buckets.size()){
				node* cur = buckets[n];
				while (cur){
					++count;
					cur = cur->next;
				}
			}

			return count;
		}
		iterator find(const key_type& key){
			size_type index = bkt_num_key(key);  //通过key定位桶节点位置
			node *first;
			for (first = buckets[index]; first && !equals(key, get_key(first->val));
				first = first->next)
				;
			return iterator(first, this);
		}
		size_type count(const key_type& key){
			const size_type index = bkt_num_key(key);
			size_type count = 0;
			for (node* cur = buckets[index]; cur != NULL; cur = cur->next){
				if (equals(key, get_key(key))){
					count++;
				}
			}
			return count;
		}
		iterator begin(){
			node* cur;
			for (size_type index = 0; index < buckets.size(); ++index){
				cur = buckets[index];
				if (cur){
					return iterator(cur, this);
				}
			}
			return end();
		}
		iterator end(){

			return iterator((node*)0, this);
		}

		vector&  get_bucket(){
			return buckets;
		}
		void swap(self& x){
			if (this != &x){
				CJH::swap(element_num, x.element_num);
				CJH::swap(hash, x.hash);
				CJH::swap(equals, x.equals);
				buckets.swap(x.buckets);
			}
		}
		~hashtable(){
			clear();
		}
	protected:
		CJH::pair<iterator, bool> insert_unique_noresize(const value_type& value){
			const size_type index = bkt_num(value);  //判断value应位于bucket中的index位置
			node* first = buckets[index];
			for (node* cur = first; cur != NULL; cur = cur->next){ //循环遍历链表看是否key值是否相等
				if (equals(get_key(value), get_key(cur->val)))
					return CJH::pair<iterator, bool>(iterator(cur, this), false);
			}
			//添加进入
			node* tmp = create_node(value);
			tmp->next = first;
			buckets[index] = tmp;
			++element_num;
			return CJH::pair<iterator, bool>(iterator(tmp, this), true);
		}

		iterator insert_equal_noresize(const value_type& value){
			const size_type index = bkt_num(value);  //判断value应位于bucket中的index位置
			node* first = buckets[index];
			for (node* cur = first; cur != NULL; cur = cur->next){ //循环遍历链表看是否key值是否相等
				if (equals(get_key(value), get_key(cur->val))){
					node* tmp = create_node(value);
					tmp->next = cur->next;
					cur->next = tmp;
					++element_num;
					return iterator(cur, this);
				}
			}
			node* tmp = create_node(value);
			tmp->next = first;
			buckets[index] = tmp;
			++element_num;
			return iterator(tmp, this);
		}
		void initialize_buckets(size_type n){
			const size_type n_buckets = next_size(n);
			buckets.reserve(n_buckets);
			buckets.insert(buckets.end(), n_buckets, (node*)0);
			element_num = 0;
		}
		node* create_node(const value_type& value){
			node* ptr= node_allocator::allocate();
			try{
				CJH::construct(CJH::addressof(ptr->val), value);
			}
			catch (...){
				node_allocator::deallocate(ptr);
				throw;
			}
			ptr->next = NULL;
			return ptr;
		}

		void destory_node(node* ptr){
			CJH::destroy(CJH::addressof(ptr->val));
			node_allocator::deallocate(ptr);
		}




	public:

		//获取元素value在大小为n的桶中的位置
		size_type bkt_num(const value_type&value ,size_type n) const{
			return bkt_num_key(get_key(value), n);
		}


		//获取元素value在本桶中的位置
		size_type bkt_num(const value_type&value) const{
			return bkt_num_key(get_key(value));
		}

		size_type bkt_num_key(const key_type& key, size_type n) const{
			return hash(key) % n;
		}

		size_type bkt_num_key(const key_type& key) const{
			return bkt_num_key(key, buckets.size());
		}

		const key_type& get_key(const CJH::pair<key_type,value_type>& value) const{
			return value.first;
		}

		const key_type& get_key(const value_type& value) const{
			value_type *p = const_cast<value_type*>(&value);
			return (key_type&)*((key_type*)p);
			
		}


		//到28个质数中，找到最接近大于n的那个质数
		inline unsigned long __stl_next_prime(unsigned long n){
			const unsigned long* first = __stl_prime_list;
			const unsigned long* last = __stl_prime_list + __stl_nun_primes;
			while (first != last){
				if (*first > n) return *first;
				++first;
			}
			return *(last - 1);
		}

		inline size_t max_bucket_count() {
			return __stl_prime_list[__stl_nun_primes - 1];
		}
	};
__CJH_END
#endif	//__HASHTABLE_H__