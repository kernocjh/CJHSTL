#ifndef __HASH_SET_H__
#define __HASH_SET_H__
#include "config.h"
#include "operator.h"
#include "iterator.h"
#include "xmemory.h"
#include "allocator.h"
#include "hashtable.h"

__CJH_BEGIN

	template<class _Ty, class HashFun = CJH::hash<_Ty>,
		class Equalkey = CJH::equal<_Ty>, class Alloc = CJH::alloc_malloc>
	class hash_set{
	public:
		typedef CJH::hashtable<_Ty, _Ty, HashFun, Equalkey, Alloc> rep_type;  // take hashtable as basic data structure
		typedef typename rep_type::hasher hasher;
		typedef typename rep_type::key_equal key_equal;
		typedef typename rep_type::key_type key_type;
		typedef typename rep_type::value_type value_type;
		typedef typename rep_type::size_type size_type;
		typedef typename rep_type::pointer_type pointer_type;
		typedef typename rep_type::reference_type reference_type;
		typedef typename rep_type::difference_type difference_type;
		typedef typename rep_type::iterator iterator;
		typedef hash_set<_Ty, HashFun, Equalkey, Alloc> self;
	public:
		hash_set():t(){

		}

		explicit hash_set(size_type n):t(n){

		}

		hash_set(size_type n, const hasher& hf) :t(n, hf, key_equal()){

		}

		hash_set(size_type n, const hasher& hf, const key_equal& key_eq) :t(n, hf, key_eq){

		}

		template<class InputIter>
		hash_set(InputIter first, InputIter last){
			t.insert_unique(first, last);
		}

		template<class InputIter>
		hash_set(InputIter first, InputIter last, size_type n, const hasher& hf) 
			:t(n, hf, key_equal()){
			t.insert_unique(first, last);
		}

		template<class InputIter>
		hash_set(InputIter first, InputIter last, size_type n, 
			const hasher& hf, const key_equal& key_eq) : t(n, hf, key_eq){
			t.insert_unique(first, last);
		}


		hash_set(const self& x):t(x.t){

		}

		self operator= (const self& x){
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

		~hash_set(){

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
			return t.erase(firts, last);
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

	private:
		rep_type t;
	};










	template<class _Ty, class HashFun = CJH::hash<_Ty>,
	class Equalkey = CJH::equal<_Ty>, class Alloc = CJH::alloc_malloc>
	class hash_multiset{
	public:
		typedef CJH::hashtable<_Ty, _Ty, HashFun, Equalkey, Alloc> rep_type;  // take hashtable as basic data structure
		typedef typename rep_type::hasher hasher;
		typedef typename rep_type::key_equal key_equal;
		typedef typename rep_type::key_type key_type;
		typedef typename rep_type::value_type value_type;
		typedef typename rep_type::size_type size_type;
		typedef typename rep_type::pointer_type pointer_type;
		typedef typename rep_type::reference_type reference_type;
		typedef typename rep_type::difference_type difference_type;
		typedef typename rep_type::iterator iterator;
		typedef hash_multiset<_Ty, HashFun, Equalkey, Alloc> self;
	public:
		hash_multiset() :t(){

		}

		explicit hash_multiset(size_type n) :t(n){

		}

		hash_multiset(size_type n, const hasher& hf) :t(n, hf, key_equal()){

		}

		hash_multiset(size_type n, const hasher& hf, const key_equal& key_eq) :t(n, hf, key_eq){

		}

		template<class InputIter>
		hash_multiset(InputIter first, InputIter last){
			t.insert_equal(first, last);
		}

		template<class InputIter>
		hash_multiset(InputIter first, InputIter last, size_type n, const hasher& hf)
			:t(n, hf, key_equal()){
			t.insert_equal(first, last);
		}

		template<class InputIter>
		hash_multiset(InputIter first, InputIter last, size_type n,
			const hasher& hf, const key_equal& key_eq) : t(n, hf, key_eq){
			t.insert_equal(first, last);
		}


		hash_multiset(const self& x) :t(x.t){

		}

		self operator= (const self& x){
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

		~hash_multiset(){

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
			return t.erase(firts, last);
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

	private:
		rep_type t;
	};
__CJH_END
#endif  //__HASH_SET_H__

//
//
//#define NUM 10000
//int main(){
//	//hs.insert(a);
//	//hs.insert(a);
//	//hs.insert(a);
//	//hs.insert(a);
//	//hs.insert(a);
//
//	////	CJH::hash_set<int> hs;
//	//CJH::hash_multiset<int> g;
//	//CJH::hash_multiset<int>::iterator it, end;
//	//cout << g.max_bucket_count() << endl;
//	//srand(time(0));
//	//g.insert(5);
//	//g.insert(1);
//	//g.insert(1);
//	//g.insert(7);
//	//g.insert(799);
//	//g.insert(430);
//	//g.clear();
//	//g.insert(5);
//	//g.insert(1);
//	//g.insert(1);
//	//g.insert(7);
//	//g.insert(799);
//	//g.insert(430);
//	//cout << "先inset在clear在insert\n";
//	//CJH::copy(g.begin(), g.end(), ostream_iterator<int>(cout, " "));
//	//cout << endl;
//	//
//	//CJH::deque<int> v;
//	//for (int i = 0; i < NUM; ++i){
//	//	v.push_back(rand() % NUM);
//	//}
//	//g.insert(v.begin(), v.end());
//	//cout << "inset(first, last)\n";
//	//CJH::copy(g.begin(), g.end(), ostream_iterator<int>(cout, " "));
//	//cout << endl;
//
//	//cout << "find\n";
//	//int num = rand() % NUM;
//	//it = g.find(num);
//	//end = g.end();
//	//
//	//cout << (it!=end?*it:0) << endl;
//	//cout << "复制构造函数和Copy AssignMent\n";
//	//CJH::hash_multiset<int> h = g;
//	//g = h;
//	//h.clear();
//	//CJH::copy(g.begin(), g.end(), ostream_iterator<int>(cout, " "));
//	//g.clear();
//	//cout << endl;
//	return 0;
//}