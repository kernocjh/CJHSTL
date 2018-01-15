#ifndef _DEQUE_H__
#define _DEQUE_H__

#include "config.h"
#include "operator.h"
#include "allocator.h"

__CJH_BEGIN

using namespace CJH;

	
	template<class mapelement_type, class Alloc = CJH::allocator<mapelement_type> >
	class map_allocate{
	public:
		typedef mapelement_type value_type;
		typedef mapelement_type* pointer_type;
		typedef size_t size_type;
		static pointer_type allocate(size_type n){
			size_t nbytes = n*sizeof(value_type);
			return Alloc::allocate(nbytes);
		}

		static void deallocate(pointer_type ptr, size_type n){
			size_t nbytes = n*sizeof(value_type);
			Alloc::deallocate(ptr, nbytes);
		}
	};
	template<class _Ty,class Alloc = CJH::allocator<_Ty>, size_t Bufsize = 512>
	class deque_allocate_map{
	public:
		typedef _Ty value_type;
		typedef _Ty* pointer_type;
		typedef _Ty& reference_type;
		typedef size_t size_type;
		typedef _Ty* mapelement_type;
		typedef mapelement_type* map_pointer;
		static pointer_type allocate(){
			size_t nbyte = sizeof(_Ty)*Bufsize;
			return (pointer_type)Alloc::allocate(nbyte);
		}

		static void deallocate(pointer_type ptr){
			size_t nbyte = sizeof(_Ty)*Bufsize;
			Alloc::deallocate(ptr, nbyte);
		}
	};


	template<class _Ty, class Alloc = deque_allocate_map<_Ty, CJH::allocator<_Ty>, Bufsize>,
	class Category = randow_access_iterator_tag, size_t Bufsize = 512>
	class _deque_iterator :public iterator_base<Category,_Ty>{
	public:
		typedef iterator_base<Category, _Ty> _Mybase;

		typedef typename _Mybase::pointer_type pointer_type;
		typedef typename _Mybase::value_type value_type;
		typedef typename _Mybase::difference_type difference_type;
		typedef typename _Mybase::reference_type reference_type;
		typedef typename _Mybase::iterator_category iterator_category;
		typedef _deque_iterator<_Ty, Alloc, Category, Bufsize> self; //在定义自己的时候必须不能一直默认， 如果这里去掉Bufsize就有可能出现模板错误
		typedef _Ty* mapelement_type;
		typedef mapelement_type* map_pointer;
	public:

		_deque_iterator() :iterator_base(),
			node(0), first(0), last(0), cur(0){

		}

		_deque_iterator(const map_pointer pnode) :iterator_base(), 
			node(pnode0),first(0), cur(0), last(0){

		}
		_deque_iterator(const map_pointer pnode,
			pointer_type pfirst, pointer_type plast, pointer_type pcur) :iterator_base(),
			node(ptr), first(pfirst), last(plast), cur(pcur){

		}

		_deque_iterator(const self& it) :iterator_base(),
			node(it.node), cur(it.cur), first(it.first), last(it.last){

		}

		self& operator= (const self& it){
			this->node = it.node;
			this->cur = it.cur;
			this->first = it.first;
			this->last = it.last;
			return *this;
		}
		reference_type operator*() const{
			return *cur;
		}

		pointer_type operator->(){
			return cur;
		}
		bool operator ==(const self& it) const{
			if (this->node != it.node) return false;
			if (this->cur != it.cur) return false;
			if (this->first != it.first) return false;
			if (this->last != it.last) return false;
			return true;
		}

		bool operator != (const self& it) const{
			return !(*this == it);
		}

		
		self& operator ++(){
			++cur;
			if (cur != last){
				return *this;
			}
			else{
				set_node(node + 1);
				cur = first;
				return *this;
			}

		}

		self& operator ++(int){
			self tmp(*this);
			++*this;
			return tmp;
		}

		self& operator --(){
			if (cur == first){
				set_node(node - 1);
				cur = last - 1;
				return *this;
			}
			--cur;
			return *this;
		}
		self& operator --(int){
			self tmp(*this);
			--*this;
			return tmp;
		}

		self& operator +=(difference_type n){
			//检查目标是否在缓冲区
			difference_type offset = n + difference_type(cur - first);
			if (offset >= 0 && offset < Bufsize){//在同缓冲区
				cur += n;
			}
			else{
				difference_type node_offset = offset > 0 ? offset / difference_type(Bufsize) :
					offset / difference_type(Bufsize) - 1;

				set_node(node + node_offset);
				if (offset > 0){
					cur = first;
					cur += offset % Buffsize;
				}
				else{
					cur = first;
					cur += -node_offset * Bufsize % Bufsize;
				}
			}
			return *this;
		}

		self& operator -=(difference_type n){
			return *this += -n;
		}

		self& operator+(int n) const{
			self tmp = *this;
			tmp += n;
			return tmp;
		}

		self& operator-(int n) const{
			self tmp = *this;
			tmp -= n;
			return tmp;
		}

		difference_type operator-(const self& it) const{
			difference_type off_node(difference_type(this->node) - difference_type(it.node));
			difference_type offset1(difference_type(this->last) - difference_type(this->cur));
			difference_type offset2(difference_type(it->cur) - difference_type(it->first));
			return difference_type(Bufsize)*off_node + offset1 + offset2;
		}

		void _SetNode(map_pointer _node){
			this->node = _node;
		}
		void _SetFirst(pointer_type _first){
			this->first = _first;
		}
		void _SetCur(pointer_type _cur){
			this->cur = _cur;
		}
		void _SetLast(pointer_type _last){
			this->last = _last;
		}

		map_pointer _GetNode() const{
			return this->node;
		}
	private:
		map_pointer node;
		pointer_type first;
		pointer_type last;
		pointer_type cur;

		inline void set_node(map_pointer newnode){
			node = newnode;
			first = *node;
			last = first + Bufsize;
		}
	};
	
	//static const size_t MAP_DEFAULT_SIZE = 4;
	
	template<class _Ty, class Alloc =  CJH::allocator<_Ty> ,size_t Bufsize = 512>
	class deque{
	public:
		typedef _Ty value_type;
		typedef value_type* pointer_type;
		typedef value_type& reference_type;
		typedef size_t size_type;
		typedef _Ty* mapelement_type;
		typedef mapelement_type* map_pointer;

		typedef map_allocate<mapelement_type, CJH::allocator<mapelement_type> > map_allocate;

		typedef deque_allocate_map<_Ty, Alloc, Bufsize> data_allocate;

		typedef _deque_iterator<_Ty, data_allocate,
			randow_access_iterator_tag, Bufsize> iterator;
		typedef deque<_Ty, Alloc,Bufsize> self;
	public:
		deque() :start(), map_size(0), start(),finish(){
			init_map();
		}
		deque(const size_type n, const value_type& value) :map_size(0), start(), finish(){
			//构造节点
			create_map_and_node(n);
			try{
				//初始化
				iterator cur(start);
				finish = CJH::uninitialized_fill_n(cur, n, value);
			}
			catch (...){
				//前调用析构函数
				CJH::destroy(begin(), end());
				destroy_node();
				destroy_map();
				throw;
			}
		}
		~deque(){
			destroy_map();
		}

		deque(const self& x){

		}

		self& operator=(const self& x){
			return *this;
		}


		void push_back(const _Ty& value){

		}

		void push_front(const _Ty& value){

		}

		void pop_back(){

		}

		void pop_front(){

		}

		iterator erase(iterator position){

		}

		iterator erase(iterator first, iterator last){

		}


		iterator insert(iterator positon, const _Ty & value){

		}

		void insert(iterator positon, size_type n, const _Ty & value){

		}


		template<class InputIter>
		void insert(InputIter first, InputIter last){

		}

		template<class InputIter>
		void insert(iterator position, InputIter first, InputIter last){

		}

		void clear(){

		}
		iterator begin() const{
			return iterator(start);
		}

		iterator end() const{
			return iterator(finish);
		}

		size_type size(){

		}

		bool empty(){

		}


		bool resize(const size_type n){

		}

		void swap(self& x){

		}
	protected:
		inline void create_map_and_node(size_type n){
			if (n == 0) return;
			int mapelementNum = n / Bufsize + 1;//初始化map元素的每个， 每一个元素维护这一个Bufsize个元素的一维数组

			//先初始化map
			if (mapelementNum > map_size) map_size = mapelementNum + 2;
			try{
				init_map();
			}
			catch (...){
				destroy_map();
				throw;
			}

			int elementinited = 1;
			//在初始化每一个map的元素
			try{
				elementinited = 1;
				for (elementinited = 1; elementinited < mapelementNum + 1; ++elementinited){
					map[elementinited] = data_allocate::allocate();
				}

			}
			catch (...){
				for (int i = 1; i < elementinited; ++i){
					data_allocate::deallocate(map[i]);
					map[i] = NULL;
				}
				destroy_map();
				throw;
			}

			start._SetNode(map + 1);
			start._SetFirst(*(map + 1));
			start._SetLast(*(map + 1) + Bufsize);
			start._SetCur(*(map + 1));

			finish._SetNode(map + mapelementNum);
			finish._SetFirst(*(map + mapelementNum));
			finish._SetLast(*(map + mapelementNum) + Bufsize);
			finish._SetCur(*(map + mapelementNum));
		}
		inline  void init_map(){
			try{
				map = map_allocate::allocate(map_size);
			}
			catch (...){
				map_allocate::deallocate(map, map_size);
				throw;
			}

			for (size_type i = 0; i < map_size; ++i){
				map[i] = NULL;
			}
		}

		inline  void destroy_map(){
			map_allocate::deallocate(map, map_size);
		}


		inline void destroy_node(){
			for (map_pointer node = start._GetNode(); node <= finish._GetNode(); ++node){
				data_allocate::deallocate(*node);
			}
		}
	private:
		map_pointer map;
		size_type map_size;
		iterator start;
		iterator finish;
	};
__CJH_END
#endif _DEQUE_H__















//主要是对构造函数和迭代器进行测试
////	clock_t start, end;
//start = clock();
////	std::deque<int, CJH::allocator<int>, 500000> q(100000000, 1);
//CJH::deque<int>q(10000000, 1);
////	std::vector<int>v(100000000, 1);
////	CJH::deque<int, CJH::allocator<int>, 500000>::iterator it = q.begin();
//end = clock();
//int i = 0;
////for (it; it != q.end(); ++it){
////	++i;
//////	cout << ++i << "\t" << *it << endl;
////}
//cout << end - start << "\t" << i;
////	CJH::copy(q.begin(), q.end(), ostream_iterator<int>(cout, " "));