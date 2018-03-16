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

//
//
////针对set的测试
//class A{
//public:
//
//	static int i;
//	A(){
//		++i;
//		cout << "默认构造函数  " << i << "\n";
//	}
//	A& operator=(const A& a){
//		if (this != &a){
//			this->~A();
//			++i;
//			//	cout << "Copy Assignment函数  " << i << "\n";
//		}
//		return *this;
//	}
//
//	bool operator==(const A& a) const{
//		return this == &a;
//	}
//
//	bool operator !=(const A& a) const{
//		return !(*this == a);
//	}
//	A(const A& a){
//		++i;
//		//cout << "复制构造函数  " << i << "\n";
//	}
//	bool operator <(const A& a){
//		return true;
//	}
//
//	bool operator <(const A& a) const{
//		return true;
//	}
//	bool operator >(const A& a){
//		return true;
//	}
//	~A(){
//		--i;
//		//	cout << "析构函数" << i << "\n";
//	}
//};
//
//int A::i = 0;
//#define NUM 10000000
//int main(){
//
//	clock_t start, end;
//	CJH::multiset<A> mp;
//	A a;
//	srand((unsigned int)time(0));
//	CJH::vector<A> v;
//	for (int i = 0; i < NUM; ++i){
//		//pair.second = a;
//		v.push_back(a);
//	}
//	start = clock();
//	mp.insert(v.begin(), v.end());
//	//	CJH::copy(mp.begin(), mp.end(), ostream_iterator<int>(cout, " \n"));
//	cout << endl;
//	end = clock();
//	cout << mp.size() << endl;
//	//	mp.t.travelor();
//	//int n = mp.size();
//	//for (int i = 0; i < n; ++i){
//	//	mp.erase(mp.begin());
//	//}
//	mp.clear();
//	v.clear();
//	cout << "time = " << end - start << endl;
//	cout << a.i << endl;
//
//
//
//	return 0;
//}




////针对set multiset  hash_set hash_multiset的压力测试测试
//class A{
//public:
//
//	static int i;
//	A(){
//		++i;
//		cout << "默认构造函数  " << i << "\n";
//	}
//	A& operator=(const A& a){
//		if (this != &a){
//			this->~A();
//			++i;
//			//	cout << "Copy Assignment函数  " << i << "\n";
//		}
//		return *this;
//	}
//
//	bool operator==(const A& a) const{
//		return this == &a;
//	}
//
//	bool operator !=(const A& a) const{
//		return !(*this == a);
//	}
//	A(const A& a){
//		++i;
//		//cout << "复制构造函数  " << i << "\n";
//	}
//	bool operator <(const A& a){
//		return true;
//	}
//
//	bool operator <(const A& a) const{
//		return true;
//	}
//	bool operator >(const A& a){
//		return true;
//	}
//	~A(){
//		--i;
//		//	cout << "析构函数" << i << "\n";
//	}
//};
//
//int A::i = 0;
//#define NUM 500000
//int main(){
//	srand(time(0));
//	clock_t start, end;
//	CJH::multiset<int> mp;
//	A a;
//	srand((unsigned int)time(0));
//	CJH::vector<int> v;
//	for (int i = 0; i < NUM; ++i){
//		v.push_back(rand() % NUM);
//	}
//	start = clock();
//	mp.insert(v.begin(), v.end());
//	//	CJH::copy(mp.begin(), mp.end(), ostream_iterator<int>(cout, " \n"));
//	cout << endl;
//	end = clock();
//	cout << mp.size() << endl;
//	cout << "time = " << end - start << endl;
//	cout << a.i << endl;
//	//	mp.t.travelor();
//	//	mp.t.inOrder();
//	//int n = mp.size();
//	//for (int i = 0; i < n; ++i){
//	//	mp.erase(mp.begin());
//	//}
//	mp.clear();
//	v.clear();
//	return 0;
//}
