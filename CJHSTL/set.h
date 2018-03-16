#ifndef __SET_H__
#define __SET_H__

#include "allocator.h"
#include "config.h"
#include "iterator.h"
#include "xmemory.h"
#include "rbtree.h"
#include "operator.h"

__CJH_BEGIN


	
	template<class _Ty, class Compare=CJH::less<_Ty> >
	class set{
	public:
		typedef _Ty key_type;
		typedef _Ty value_type;
		typedef Compare key_compare;
		typedef Compare value_compare;
		typedef CJH::rbtree_base<key_type, value_type, key_compare> rep_type;
		typedef typename rep_type::reference_type reference_type;
		typedef typename rep_type::size_type size_type;
		typedef typename rep_type::pointer_type pointer_type;
		typedef typename rep_type::different_type different_type;
		typedef typename rep_type::iterator iterator;
		typedef typename rep_type::reverse_iterator reverse_iterator;
		typedef set<_Ty, Compare> self;
		
	public:
		set():t(){

		}

		template<class Iter>
		set(Iter first, Iter last, const Compare& cmp):t(cmp){
			t.insert_unique(first, last);
			
		}

		template<class Iter>
		set(Iter first, Iter last):t(){
			t.insert_unique(first, last);
		}

		set(const self& x):t(x.t){

		}

		self& operator=(const self&x){
			if (this != &x){
				self tmp(x);
				swap(tmp);
			}
			return *this;
		}

		key_compare key_comp(){
			return t.key_comp();
		}

		iterator begin(){
			return  t.begin();
		}

		iterator end(){
			return t.end();
		}

		reverse_iterator rbegin(){
			return t.rbegin();
		}

		reverse_iterator rend(){
			return t.rend();
		}
		bool empty(){
			return t.empty();
		}

		size_type size(){
			return t.size();
		}
		
		size_type max_size(){
			return t.max_size();
		}

		void insert(const value_type& value){
			t.insert_unique(CJH::pair<key_type, value_type>(value, value));
		}

		template<class Iter>
		void insert(Iter first, Iter last){
			t.insert_unique(first, last);
		}


		void erase(const value_type& value){
			t.erase(value);
		}

		void erase(iterator first, iterator last){
			t.erase(first, last);
		}

		iterator erase(iterator position){
			t.erase(position);
		}

		void clear(){
			erase(begin(), end());
		}

		iterator find(const key_type& key){
			return t.find(key);
		}

		size_type count(const key_type& key){
			return find(key) != end() ? 1 : 0;
		}

		iterator lower_bound(const key_type& key){
			return t.lower_bound(key);
		}

		iterator upper_bound(const key_type& key){
			return t.upper_bound(key);
		}
		void swap(self& x){
			if (this != &x){
				t.swap(x.t);
			}
		}

		~set(){

		}
	private:
		rep_type t;
	};


	template<class _Ty, class Compare = CJH::less<_Ty> >
	class multiset{
	public:
		typedef _Ty key_type;
		typedef _Ty value_type;
		typedef Compare key_compare;
		typedef Compare value_compare;
		typedef CJH::rbtree_base<key_type, value_type, key_compare> rep_type;
		typedef typename rep_type::reference_type reference_type;
		typedef typename rep_type::size_type size_type;
		typedef typename rep_type::pointer_type pointer_type;
		typedef typename rep_type::different_type different_type;
		typedef typename rep_type::iterator iterator;
		typedef typename rep_type::reverse_iterator reverse_iterator;
		typedef multiset<_Ty, Compare> self;

	public:
		multiset() :t(){

		}

		template<class Iter>
		multiset(Iter first, Iter last, const Compare& cmp) : t(cmp){
			t.insert_equal(first, last);

		}

		template<class Iter>
		multiset(Iter first, Iter last) : t(){
			t.insert_equal(first, last);
		}

		multiset(const self& x) :t(x.t){

		}

		self& operator=(const self&x){
			if (this != &x){
				self tmp(x);
				swap(tmp);
			}
			return *this;
		}

		key_compare key_comp(){
			return t.key_comp();
		}

		iterator begin(){
			return  t.begin();
		}

		iterator end(){
			return t.end();
		}

		reverse_iterator rbegin(){
			return t.rbegin();
		}

		reverse_iterator rend(){
			return t.rend();
		}
		bool empty(){
			return t.empty();
		}

		size_type size(){
			return t.size();
		}

		size_type max_size(){
			return t.max_size();
		}

		void insert(const value_type& value){
			t.insert_equal(CJH::pair<key_type, value_type>(value, value));
		}

		template<class Iter>
		void insert(Iter first, Iter last){
			t.insert_equal(first, last);
		}


		void erase(const value_type& value){
			t.erase(value);
		}

		void erase(iterator first, iterator last){
			t.erase(first, last);
		}

		iterator erase(iterator position){
			t.erase(position);
		}

		void clear(){
			erase(begin(), end());
		}

		iterator find(const key_type& key){
			return t.find(key);
		}

		size_type count(const key_type& key){
			return find(key) != end() ? 1 : 0;
		}

		iterator lower_bound(const key_type& key){
			return t.lower_bound(key);
		}

		iterator upper_bound(const key_type& key){
			return t.upper_bound(key);
		}
		void swap(self& x){
			if (this != &x){
				t.swap(x.t);
			}
		}

		~multiset(){

		}
	private:
		rep_type t;
	};

__CJH_END
#endif //__SET_H__