#ifndef __MAP_H__
#define __MAP_H__
#include "allocator.h"
#include "config.h"
#include "iterator.h"
#include "xmemory.h"
#include "rbtree.h"
#include "operator.h"


__CJH_BEGIN
	template<class Key, class _Ty, class Compare = CJH::less<int> >
	class map{
	public:
		typedef Key key_type;
		typedef _Ty value_type;
		typedef Compare key_compare;
		typedef Compare value_compare;
		typedef CJH::rbtree_base<key_type, value_type, key_compare> rep_type;
		typedef CJH::pair<Key, _Ty> pair;
		typedef typename rep_type::reference_type reference_type;
		typedef typename rep_type::size_type size_type;
		typedef typename rep_type::pointer_type pointer_type;
		typedef typename rep_type::different_type different_type;
		typedef typename rep_type::iterator iterator;
		typedef typename rep_type::reverse_iterator reverse_iterator;
		typedef map<Key, _Ty, Compare> self;

	public:
		map() :t(){
			
		}

		map(const self& x):t(x.t){

		}

		self& operator=(const self& x){
			if (this != &x){
				self tmp(x);
				swap(tmp);
			}
			return *this;
		}

		key_compare key_comp() const{
			return t.key_comp();
		}

		value_compare value_comp() const{
			return (value_compare)t.key_comp();
		}

		iterator begin(){
			return t.begin();
		}

		reverse_iterator rbegin(){
			return t.rbegin();
		}

		iterator end(){
			return t.end();
		}

		reverse_iterator rend() {
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

		value_type& operator[](const key_type& key){
			iterator it = find(key);
			if (it == end()){
				return *insert(pair(key, value_type()));
			}
			else{
				return *it;
			}
		}

		void swap(map<Key, _Ty, Compare> &x){
			t.swap(x);
		}

		iterator insert(const pair& p){ //�����Լ����iterator
			return t.insert_unique(p);
		}

		iterator insert(iterator position, const pair& p){
			return t.insert_unique(p);
		}

		//template<class Iter>
		//void insert(Iter first, Iter last){

		//}

		void erase(iterator position){
			t.erase(position);
		}

		iterator erase(const key_type& key){
			return t.erase(key);
		}

		void erase(iterator first, iterator last){
			t.erase(first, last);
		}

		void clear(){
			erase(begin(), end());
		}

		iterator find(const key_type& key){
			return t.find(key);
		}

		size_type count(const key_type& key){
			return t.find(key) != end() ? 1 : 0;
		}

		iterator lower_bound(const key_type& key){
			return t.lower_bound(key);
		}

		iterator upper_bound(const key_type& key){
			return t.upper_bound(key);
		}

		CJH::pair<iterator, iterator> equal_range(const key_type& key){
			iterator first = lower_bound(key);
			iterator last = upper_bound(key);
			if (first == end() || last == end()){
				return CJH::pair<iterator, iterator>(end(), end());
			}
			else{
				return CJH::pair<iterator, iterator>(first, ++last);
			}
		}
		rep_type t;
	private:

	};




	template<class Key, class _Ty, class Compare = CJH::less<int> >
	class multimap{
	public:
		typedef Key key_type;
		typedef _Ty value_type;
		typedef Compare key_compare;
		typedef Compare value_compare;
		typedef CJH::rbtree_base<key_type, value_type, key_compare> rep_type;
		typedef CJH::pair<key_type, value_type> pair;
		typedef typename rep_type::reference_type reference_type;
		typedef typename rep_type::size_type size_type;
		typedef typename rep_type::pointer_type pointer_type;
		typedef typename rep_type::different_type different_type;
		typedef typename rep_type::iterator iterator;
		typedef typename rep_type::reverse_iterator reverse_iterator;
		typedef multimap<Key, _Ty, Compare> self;

	public:
		multimap() :t(){

		}

		multimap(const self& x) :t(x.t){

		}

		self& operator=(const self& x){
			if (this != &x){
				self tmp(x);
				swap(tmp);
			}
			return *this;
		}

		key_compare key_comp() const{
			return t.key_comp();
		}

		value_compare value_comp() const{
			return (value_compare)t.key_comp();
		}

		iterator begin(){
			return t.begin();
		}

		reverse_iterator rbegin(){
			return t.rbegin();
		}

		iterator end(){
			return t.end();
		}

		reverse_iterator rend() {
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

		/*value_type& operator[](const key_type& key){
			iterator it = find(key);
			if (it == end()){
				return *insert(pair(key, value_type()));
			}
			else{
				return *it;
			}
		}*/

		void swap(multimap<Key, _Ty, Compare> &x){
			t.swap(x);
		}

		iterator insert(const pair& p){ //�����Լ����iterator
			return t.insert_equal(p);
		}

		iterator insert(iterator position, const pair& p){
			return t.insert_equal(p);
		}

		//template<class Iter>
		//void insert(Iter first, Iter last){

		//}

		void erase(iterator position){
			t.erase(position);
		}

		iterator erase(const key_type& key){
			return t.erase(key);
		}

		void erase(iterator first, iterator last){
			t.erase(first, last);
		}

		void clear(){
			erase(begin(), end());
		}

		iterator find(const key_type& key){
			return t.find(key);
		}

		size_type count(const key_type& key){
			return t.find(key) != end() ? 1 : 0;
		}

		iterator lower_bound(const key_type& key){
			return t.lower_bound(key);
		}

		iterator upper_bound(const key_type& key){
			return t.upper_bound(key);
		}

		CJH::pair<iterator, iterator> equal_range(const key_type& key){
			iterator first = lower_bound(key);
			iterator last = upper_bound(key);
			if (first == end() || last == end()){
				return CJH::pair<iterator, iterator>(end(), end());
			}
			else{
				return CJH::pair<iterator, iterator>(first, ++last);
			}
		}
		rep_type t;
	private:

	};

__CJH_END

#endif  //__MAP_H__
//���multimap�Ĳ���
//class A{
//public:
//
//	static int i;
//	A(){
//		++i;
//		cout << "Ĭ�Ϲ��캯��  " << i << "\n";
//	}
//	A& operator=(const A& a){
//		if (this != &a){
//			this->~A();
//			++i;
//			cout << "Copy Assignment����  " << i << "\n";
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
//		cout << "���ƹ��캯��  " << i << "\n";
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
//		cout << "��������" << i << "\n";
//	}
//};
//
//int A::i = 0;
//#define NUM 1000
//int main(){
//
//	//	CJH::multimap<int, int, CJH::greater<int>> mp;
//	//	CJH::multimap<int, int > mp;
//	//	A a;
//	//	CJH::pair<int, int> pair;
//	////	pair.second = a;
//	//	srand(time(0));
//	//	for (int i = 0; i < NUM; ++i){
//	//		pair.first = rand() % NUM;
//	//		pair.second = pair.first;
//	//		mp.insert(pair);
//	//	}
//	//	CJH::copy(mp.begin(), mp.end(), ostream_iterator<int>(cout, " \n"));
//	//	cout << endl;
//	//	cout << mp.size() << endl;
//	//	mp.t.travelor();
//	//	//int n = mp.size();
//	//	//for (int i = 0; i < n; ++i){
//	//	//	mp.erase(mp.begin());
//	//	//}
//	//	mp.t.clear();
//	//	mp.t.clear();
//	//	mp.t.clear();
//
//
//
//	CJH::multimap<int, A> mp;
//	A a;
//	CJH::pair<int, A> pair;
//	pair.second = a;
//	srand((unsigned int)time(0));
//	for (int i = 0; i < NUM; ++i){
//		pair.first = rand() % NUM;
//		//pair.second = a;
//		mp.insert(pair);
//	}
//	//	CJH::copy(mp.begin(), mp.end(), ostream_iterator<int>(cout, " \n"));
//	cout << endl;
//	cout << mp.size() << endl;
//	mp.t.travelor();
//	//int n = mp.size();
//	//for (int i = 0; i < n; ++i){
//	//	mp.erase(mp.begin());
//	//}
//	mp.t.clear();
//	mp.t.clear();
//	mp.t.clear();
//	return 0;
//}


//���map�Ĳ���
//class A{
//public:
//
//	static int i;
//	A(){
//		++i;
//		cout << "Ĭ�Ϲ��캯��  " << i << "\n";
//	}
//	A& operator=(const A& a){
//		if (this != &a){
//			this->~A();
//			++i;
//			cout << "Copy Assignment����  " << i << "\n";
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
//		cout << "���ƹ��캯��  " << i << "\n";
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
//		cout << "��������" << i << "\n";
//	}
//};
//
//int A::i = 0;
//#define NUM 1000
//int main(){
//
//	//	CJH::map<int, int> mp;
//	//	A a;
//	//	CJH::pair<int, int> pair;
//	////	pair.second = a;
//	//	srand(time(0));
//	//	for (int i = 0; i < NUM; ++i){
//	//		/*pair.first = rand() % NUM;
//	//		pair.second = pair.first;
//	//		mp.insert(pair);*/
//	//		int num = rand() % NUM;
//	//		mp[num] = num;
//	//	}
//	//	CJH::copy(mp.begin(), mp.end(), ostream_iterator<int>(cout, " \n"));
//	//	cout << endl;
//	//	cout << mp.size() << endl;
//	//	mp.t.travelor();
//	//	//int n = mp.size();
//	//	//for (int i = 0; i < n; ++i){
//	//	//	mp.erase(mp.begin());
//	//	//}
//	//	mp.t.clear();
//	//	mp.t.clear();
//	//	mp.t.clear();
//
//
//
//	CJH::map<int, A> mp;
//	A a;
//	CJH::pair<int, A> pair;
//	pair.second = a;
//	srand((unsigned int)time(0));
//	for (int i = 0; i < NUM; ++i){
//		pair.first = rand() % NUM;
//		//pair.second = a;
//		mp.insert(mp.begin(), pair);
//		mp[pair.first] = a;
//	}
//	//	CJH::copy(mp.begin(), mp.end(), ostream_iterator<int>(cout, " \n"));
//	cout << endl;
//	cout << mp.size() << endl;
//	mp.t.travelor();
//	//int n = mp.size();
//	//for (int i = 0; i < n; ++i){
//	//	mp.erase(mp.begin());
//	//}
//	mp.t.clear();
//	mp.t.clear();
//	mp.t.clear();
//	return 0;
//}