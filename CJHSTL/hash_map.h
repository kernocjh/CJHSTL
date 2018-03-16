#ifndef __HASH_MAP_H__
#define __HASH_MAP_H__
#include "config.h"
#include "operator.h"
#include "iterator.h"
#include "xmemory.h"
#include "allocator.h"
#include "hashtable.h"

__CJH_BEGIN
	template<class Key, class _Ty, 
			class HashFun = CJH::hash<Key>,
			class Equalkey = CJH::equal<Key>, 
			class Alloc = CJH::alloc_malloc>
	class hash_map{
	private:
		typedef CJH::pair<Key, _Ty> Value;
		typedef CJH::hashtable<Key, Value, HashFun, Equalkey, Alloc> rep_type;  // take hashtable as basic data structure
		rep_type t;
	public:

		typedef typename rep_type::hasher hasher;
		typedef typename rep_type::key_equal key_equal;
		typedef typename rep_type::key_type key_type;
		typedef typename rep_type::value_type value_type;
		typedef typename rep_type::size_type size_type;
		typedef typename rep_type::pointer_type pointer_type;
		typedef typename rep_type::reference_type reference_type;
		typedef typename rep_type::difference_type difference_type;
		typedef typename rep_type::iterator iterator;
		typedef hash_map<Key, _Ty, HashFun, Equalkey, Alloc> self;
	public:
		hash_map() :t(){

		}

		explicit hash_map(size_type n) :t(n){

		}

		hash_map(size_type n, const hasher& hf) :t(n, hf, key_equal()){

		}

		hash_map(size_type n, const hasher& hf, const key_equal& key_eq) :t(n, hf, key_eq){

		}

		template<class InputIter>
		hash_map(InputIter first, InputIter last){
			t.insert_unique(first, last);
		}

		template<class InputIter>
		hash_map(InputIter first, InputIter last, size_type n, const hasher& hf)
			:t(n, hf, key_equal()){
			t.insert_unique(first, last);
		}

		template<class InputIter>
		hash_map(InputIter first, InputIter last, size_type n,
			const hasher& hf, const key_equal& key_eq) : t(n, hf, key_eq){
			t.insert_unique(first, last);
		}


		hash_map(const self& x) :t(x.t){

		}

		self& operator= (const self& x){
			if (this != &x){
				self tmp(x);
				swap(tmp);
			}

			return *this;
		}

		_Ty& operator[] (const key_type& key){
			return ((*(t.insert_unique(value_type(key, _Ty())).first)).second);
		}

		iterator begin(){
			return t.begin();
		}

		iterator end(){
			return t.end();
		}
		size_type size() const{
			return t.size();
		}

		bool empty() const{
			return t.empty();
		}


		void swap(self& x){
			if (this != &x){
				t.swap(x.t);
			}
		}

		~hash_map(){

		}

	public:
		iterator insert(const value_type& value){
			return t.insert_unique(value).first;
		}

		template<class InputIter>
		void insert(InputIter first, InputIter last){
			t.insert_unique(first, last);
		}

		iterator find(const key_type& key) {
			return t.find(key);
		}

		size_type count(const key_type& key) const{
			return t.count(key);
		}

		CJH::pair<iterator, iterator> equal_range(const key_type& key){
			iterator first = find(key);
			iterator last = first;
			if (first != end())++last;
			return CJH::pair<iterator, iterator>(first, last);
		}

		size_type erase(const key_type& key){
			return t.erase(key);
		}

		iterator erase(iterator position){
			return t.erase(position);
		}

		iterator erase(iterator first, iterator last){
			return t.erase(first, last);
		}

		void clear(){
			t.clear();
		}

		void resize(size_type newsize){
			t.resize(newsize);
		}

		size_type bucket_count() const{
			return t.bucket_count();
		}

		size_type max_bucket_count() const{
			return t.max_bucket_count();
		}

		size_type elems_in_bucket(size_type n) const{
			return t.elems_in_bucket(n);
		}
	};
//hash_map测试代码
//	class A{
//	public:
//
//		static int i;
//		A(){
//			++i;
//			cout << "默认构造函数  " << i << "\n";
//		}
//		A& operator=(const A& a){
//			if (this != &a){
//				this->~A();
//				++i;
//				cout << "Copy Assignment函数  " << i << "\n";
//			}
//			return *this;
//		}
//
//		bool operator==(const A& a) const{
//			return this == &a;
//		}
//
//		bool operator !=(const A& a) const{
//			return !(*this == a);
//		}
//		A(const A& a){
//			++i;
//			cout << "复制构造函数  " << i << "\n";
//		}
//		bool operator <(const A& a){
//			return true;
//		}
//
//		bool operator <(const A& a) const{
//			return true;
//		}
//		bool operator >(const A& a){
//			return true;
//		}
//		~A(){
//			--i;
//			cout << "析构函数" << i << "\n";
//		}
//	};
//
//	int A::i = 0;
//#define NUM 100
//	int main(){
//		A a;
//		CJH::pair<int, A> p(5, a);
//		//	CJH::hash_map<int, int> a;
//		//	CJH::hash_set<int> hs;
//		//	const int b = get(CJH::pair<int, int>(5, 250));
//		CJH::hash_map<int, A> g;
//		CJH::hash_set<int> s;
//		CJH::hash_map<int, A>::iterator it, end;
//		g[5] = a;
//		g[7] = a;
//		for (int i = 0; i < NUM; ++i){
//			g[rand() % NUM] = a;
//		}
//		for (it = g.begin(); it != g.end(); ++it){
//			cout << it->first << "\t"; /*<< it->second << "\n";*/
//		}
//
//
//		cout << "size() = " << g.size() << "\t bucket_count()" << g.bucket_count() << "\n";
//		//	cout << g.max_bucket_count() << endl;
//		srand(time(0));
//		int num = 250;
//		g.insert(CJH::pair<int, A>(5, a));
//		g.insert(CJH::pair<int, A>(1, a));
//		g.insert(CJH::pair<int, A>(1, a));
//		g.insert(CJH::pair<int, A>(7, a));
//		g.insert(CJH::pair<int, A>(799, a));
//		g.insert(CJH::pair<int, A>(430, a));
//		g.clear();
//		g.insert(CJH::pair<int, A>(5, a));
//		g.insert(CJH::pair<int, A>(1, a));
//		g.insert(CJH::pair<int, A>(1, a));
//		g.insert(CJH::pair<int, A>(7, a));
//		g.insert(CJH::pair<int, A>(799, a));
//		g.insert(CJH::pair<int, A>(430, a));
//		cout << "先inset在clear在insert\n";
//		cout << "size() = " << g.size() << "\t bucket_count()" << g.bucket_count() << "\n";
//		for (it = g.begin(); it != g.end(); ++it){
//			cout << it->first << "\t";//<< it->second << "\n";
//		}
//
//		CJH::deque<CJH::pair<int, A> > v;
//		for (int i = 0; i < NUM; ++i){
//			p.first = rand() % NUM;
//			p.second = a;
//			v.push_back(p);
//		}
//
//		g.insert(v.begin(), v.end());
//		v.clear();
//		cout << "inset(first, last)\n";
//		for (it = g.begin(); it != g.end(); ++it){
//			cout << it->first << "\t";/* << it->second << "\n";*/
//		}
//		cout << "size() = " << g.size() << "\t bucket_count()" << g.bucket_count() << "\n";
//		cout << "find\n";
//		num = rand() % NUM;
//		it = g.find(num);
//		end = g.end();
//
//		cout << (it != end ? (*it).first : 0) << endl;
//		cout << "复制构造函数和Copy AssignMent\n";
//		CJH::hash_map<int, A>  h = g;
//		g = h;
//		cout << "size() = " << g.size() << "\t bucket_count()" << g.bucket_count() << "\n";
//		h.clear();
//		for (it = g.begin(); it != g.end(); ++it){
//			cout << it->first << "\t";/*<< it->second << "\n";*/
//		}
//		g.clear();
//		cout << "size() = " << g.size() << "\t bucket_count()" << g.bucket_count() << "\n";
//		cout << endl;
//		return 0;
//	}






	template<class Key, class _Ty,
	class HashFun = CJH::hash<Key>,
	class Equalkey = CJH::equal<Key>,
	class Alloc = CJH::alloc_malloc>
	class hash_multimap{
	private:
		typedef CJH::pair<Key, _Ty> Value;
		typedef CJH::hashtable<Key, Value, HashFun, Equalkey, Alloc> rep_type;  // take hashtable as basic data structure
		rep_type t;
	public:

		typedef typename rep_type::hasher hasher;
		typedef typename rep_type::key_equal key_equal;
		typedef typename rep_type::key_type key_type;
		typedef typename rep_type::value_type value_type;
		typedef typename rep_type::size_type size_type;
		typedef typename rep_type::pointer_type pointer_type;
		typedef typename rep_type::reference_type reference_type;
		typedef typename rep_type::difference_type difference_type;
		typedef typename rep_type::iterator iterator;
		typedef hash_multimap<Key, _Ty, HashFun, Equalkey, Alloc> self;
	public:
		hash_multimap() :t(){

		}

		explicit hash_multimap(size_type n) :t(n){

		}

		hash_multimap(size_type n, const hasher& hf) :t(n, hf, key_equal()){

		}

		hash_multimap(size_type n, const hasher& hf, const key_equal& key_eq) :t(n, hf, key_eq){

		}

		template<class InputIter>
		hash_multimap(InputIter first, InputIter last){
			t.insert_equal(first, last);
		}

		template<class InputIter>
		hash_multimap(InputIter first, InputIter last, size_type n, const hasher& hf)
			:t(n, hf, key_equal()){
			t.insert_equal(first, last);
		}

		template<class InputIter>
		hash_multimap(InputIter first, InputIter last, size_type n,
			const hasher& hf, const key_equal& key_eq) : t(n, hf, key_eq){
			t.insert_equal(first, last);
		}


		hash_multimap(const self& x) :t(x.t){

		}

		self& operator= (const self& x){
			if (this != &x){
				self tmp(x);
				swap(tmp);
			}

			return *this;
		}

		iterator begin(){
			return t.begin();
		}

		iterator end(){
			return t.end();
		}
		size_type size() const{
			return t.size();
		}

		bool empty() const{
			return t.empty();
		}


		void swap(self& x){
			if (this != &x){
				t.swap(x.t);
			}
		}

		~hash_multimap(){

		}

	public:
		iterator insert(const value_type& value){
			return t.insert_equal(value);
		}

		template<class InputIter>
		void insert(InputIter first, InputIter last){
			t.insert_equal(first, last);
		}

		iterator find(const key_type& key) {
			return t.find(key);
		}

		size_type count(const key_type& key) const{
			return t.count(key);
		}

		CJH::pair<iterator, iterator> equal_range(const key_type& key){
			iterator first = find(key);
			iterator last = first;
			if (first != end())++last;
			return CJH::pair<iterator, iterator>(first, last);
		}

		size_type erase(const key_type& key){
			return t.erase(key);
		}

		iterator erase(iterator position){
			return t.erase(position);
		}

		iterator erase(iterator first, iterator last){
			return t.erase(first, last);
		}

		void clear(){
			t.clear();
		}

		void resize(size_type newsize){
			t.resize(newsize);
		}

		size_type bucket_count() const{
			return t.bucket_count();
		}

		size_type max_bucket_count() const{
			return t.max_bucket_count();
		}

		size_type elems_in_bucket(size_type n) const{
			return t.elems_in_bucket(n);
		}
	};




//
__CJH_END
#endif  //__hash_map_H__
//class A{
//	public:
//
//		static int i;
//		A(){
//			++i;
//			cout << "默认构造函数  " << i << "\n";
//		}
//		A& operator=(const A& a){
//			if (this != &a){
//				this->~A();
//				++i;
//				cout << "Copy Assignment函数  " << i << "\n";
//			}
//			return *this;
//		}
//
//		bool operator==(const A& a) const{
//			return this == &a;
//		}
//
//		bool operator !=(const A& a) const{
//			return !(*this == a);
//		}
//		A(const A& a){
//			++i;
//			cout << "复制构造函数  " << i << "\n";
//		}
//		bool operator <(const A& a){
//			return true;
//		}
//
//		bool operator <(const A& a) const{
//			return true;
//		}
//		bool operator >(const A& a){
//			return true;
//		}
//		~A(){
//			--i;
//			cout << "析构函数" << i << "\n";
//		}
//	};
//
//	int A::i = 0;
//#define NUM 1000
//	int main(){
//		A a;
//		CJH::pair<int, A> p(5, a);
//		//	CJH::hash_map<int, int> a;
//		//	CJH::hash_set<int> hs;
//		//	const int b = get(CJH::pair<int, int>(5, 250));
//		CJH::hash_multimap<int, A> g;
//		CJH::hash_set<int> s;
//		CJH::hash_multimap<int, A>::iterator it, end;
//
//		cout << "size() = " << g.size() << "\t bucket_count()" << g.bucket_count() << "\n";
//		//	cout << g.max_bucket_count() << endl;
//		srand(time(0));
//		int num = 250;
//		g.insert(CJH::pair<int, A>(5, a));
//		g.insert(CJH::pair<int, A>(1, a));
//		g.insert(CJH::pair<int, A>(1, a));
//		g.insert(CJH::pair<int, A>(7, a));
//		g.insert(CJH::pair<int, A>(799, a));
//		g.insert(CJH::pair<int, A>(430, a));
//		g.clear();
//		g.insert(CJH::pair<int, A>(5, a));
//		g.insert(CJH::pair<int, A>(1, a));
//		g.insert(CJH::pair<int, A>(1, a));
//		g.insert(CJH::pair<int, A>(7, a));
//		g.insert(CJH::pair<int, A>(799, a));
//		g.insert(CJH::pair<int, A>(430, a));
//		cout << "先inset在clear在insert\n";
//		cout << "size() = " << g.size() << "\t bucket_count()" << g.bucket_count() << "\n";
//		for (it = g.begin(); it != g.end(); ++it){
//			cout << it->first << "\t";//<< it->second << "\n";
//		}
//
//		CJH::vector<CJH::pair<int, A> > v;
//		for (int i = 0; i < NUM; ++i){
//			p.first = rand() % NUM;
//			p.second = a;
//			v.push_back(p);
//		}
//
//		g.insert(v.begin(), v.end());
//		v.clear();
//		cout << "inset(first, last)\n";
//		for (it = g.begin(); it != g.end(); ++it){
//			cout << it->first << "\t";/* << it->second << "\n";*/
//		}
//		cout << "size() = " << g.size() << "\t bucket_count()" << g.bucket_count() << "\n";
//		cout << "find\n";
//		num = rand() % NUM;
//		it = g.find(num);
//		end = g.end();
//
//		cout << (it != end ? (*it).first : 0) << endl;
//		cout << "复制构造函数和Copy AssignMent\n";
//		CJH::hash_multimap<int, A>  h = g;
//		g = h;
//		cout << "size() = " << g.size() << "\t bucket_count()" << g.bucket_count() << "\n";
//		h.clear();
//		for (it = g.begin(); it != g.end(); ++it){
//			cout << it->first << "\t";/*<< it->second << "\n";*/
//		}
//		g.clear();
//		cout << "size() = " << g.size() << "\t bucket_count()" << g.bucket_count() << "\n";
//		cout << endl;
//		return 0;
//	}



//
//
//
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
//			//		cout << "Copy Assignment函数  " << i << "\n";
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
//		//	cout << "复制构造函数  " << i << "\n";
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

//int A::i = 0;
//#define NUM 400000
//int main(){
//	A a;
//	clock_t start, end1l;
//	CJH::pair<int, A> p(5, a);
//	//	CJH::hash_map<int, int> a;
//	//	CJH::hash_set<int> hs;
//	//	const int b = get(CJH::pair<int, int>(5, 250));
//	CJH::hash_multimap<int, A> g;
//	CJH::hash_set<int> s;
//	CJH::hash_multimap<int, A>::iterator it, end;
//
//	cout << "size() = " << g.size() << "\t bucket_count()" << g.bucket_count() << "\n";
//	//	cout << g.max_bucket_count() << endl;
//	srand(time(0));
//	int num = 250;
//	g.insert(CJH::pair<int, A>(5, a));
//	g.insert(CJH::pair<int, A>(1, a));
//	g.insert(CJH::pair<int, A>(1, a));
//	g.insert(CJH::pair<int, A>(7, a));
//	g.insert(CJH::pair<int, A>(799, a));
//	g.insert(CJH::pair<int, A>(430, a));
//	g.clear();
//	g.insert(CJH::pair<int, A>(5, a));
//	g.insert(CJH::pair<int, A>(1, a));
//	g.insert(CJH::pair<int, A>(1, a));
//	g.insert(CJH::pair<int, A>(7, a));
//	g.insert(CJH::pair<int, A>(799, a));
//	g.insert(CJH::pair<int, A>(430, a));
//	cout << "先inset在clear在insert\n";
//	cout << "size() = " << g.size() << "\t bucket_count()" << g.bucket_count() << "\n";
//	for (it = g.begin(); it != g.end(); ++it){
//		cout << it->first << "\t";//<< it->second << "\n";
//	}
//
//	CJH::vector<CJH::pair<int, A> > v;
//	for (int i = 0; i < NUM; ++i){
//		p.first = rand() % NUM;
//		p.second = a;
//		v.push_back(p);
//	}
//	g.insert(v.begin(), v.end());
//	//	
//	//	g.clear();
//	//	cout << g.size() << "\t" << v.size()<<"\t"<<v.map_size << endl;
//
//	start = clock();
//	g.clear();
//	//	g.erase(g.begin(), g.end());
//	end1l = clock();
//	v.clear();
//	cout << a.i << endl;
//	cout << end1l - start;
//	return 0;
//}
