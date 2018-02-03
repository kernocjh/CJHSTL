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

		_deque_iterator(const map_pointer pnode) :
			node(pnode0),first(0), cur(0), last(0){

		}
		_deque_iterator(const map_pointer pnode,
			pointer_type pfirst, pointer_type plast, pointer_type pcur) :iterator_base()
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
			/*++cur;
			if (cur != last){
				return *this;
			}
			else{
				set_node(node + 1);
				cur = first;
				return *this;
			}*/

		}

		self operator ++(int){
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
		self operator --(int){
			self tmp(*this);
			--*this;
			return tmp;
		}

		self& operator +=(difference_type n){
			//检查目标是否在缓冲区, offset 是新位置相对于first的偏移量
			difference_type offset = n + difference_type(cur - first);
			if (offset >= 0 && offset < Bufsize){//在同缓冲区
				cur += n;
			}
			else{
			//	difference_type node_offset = (offset > 0 ? offset / difference_type(Bufsize) :
			//		-difference_type((-offset - 1) / difference_type(Bufsize)) - 1);
				difference_type node_offset = (offset > 0 ? offset / difference_type(Bufsize) :
	-(-offset%difference_type(Bufsize) == 0 ? (-offset / difference_type(Bufsize)) : (-offset / difference_type(Bufsize) + 1)));
				set_node(node + node_offset);
			//	cur = first + (offset - node_offset*difference_type(Bufsize));
				if (offset > 0){
					cur = first;
					cur += offset % Bufsize;
				}
				else{
					cur = first;
					cur += -offset * Bufsize % Bufsize;
				}
			}
			return *this;
		}

		self& operator -=(difference_type n){
			return *this += -n;
		}

		self operator+(int n) const{
			self tmp = *this;
			tmp += n;
			return tmp;
		}

		self operator-(int n) const{
			self tmp = *this;
			tmp -= n;
			return tmp;
		}

		difference_type operator-(const self& it) const{
			//这里需要注意offset1中减数迭代器
			//而offset2中为被减数迭代器里面的内容
			difference_type off_node(difference_type(_GetNode() - it._GetNode()));
			difference_type offset1(difference_type(_GetCur()-_GetFirst()));
			difference_type offset2(difference_type(it._GetLast() - it._GetCur()));
			return difference_type(Bufsize)*(off_node - 1) + offset1 + offset2;
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

		pointer_type _GetLast() const{
			return this->last;
		}

		pointer_type _GetFirst() const{
			return this->first;
		}

		pointer_type _GetCur() const{
			return this->cur;
		}

		void _Swap(self& x){
			CJH::swap(node, x.node);
			CJH::swap(first, x.first);
			CJH::swap(last, x.last);
			CJH::swap(cur, x.cur);
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
		typedef typename iterator::difference_type difference_type;
		typedef deque<_Ty, Alloc,Bufsize> self;
	public:
		deque() : map_size(0), start(),finish(){
			init_map();
			create_map_and_node(1);
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

		deque(const self& x):map_size(x.map_size){
			size_type offCur = 0;
			try{
				map = map_allocate::allocate(x.map_size);
				size_type counter = 0;
				for (offCur = 0; offCur < x.map_size; ++offCur){
					if (x.map[offCur] != NULL){
						map[offCur] = data_allocate::allocate();
						++counter;
						if (counter == 1){
							start._SetNode(&map[offCur]);
							start._SetFirst(map[offCur]);
							start._SetLast(map[offCur] + Bufsize);
							start._SetCur(map[offCur] + (x.start._GetCur() - x.start._GetFirst()));
						}
					}
					else{
						map[offCur] = NULL;
					}
				}
				finish = CJH::uninitialized_copy(x.begin(), x.end(), start);
			}
			catch (...){
				destroy_node();
				destroy_map();
				throw;
			}
		}

		self& operator=(const self& x){
			if (this == &x) return *this;
			size_type offCur = 0;
			map_pointer oldmap = map;
			size_type oldmap_size = map_size;
			iterator oldstart = start;
			iterator oldfinish = finish;
			try{
				map = map_allocate::allocate(x.map_size);
				size_type counter = 0;
				for (offCur = 0; offCur < x.map_size; ++offCur){
					if (x.map[offCur] != NULL){
						map[offCur] = data_allocate::allocate();
						++counter;
						if (counter == 1){
							start._SetNode(&map[offCur]);
							start._SetFirst(map[offCur]);
							start._SetLast(map[offCur] + Bufsize);
							start._SetCur(map[offCur] + (x.start._GetCur() - x.start._GetFirst()));
						}
					}
					else{
						map[offCur] = NULL;
					}
				}
				finish = CJH::uninitialized_copy(x.begin(), x.end(), start);
				CJH::destroy(oldstart, oldfinish);
				for (size_type i = 0; i < map_size; ++i){
					if (oldmap[i] != NULL){
						data_allocate::deallocate(oldmap[i]);
					}
					oldmap[i] = NULL;
				}
				map_allocate::deallocate(oldmap, oldmap_size);
				oldmap = NULL;

			}
			catch (...){
				destroy_node();
				destroy_map();
				finish = oldfinish;
				start = oldstart;
				map_size = oldmap_size;
				map = oldmap;
				throw;
			}


			return *this;
		}


		void push_back(const _Ty& value){
			if (finish._GetCur() != finish._GetLast() - 1){
				CJH::construct(CJH::addressof(*finish), value);
				++finish;
			}
			else{
				push_back_aux(value);
			}
		}

		void push_front(const _Ty& value){
			if (start._GetCur() != start._GetFirst()){
				CJH::construct(--start, value);
			}
			else{
				push_front_aux(value);
			}
		}

		void pop_back(){
			if (finish._GetCur() != finish._GetFirst()){
				--finish;
				CJH::destroy(CJH::addressof(*finish));
			}
			else{
				map_pointer node = finish._GetNode();
				--finish;
				CJH::destroy(CJH::addressof(*finish));
				data_allocate::deallocate(*node);
				*node = NULL;
			}
		}

		void pop_front(){

			//if (empty())  将会抛出编译错误

			CJH::destroy(CJH::addressof(*start));
			if (start._GetCur() + 1 != start._GetLast()){
				++start;
			}
			else{
				map_pointer node = start._GetNode();
				++start;
				data_allocate::deallocate(*node);
				*node = NULL;
			}
		}

		iterator erase(iterator position){
			erase(position, position + 1);
		}

		iterator erase(iterator first, iterator last){
			if (first == start&&last == finish){
				clear();
				return finish;
			}
			else{
				difference_type diff = last - first;
				CJH::destroy(first, last);
				difference_type elementnum_before = first - start;  //计算删除区间之前的元素
				difference_type elementnum_last = finish - last;	//计算删除区间之后的元素
				if (elementnum_before < elementnum_last){  //前面的元素少删除移动前面
					iterator new_start = CJH::copy_backward(start, first, last);  // 将[start, first)之间的元素移动last之前  如 1 2 3 先复制 3 2 1
					for (map_pointer node = start._GetNode(); node != new_start._GetNode(); ++node){ //回收多余的空间
						data_allocate::deallocate(*node);
						*node = NULL;
					}
					start = new_start;
				}
				else{
					iterator new_finish = CJH::copy(last, finish, first); // 将[last, finish)之间的元素移动first之后 如 1 2 3 先复制 3 2 1
					for (map_pointer node = finish._GetNode(); node != new_finish._GetNode(); --node){
						data_allocate::deallocate(*node);
						*node = NULL;
					}
					finish = new_finish;
				}
				return start + elementnum_before;
			}
		}


		iterator insert(iterator positon, const _Ty & value){
			return insert(positon, 1, value);
		}

		iterator insert(iterator positon, size_type n, const _Ty & value){
			difference_type elementnum_before = positon - start;
			difference_type elementnum_last = finish - positon;
			if (elementnum_before < elementnum_last){ //前面的元素少于后面的元素
				//查看前方的声誉空间
				size_type _Curoff = 0;
				try{
					for (_Curoff = 0; _Curoff < n; ++_Curoff)
						push_front(value);
				}
				catch (...){
					for (size_type i = 0; i < _Curoff; ++i){
						pop_front();
					}
					throw;
				}

				CJH::reverse(begin(), begin() + n); 
				CJH::rotate(begin(), begin() + n, begin() + n + elementnum_before);
			}
			else{
				size_type _Curoff = 0;
				try{
					for (_Curoff = 0; _Curoff < n; ++_Curoff)
						push_back(value);
				}
				catch (...){
					for (size_type i = 0; i < _Curoff; ++i){
						pop_back();
					}
					throw;
				}
				CJH::rotate(begin() + elementnum_before, begin() + elementnum_before + elementnum_last, end());
			}

			return start + elementnum_before + n;
		}

		template<class InputIter>
		void insert(iterator position, InputIter first, InputIter last){
			difference_type elementnum_before = position - start;
			difference_type elementnum_last = finish - position;
			if (elementnum_before < elementnum_last){ //前面的元素少于后面的元素
				//查看前方的声誉空间
				size_type _Curoff = 0;
				try{
					for (first; first != last; ++first){
						push_front(*first);
						++_Curoff;
					}
				}
				catch (...){
					for (size_type i = 0; i < _Curoff; ++i){
						pop_front();
					}
					throw;
				}

				CJH::reverse(begin(), begin() + _Curoff);
				CJH::rotate(begin(), begin() + _Curoff, begin() + _Curoff + elementnum_before);
			}
			else{
				size_type _Curoff = 0;
				try{
					for (first; first != last; ++first){
						push_back(*first);
						++_Curoff;
					}
						
				}
				catch (...){
					for (size_type i = 0; i < _Curoff; ++i){
						pop_back();
					}
					throw;
				}
				CJH::rotate(begin() + elementnum_before, begin() + elementnum_before + elementnum_last, end());
			}
		}

		void clear(){
			CJH::destroy(start, finish);
			for (map_pointer node = start._GetNode(); node != finish._GetNode(); ++node){
				data_allocate::deallocate(*node);
				*node = NULL;
			}
			start = finish;
		}
		iterator begin() const{
			return iterator(start);
		}

		iterator end() const{
			return iterator(finish);
		}

		reference_type back() const{
			return (*(end() - 1));
		}
		reference_type front() const{
			return (*(begin()));
		}
		size_type size() const{
			return size_type(finish - start);
		}

		value_type& operator[] (int n){
			return (*(begin() + n));
		}
		bool empty(){
			return finish == start;
		}


		void resize(const size_type n){
			if (size() == n) return;
			else if (size() > n){
				erase(begin() + 5, end());
			}
			else{
				insert(end(), n - size(), value_type());
			}
		}

		void swap(self& x){
			CJH::swap(map, x.map);
			CJH::swap(map_size, x.map_size);
			start._Swap(x.start);
			finish._Swap(x.finish);
		}
	protected:

		inline void push_front_aux(const _Ty& value){
			//情况1:若前方有map位， 则直接申请空间
			if (map[0] == NULL){
				map_pointer newnode = start._GetNode();
				try{
					--newnode;
					*newnode = data_allocate::allocate();
					--start;
					CJH::uninitialized_fill_n(start, 1, value);
				}
				catch (...){
					data_allocate::deallocate(*newnode);
				}
			}
			else{//若前方无map位，更新map空间
				update_map_front();
				push_front_aux(value);
			}
		}

		inline void push_back_aux(const value_type& value){
			//情况一:若为一个元素都没有的加入
			if (map == NULL){
				try{
					create_map_and_node(1);
					finish = CJH::uninitialized_fill_n(finish, 1, value);
				}
				catch (...){
					throw;
				}
			}
			else{
				//情况二:若在map还有节点为NULL 只用冲申请节点即可， 这里表示当前finish指向的缓冲只能容纳一个元素了  所以需要开辟新空间以供给finishi进行++操作 
				if (NULL == map[map_size - 1]){
					map_pointer curnode = finish._GetNode();
					++curnode;
					try{
						if (*curnode != NULL) throw;
						else{
							*curnode = (pointer_type)data_allocate::allocate();
						}
						finish = CJH::uninitialized_fill_n(finish, 1, value);
					}
					catch (...){
						data_allocate::deallocate(*curnode);
						throw;
					}
				}
				//情况三:若map都没有了位置， 则需要更新map在加入节点 
				else{
					update_map_back();
					push_back_aux(value);
				}
			}
		}
		/** update 函数还有大bug有待更新*/
		inline void update_map_front(){
			map_pointer newmap = NULL;
			size_type newmap_size = map_size / 2 + 1 + map_size;
			try{
				newmap = map_allocate::allocate(newmap_size);
				int index = newmap_size;
				for (int i = map_size - 1; i >= 0; --i){
					newmap[--index] = map[i];
				}
				for (int i = index; i > 0;){
					newmap[--i] = NULL;
				}
				//更新 迭代器 start finish  更新水位
				destroy_map();
				map = newmap;
				start._SetNode(&map[index]);
				for (int i = newmap_size - 1; i >= 0; --i){
					if (map[i] != NULL){
						finish._SetNode(&map[i]);
						break;
					}
				}
				map_size = newmap_size;
			}
			catch (...){
				map_allocate::deallocate(newmap, newmap_size);
				throw;
			}
		}

		inline void update_map_back(){
			map_pointer newmap = NULL;
			size_type newmap_size = map_size / 2 + 1 + map_size;
			try{
				newmap = map_allocate::allocate(newmap_size);
				for (size_type i = 0; i < map_size; ++i){
					newmap[i] = map[i];
				}
				for (size_type i = map_size; i < newmap_size; ++i){
					newmap[i] = NULL;
				}
				//更新 迭代器 start finish
				destroy_map();
				map = newmap;
				for (size_type i = 0; i < map_size; ++i){
					if (map[i] != NULL){
						start._SetNode(&map[i]);
						break;
					}
				}
				finish._SetNode(&map[map_size - 1]);
				map_size = newmap_size;
			}
			catch (...){
				map_allocate::deallocate(newmap, newmap_size);
				throw;
			}

		}

		inline void create_map_and_node(size_type n){
			if (n == 0) return;
		//	int mapelementNum = n / Bufsize + 1;//初始化map元素的每个， 每一个元素维护这一个Bufsize个元素的一维数组
			size_type mapelementNum = n%Bufsize != 0 ? n / Bufsize + 1 : n / Bufsize;
			//先初始化map
			if (mapelementNum > map_size) map_size = mapelementNum + 2;
			try{
				init_map();
			}
			catch (...){
				destroy_map();
				throw;
			}

			size_type elementinited = 1;
			//在初始化每一个map的元素
			try{
				elementinited = 1;
				for (elementinited = 1; elementinited < mapelementNum + 1; ++elementinited){
					map[elementinited] = data_allocate::allocate();
				}

			}
			catch (...){
				for (size_type i = 1; i < elementinited; ++i){
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
			if (map_size == 0)return;
			try{
				map = map_allocate::allocate(map_size);
			}
			catch (...){
				map_allocate::deallocate(map, map_size);
				map = NULL;
				throw;
			}

			for (size_type i = 0; i < map_size; ++i){
				map[i] = NULL;
			}
		}

		inline  void destroy_map(){
			map_allocate::deallocate(map, map_size);
			map = NULL;
		}


		inline void destroy_node(){
		/*	for (map_pointer node = start._GetNode(); node <= finish._GetNode(); ++node){
				data_allocate::deallocate(*node);
				*node = NULL;
			}*/
			for (size_type i = 0; i < map_size; ++i){
				if (map[i] != NULL){
					data_allocate::deallocate(map[i]);
				}
				map[i] = NULL;
			}
		}
	public:
		map_pointer map;
		size_type map_size;
		iterator start;
		iterator finish;
	};
__CJH_END
#endif _DEQUE_H__

//inset 函数综合测试
//std::deque<int> dq;
//dq.insert(dq.end(), dq.begin(), dq.end());
//CJH::deque<int, CJH::allocator<int>, 1> q;
//
//q.push_back(1);
//
//CJH::copy(q.begin(), q.end(), ostream_iterator<int>(cout, " "));
//cout << endl;
//CJH::list<int> v(10, 250);
//q.insert(q.begin(), v.begin(), v.end());
////	q.push_front(7);
//CJH::copy(q.begin(), q.end(), ostream_iterator<int>(cout, " ")); cout << endl;
//CJH::vector<int> v1(5, 3600);
//q.insert(q.begin() + 7, v1.begin(), v1.end());
//CJH::copy(q.begin(), q.end(), ostream_iterator<int>(cout, " ")); cout << endl;
//
//CJH::copy(q.end() - 9, q.end(), ostream_iterator<int>(cout, " ")); cout << endl;
//q.insert(q.end() - 11, v1.begin(), v1.end());
//CJH::copy(q.begin(), q.end(), ostream_iterator<int>(cout, " ")); cout << endl;

//主要是对erase 和 clear和删除有关的函数做测试
//#define CIRNUM 100
//class A{
//public:
//	int i;
//	static int counter;
//	int *num;
//	A() :i(++counter){
//		num = new int[CIRNUM];
//		cout << "i = " << i << "\t默认构造函数 counter = " << counter << "\n";
//	}
//
//	A(const A& a){
//		++counter;
//		this->i = a.i;
//		this->num = new int[CIRNUM];
//		cout << "i = " << i << "\t复制构造函数 counter = " << counter << "\n";
//	}
//
//	A& operator =(const A& a){
//		++counter;
//		this->i = a.i;
//		cout << "i = " << i << "\tCopy Assigment函数 counter = " << counter << "\n";
//		return *this;
//	}
//	~A(){
//		delete[]num;
//		cout << "i = " << i << "\t析构函数 counter = " << counter << "\n";
//		--counter;
//	}
//};
//
//ostream& operator <<(ostream& os, const A& a){
//	os << a.i;
//	return os;
//}
//
//int A::counter = 0;
//int main(){
//	CJH::deque<int, CJH::allocator<int>, 100> p;
//	for (int i = 0; i < CIRNUM; ++i)
//		p.push_back(i);
//	CJH::copy(p.begin(), p.end(), ostream_iterator<int>(cout, " "));
//	cout << endl;
//	p.erase(p.begin() + 10, p.begin() + 20);
//	CJH::copy(p.begin(), p.end(), ostream_iterator<int>(cout, " "));
//
//	cout << endl;
//	A a;
//	CJH::deque<A, CJH::allocator<A>, 100> q;
//	cout << q.size() << endl;
//	for (int i = 0; i < CIRNUM; ++i)
//		q.push_back(a);
//
//	cout << q.size() << endl;
//	CJH::copy(q.begin(), q.end(), ostream_iterator<A>(cout, " "));
//
//	cout << endl;
//	q.erase(q.begin(), q.begin() + 5);
//	cout << q.size() << endl;
//	q.push_front(a);
//	q.push_back(a);
//	CJH::copy(q.begin(), q.end(), ostream_iterator<A>(cout, " "));
//	cout << q.size() << endl;
//	cout << endl;
//	return 0;
//}



//主要是对A做专项测试随机测试  还有就是A预期析构函数测试
//#define CIRNUM 100000000
//class A{
//public:
//	int i;
//	static int counter;
//	A() :i(++counter){
//		cout << "i = " << i << "\t默认构造函数 counter = " << counter << "\n";
//	}
//
//	A(const A& a){
//		++counter;
//		this->i = a.i;
//		cout << "i = " << i << "\t复制构造函数 counter = " << counter << "\n";
//	}
//
//	A& operator =(const A& a){
//		++counter;
//		this->i = a.i;
//		cout << "i = " << i << "\tCopy Assigment函数 counter = " << counter << "\n";
//		return *this;
//	}
//	~A(){
//		--counter;
//		cout << "i = " << i << "\t析构函数 counter = " << counter << "\n";
//	}
//};
//
//ostream& operator <<(ostream& os, const A& a){
//	os << a.i;
//	return os;
//}
//
//int A::counter = 0;
//int main(){
//
//	CJH::deque<int, CJH::allocator<int>, 3> q;
//	srand(time(0));
//	for (int i = 0; i < CIRNUM; ++i){
//
//		int n = rand() % CIRNUM / 2;
//		for (int j = 0; j < n; ++j){
//			int num = rand() % 2;
//			if (num == 0)
//				q.push_back(rand() % CIRNUM);
//			else if (num == 1)
//				q.push_front(rand() % CIRNUM);
//		}
//
//		for (int j = 0; j < n; ++j){
//			int num = rand() % 3;
//			if (num == 1)
//				q.pop_back();
//			else if (num == 2)
//				q.pop_front();
//			else;
//		}
//	}
//	CJH::copy(q.begin(), q.end(), ostream_iterator<int>(cout, " "));
//	cout << endl;
//	/*A a;
//	cout << a;
//	CJH::deque<A,CJH::allocator<A>,99> q;
//	for (int i = 0; i < CIRNUM; ++i)
//	q.push_back(a);
//	CJH::copy(q.begin(), q.end(), ostream_iterator<A>(cout, " "));
//	cout << endl;
//	for (int i = 0; i < CIRNUM; ++i)
//	q.pop_back();
//	q.push_front(a);
//	q.push_back(a);
//	CJH::copy(q.begin(), q.end(), ostream_iterator<A>(cout, " "));*/
//	cout << endl;
//	return 0;
//}






//测试代码2  主要对push_back和push_front
//clock_t start, end;
//start = clock();
//CJH::deque<int, CJH::allocator<int>, 100>q;
//CJH::deque<int>::iterator it;
//int front = 0;
//int back = 0;
//srand(time(0));
//for (int i = 1; i <= CIRNUM; ++i){
//	if (i == CIRNUM)
//		i = i;
//	if (i % 2 == 1){
//		front++;
//		q.push_back(1);
//	}
//	else{
//		back++;
//		q.push_front(1);
//	}
//}
//
///*for (int j = 0; j < 10; ++j){
//for (int i = 1; i <= CIRNUM; ++i){
//if (i + 1 == CIRNUM)
//i = i;
//q.push_front(i);
//}
//for (int i = 1; i <= CIRNUM; ++i){
//if (i + 1 == CIRNUM)
//i = i;
//q.push_back(i);
//}
//}*/
//end = clock();
//cout << "time = " << end - start << endl;
//cout << "front = " << front << "\t" << "back = " << back << endl;
////	it = q.begin();
//int i = 0;
///*for (it; it != q.end(); ++it){
//++i;
//}*/
////	cout << i << endl;
////	CJH::copy(q.begin(), q.end(), ostream_iterator<int>(cout, "\n"));
//cout << "time = " << end - start << endl;
//




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