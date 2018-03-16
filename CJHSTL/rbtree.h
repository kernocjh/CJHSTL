#ifndef __RBTREE_H__
#define __RBTREE_H__
#include "config.h"
#include "allocator.h"
#include "iterator.h"
#include "operator.h"
#include "xmemory.h"

__CJH_BEGIN

	enum Color { red, black };
	template<class Key, class _Ty>
	struct _RBtreeNode{
		typedef struct _RBtreeNode<Key, _Ty>* RBtreeNodePtr;
		typedef Key key_type;
		typedef _Ty value_type;
		typedef value_type* pointer_type;
		typedef value_type& reference_type;
		typedef size_t size_type;
		Key key;
		_Ty value;
		Color color;
		RBtreeNodePtr left;
		RBtreeNodePtr right;
		RBtreeNodePtr parent;
		_RBtreeNode(const Key& _key, const _Ty& _value) : key(_key), value(_value),
			left(NULL), right(NULL), parent(NULL), color(red){
		
		}

		_RBtreeNode() :key(), value(), 
			left(NULL), right(NULL), parent(NULL), color(red){
		
		}
	};

	template<class Key, class _Ty>
	 struct _RBtree{
		typedef _RBtreeNode<Key, _Ty> RBtreeNode;
		typedef typename RBtreeNode::RBtreeNodePtr RBtreeNodePtr;
		RBtreeNodePtr root;
		RBtreeNodePtr NIL;
		_RBtree() :root(NULL), NIL(NULL){

		}

		_RBtree(const _RBtree& rb) :root(rb.root), NIL(rb.NIL){

		}

		_RBtree(const RBtreeNodePtr& _root, const RBtreeNodePtr& _NIL) :root(_root), NIL(_NIL){

		}
	};

	template<class Key, class _Ty, class Alloc=CJH::allocator<_RBtreeNode<Key, _Ty> > >
	class rbtreenode_allocator{
	public:
		typedef typename Alloc::value_type value_type;  //以一个node为标准
		typedef typename Alloc::pointer pointer_type;
		typedef typename Alloc::size_type size_type;
		typedef typename Alloc::different_type different_type;
		static pointer_type allocate(){
			size_t nbyte = sizeof(value_type);
			return (pointer_type)Alloc::allocate(nbyte);
		}
		static pointer_type allocate(const size_type n){
			size_t nbyte = sizeof(value_type)*n;
			return (pointer_type)Alloc::allocate(nbyte);
		}

		static void deallocate(pointer_type ptr){
			size_t nbyte = sizeof(value_type);
			Alloc::deallocate(ptr, nbyte);
		}

		static void deallocate(pointer_type ptr, const size_type n){
			size_t nbyte = sizeof(value_type)*n;
			Alloc::deallocate(ptr, nbyte);
		}
	};
	
	template<class Key, class _Ty, class RBtreeNode = _RBtreeNode<Key, _Ty>,
			class Category = CJH::bidirectinal_iterator_tag>
	class rbtree_iterator:public iterator_base<Category, _Ty> {
	public:
		typedef iterator_base<Category, _Ty> _Mybase;
		typedef typename _Mybase::difference_type difference_type;
		typedef typename _Mybase::iterator_category iterator_category;

		typedef typename RBtreeNode::size_type size_type;
		typedef typename RBtreeNode::key_type key_type;
		typedef typename RBtreeNode::value_type value_type;
		typedef typename RBtreeNode::pointer_type pointer_type;
		typedef typename RBtreeNode::reference_type reference_type;
		typedef typename RBtreeNode::RBtreeNodePtr RBtreeNodePtr;
		typedef _RBtree<Key, _Ty> RBtree;
		typedef RBtree* RBtreePtr;
		typedef rbtree_iterator<Key, _Ty, RBtreeNode, Category> self;
	public:
		rbtree_iterator() :iterator_base<Category, _Ty>(), node(){

		}
		explicit rbtree_iterator(const RBtree& rb) :node(rb){

		}
		rbtree_iterator(const self& x) : node(x.node){

		}

		rbtree_iterator(const RBtreeNodePtr root, const RBtreeNodePtr NIL) :node(root, NIL){

		}

		self& operator=(const self& x){
			if (this != &x){
				node.NIL = x.node.NIL;
				node.root = x.node.root;
			}
			return *this;
		}

		reference_type operator *(){
			return (node.root->value);
		}

		pointer_type operator ->(){
			return (&(operator*()));
		}


		bool operator ==(const self& x){
			return (node.NIL == x.node.NIL && node.root == x.node.root);
		}
		
		bool operator !=(const self& x){
			return !(*this == x);
		}

		self& operator ++(){
			//当当前节点的root和NIL都指向NIL的时候的情况是否抛出异常有待检验
			RBtreeNodePtr tmp = NULL;
			if (node.root->right != node.NIL){// judge it is right child
			
				tmp = node.root->right;
				while (tmp->left != node.NIL)
					tmp = tmp->left;
				node.root = tmp;
			}
			else{ //it is not tight child
				tmp = node.root;
				while (tmp->parent != node.NIL && tmp->parent->right == tmp)  // tmp->parent != node.NIL  this condition must be think out
					tmp = tmp->parent;
				node.root = tmp->parent;
			}
			return *this;
		}

		self operator ++(int){
			self tmp(*this);
			++*this;
			return tmp;
		}

		self& operator --(){
			if (node.NIL == node.root){
				node.root = node.NIL->right;
			}
			else{
				RBtreeNodePtr tmp = NULL;
				if (node.root->left != node.NIL){
					tmp = node.root->left;
					while (tmp->right != node.NIL)
						tmp = tmp->right;
					node.root = tmp;
				}
				else{
					tmp = node.root;
					while (tmp->parent != node.NIL && tmp->parent->left == tmp)
						tmp = tmp->parent;
					node.root = tmp->parent;
				}
			}
			return *this;
		}

		self operator--(int){
			self tmp(*this);
			--*this;
			return tmp;
		}


		~rbtree_iterator(){
		}

		void _SetNode(const RBtree& rb){
			node.NIL = rb.NIL;
			node.root = rb.root;
		}

		void _SetNode_Cur(RBtreeNodePtr ptr){
			node.root = ptr;
		}
		void _SetNode_NIL(RBtreeNodePtr ptr){
			node.NIL = ptr;
		}

		RBtreePtr _GetNode() const{
			return &node;
		}

		RBtreeNodePtr _GetNode_Cur() const{
			return node.root;
		}

		RBtreeNodePtr _GetNode_NIL() const{
			return node.NIL;
		}

		void swap(self& x){
			if (this != &x){
				CJH::swap(node.NIL, x.node.NIL);
				CJH::swap(node.root, x.node.root);
			}
		}
	private:
		RBtree node;
	};

	template<class Key, class _Ty, class RBtreeNode = _RBtreeNode<Key, _Ty>,
				class Category = CJH::bidirectinal_iterator_tag>
	class rbtree_reverse_iterator{
	public:
		typedef iterator_base<Category, _Ty> _Mybase;
		typedef typename _Mybase::difference_type difference_type;
		typedef typename _Mybase::iterator_category iterator_category;

		typedef typename RBtreeNode::size_type size_type;
		typedef typename RBtreeNode::key_type key_type;
		typedef typename RBtreeNode::value_type value_type;
		typedef typename RBtreeNode::pointer_type pointer_type;
		typedef typename RBtreeNode::reference_type reference_type;
		typedef typename RBtreeNode::RBtreeNodePtr RBtreeNodePtr;
		typedef _RBtree<Key, _Ty> RBtree;
		typedef RBtree* RBtreePtr;
		typedef rbtree_reverse_iterator<Key, _Ty, RBtreeNode, Category> self;
	public:
		rbtree_reverse_iterator() :iterator_base<Category, _Ty>(), node(){

		}
		explicit rbtree_reverse_iterator(const RBtree& rb) :node(rb){

		}
		rbtree_reverse_iterator(const self& x) : node(x.node){

		}

		rbtree_reverse_iterator(const RBtreeNodePtr root, const RBtreeNodePtr NIL) :node(root, NIL){

		}

		self& operator=(const self& x){
			if (this != &x){
				node.NIL = x.node.NIL;
				node.root = x.node.root;
			}
			return *this;
		}

		reference_type operator *(){
			return (node.root->value);
		}

		pointer_type operator ->(){
			return (&(operator*()));
		}


		bool operator ==(const self& x){
			return (node.NIL == x.node.NIL && node.root == x.node.root);
		}

		bool operator !=(const self& x){
			return !(*this == x);
		}

		self& operator --(){
			//当当前节点的root和NIL都指向NIL的时候的情况是否抛出异常有待检验
			RBtreeNodePtr tmp = NULL;
			if (node.root->right != node.NIL){// judge it is right child

				tmp = node.root->right;
				while (tmp->left != node.NIL)
					tmp = tmp->left;
				node.root = tmp;
			}
			else{ //it is not tight child
				tmp = node.root;
				while (tmp->parent != node.NIL && tmp->parent->right == tmp)  // tmp->parent != node.NIL  this condition must be think out
					tmp = tmp->parent;
				node.root = tmp->parent;
			}
			return *this;
		}

		self operator ++(int){
			self tmp(*this);
			++*this;
			return tmp;
		}

		self& operator ++(){
			if (node.NIL == node.root){
				node.root = node.NIL->right;
			}
			else{
				RBtreeNodePtr tmp = NULL;
				if (node.root->left != node.NIL){
					tmp = node.root->left;
					while (tmp->right != node.NIL)
						tmp = tmp->right;
					node.root = tmp;
				}
				else{
					tmp = node.root;
					while (tmp->parent != node.NIL && tmp->parent->left == tmp)
						tmp = tmp->parent;
					node.root = tmp->parent;
				}
			}
			return *this;
		}

		self operator--(int){
			self tmp(*this);
			--*this;
			return tmp;
		}


		~rbtree_reverse_iterator(){
		}

		void _SetNode(const RBtree& rb){
			node.NIL = rb.NIL;
			node.root = rb.root;
		}

		void _SetNode_Cur(RBtreeNodePtr ptr){
			node.root = ptr;
		}
		void _SetNode_NIL(RBtreeNodePtr ptr){
			node.NIL = ptr;
		}

		RBtreePtr _GetNode() const{
			return &node;
		}

		RBtreeNodePtr _GetNode_Cur() const{
			return node.root;
		}

		RBtreeNodePtr _GetNode_NIL() const{
			return node.NIL;
		}

		void swap(self& x){
			if (this != &x){
				CJH::swap(node.NIL, x.node.NIL);
				CJH::swap(node.root, x.node.root);
			}
		}
	private:
		RBtree node;

	};
	
    template<class Key, class _Ty, class Compare = CJH::less<Key>, class Alloc = rbtreenode_allocator<Key, _Ty> >
	class rbtree_base{
	public:
		typedef _Ty value_type;
		typedef Key key_type;
		typedef Color color_type;
		typedef value_type* pointer_type;
		typedef value_type& reference_type;
		typedef size_t size_type;
		typedef typename Alloc::different_type different_type;

		typedef _RBtree<Key, _Ty> RBtree;
		typedef typename RBtree::RBtreeNode RBtreeNode;
		typedef typename RBtree::RBtreeNodePtr RBtreeNodePtr;
		typedef rbtree_base<Key, _Ty, Compare, Alloc> self;


		typedef rbtree_iterator<Key, _Ty, RBtreeNode, bidirectinal_iterator_tag> iterator;
		typedef rbtree_reverse_iterator<Key, _Ty, RBtreeNode, bidirectinal_iterator_tag> reverse_iterator;
	protected:
		void initRBtree(RBtree& rbtree){
			try{
				rbtree.NIL = Alloc::allocate();
			}
			catch (...){
				Alloc::deallocate(rbtree.NIL);
				throw;
			}
			CJH::construct(CJH::addressof((rbtree.NIL->key)), key_type());
			CJH::construct(CJH::addressof((rbtree.NIL->value)), value_type());
			rbtree.NIL->left = rbtree.NIL;
			rbtree.NIL->right = rbtree.NIL;
			rbtree.NIL->parent = rbtree.NIL;
			rbtree.NIL->color = black;
			rbtree.root = rbtree.NIL;
		}

		void destroyRBtree(RBtree& rbtree){
			CJH::destroy(CJH::addressof(rbtree.NIL->key));
			CJH::destroy(CJH::addressof(rbtree.NIL->value));
			Alloc::deallocate(rbtree.NIL);
		}

		RBtreeNodePtr alloc_node(){
			return Alloc::allocate();
		}

		void free_node(RBtreeNodePtr ptr){
			Alloc::deallocate(ptr);
		}
		RBtreeNodePtr create_node(const key_type& key, const value_type& value){
			RBtreeNodePtr ptr = NULL;
			ptr = alloc_node();  // ******************************** 这里设不设置抛出异常 有进一步考虑
			try{
				CJH::construct(CJH::addressof(ptr->key), key);
			}
			catch (...){
				free_node(ptr);
				throw;
			}

			try{
				CJH::construct(CJH::addressof(ptr->value), value);
			}
			catch (...){
				CJH::destroy(CJH::addressof(ptr->key));
				free_node(ptr);
				throw;
			}
			ptr->color = red;
			ptr->left = rbtree.NIL;
			ptr->right = rbtree.NIL;
			ptr->parent = rbtree.NIL;
			return ptr;
		}

		RBtreeNodePtr clone_node(const RBtreeNodePtr& x){
			RBtreeNodePtr tmp = create_node(x->key, x->value);
			tmp->color = x->color;
			tmp->left = x->left;
			tmp->right = x->right;
			tmp->parent = x->parent;
			return tmp;
		}

		void destroy_node(RBtreeNodePtr x){

			CJH::destroy(CJH::addressof(x->key));
			CJH::destroy(CJH::addressof(x->value));
			free_node(x);
		}

		RBtreeNodePtr left(const RBtreeNodePtr x){
			return x->left;
		}

		RBtreeNodePtr leftmost(){
			return rbtree.NIL->left;
		}

		RBtreeNodePtr right(const RBtreeNodePtr x){
			return x->right;
		}

		RBtreeNodePtr rightmost(){
			return rbtree.NIL->right;
		}

		RBtreeNodePtr parent(const RBtreeNodePtr x){
			return x->parent;
		}

		RBtreeNodePtr NIL(){
			return rbtree.NIL;
		}

		key_type getkey(const RBtreeNodePtr x){
			return x->key;
		}

		reference_type value(const RBtreeNodePtr x){
			return (reference_type)x->value;
		}

		color_type& color(const RBtreeNodePtr x){
			return (color_type&)x->color;
		}

		RBtreeNodePtr root(){
			return rbtree.root;
		}
		

		//此地方有大问题， 关于异常安全的
		RBtreeNodePtr Clone(const RBtreeNodePtr x, const RBtreeNodePtr NIL){
			if (x == NIL){
				return rbtree.NIL;
			}
			RBtreeNodePtr node = NULL, left, right;
			bool isThrow = false;
			node = create_node(x->key, x->value);
			node->color = x->color;
			left = Clone(x->left, NIL);
			right = Clone(x->right, NIL);
			node->left = left;
			node->right = right;
			if (left != rbtree.NIL){
				left->parent = node;
			}
			if (right != rbtree.NIL){
				right->parent = node;
			}
			return node;
		}
	public:
		rbtree_base() :rbtree(), Mycount(0), key_compare(){
			try{
				initRBtree(rbtree);
			}
			catch (...){
				destroyRBtree(rbtree);
				throw;
			}
		}

		rbtree_base(const Compare& cmp) :key_compare(cmp), rbtree(), Mycount(0){
			try{
				initRBtree(rbtree);
			}
			catch (...){
				destroyRBtree(rbtree);
				throw;
			}
		}
		rbtree_base(const self& x) :rbtree(x.rbtree), Mycount(0), key_compare(x.key_compare){   //此构造函数存在异常安全
			try{
				rbtree.NIL = NULL;
				rbtree.root = NULL;
				initRBtree(rbtree);
			}
			catch (...){
				destroyRBtree(rbtree);
				throw;
			}

			try{
				rbtree.root = Clone(x.rbtree.root, x.rbtree.NIL);
			}
			catch (...){
				destroyRBtree(rbtree);
				throw;
			}
			//确定首尾元素的水位
			RBtreeNodePtr tmp = rbtree.root;
			while (tmp->left != rbtree.NIL)
				tmp = tmp->left;
			rbtree.NIL->left = tmp;

			tmp = rbtree.root;
			while (tmp->right != rbtree.NIL)
				tmp = tmp->right;
			Mycount = x.Mycount;

			rbtree.NIL->right = tmp;
		}

		explicit rbtree_base(const RBtree& value) :rbtree(value){

		}

		self& operator=(const self& x){
			if (this != &x){
				self tmp(x);
				this->swap(tmp);
			}
			return *this;
		}

		iterator begin(){
			return iterator(leftmost(), NIL());
		}

		reverse_iterator rbegin(){
			return reverse_iterator(rightmost(), NIL());
		}


		iterator end(){
			return iterator(NIL(), NIL());
		}

		reverse_iterator rend(){
			return reverse_iterator(NIL(), NIL());
		}
		bool empty(){
			return Mycount == 0;
		}

		size_type size() const{
			return Mycount;
		}

		size_type max_size() const{
			return size_type(-1);
		}

		iterator find(const key_type& key){
			RBtreeNodePtr tmp = root();
			RBtreeNodePtr ptr = NIL();
			while (tmp != NIL()){
				if (getkey(tmp) == key){
					ptr = tmp;
					break;
				}
				else if (key_compare(key, (getkey(tmp)))){
					tmp = left(tmp);
				}
				else{
					tmp = right(tmp);
				}
			}
			return iterator(ptr, NIL());
		}
		Compare key_comp() const{
			return key_compare;
		}

		//这两个函数算法理解有问题
	/*	iterator lower_bound(const key_type& key){
			RBtreeNodePtr tmp = rbtree.root;
			RBtreeNodePtr ptr = NIL();
			while (tmp != NIL()){
				if (getkey(tmp) == key){
					ptr = tmp;
					tmp = left(tmp);
				}
				else if (key_compare(key, getkey(tmp))){
					tmp = left(tmp);
				}
				else{
					tmp = right(tmp);
				}
			}

			return iterator(ptr, NIL());
		}

		iterator upper_bound(const key_type& key){
			RBtreeNodePtr tmp = rbtree.root;
			RBtreeNodePtr ptr = NIL();
			while (tmp != NIL()){
				if (getkey(tmp) == key){
					ptr = tmp;
					tmp = right(tmp);
				}
				else if (key_compare(key, getkey(tmp))){
					tmp = left(tmp);
				}
				else{
					tmp = right(tmp);
				}
			}

			return iterator(ptr, NIL());
		}*/

		size_type count(const key_type& key){
			iterator first = lower_bound(key);
			iterator last = upper_bound(key);
			if (first._GetNode_Cur() == NIL() || last._GetNode_Cur() == NIL()) return 0;
			else{
				size_type count = 1;
				while (first != last)
					++count;
				return count;
			}
		}
		void clear(){
			erase(begin(), end());
		}
		~rbtree_base(){
			clear();
			destroyRBtree(rbtree);
		}

		void swap(self& x){
			if (this != &x){
				CJH::swap(rbtree.NIL, x.rbtree.NIL);
				CJH::swap(rbtree.root, x.rbtree.root);
				CJH::swap(Mycount, x.Mycount);
				CJH::swap(key_compare, x.key_compare);
			}
			
		}
	public:

		iterator insert_unique(const CJH::pair<key_type, value_type>& pair){
			RBtreeNodePtr ptr = create_node(pair.first, pair.second);
			if (!rbtreeInsert_unique(&rbtree, ptr)){
				destroy_node(ptr);
				return end();
			}
			return iterator(ptr, NIL());
		}
		template<class InputIter>
		iterator insert_unique(InputIter first, InputIter last){
			typedef typename CJH::iterator_traits<InputIter>::value_type value_type;
			RBtreeNodePtr ptr = NIL();
			while (first != last){
				RBtreeNodePtr ptr = create_node((*first), *first);
				if (!rbtreeInsert_unique(&rbtree, ptr)){
					destroy_node(ptr);
					return end();
				}
				++first;
			}
			return iterator(ptr, NIL());
		}

		template<class InputIter>
		iterator insert_unique_tree(InputIter first, InputIter last){
			typedef typename CJH::iterator_traits<InputIter>::value_type value_type;
			RBtreeNodePtr ptr = NIL();
			while (first != last){
				RBtreeNodePtr ptr = create_node((*first).first, (*first).second);
				if (!rbtreeInsert_unique(&rbtree, ptr)){
					destroy_node(ptr);
					return end();
				}
				++first;
			}
			return iterator(ptr, NIL());
		}

		template<class InputIter>
		iterator insert_equal(InputIter first, InputIter last){  //此代码仅仅只针对multiset
			typedef typename CJH::iterator_traits<InputIter>::value_type value_type;
			RBtreeNodePtr ptr = NIL();
			while (first != last){
				RBtreeNodePtr ptr = create_node(*first, *first);
				if (!rbtreeInsert_equal(&rbtree, ptr)){
					destroy_node(ptr);
					return end();
				}
				++first;
			}
			return iterator(ptr, NIL());
		}

		template<class InputIter>
		iterator insert_equal_tree(InputIter first, InputIter last){
			typedef typename CJH::iterator_traits<InputIter>::value_type value_type;
			RBtreeNodePtr ptr = NIL();
			while (first != last){
				RBtreeNodePtr ptr = create_node((*first).first, (*first).second);
				if (!rbtreeInsert_equal(&rbtree, ptr)){
					destroy_node(ptr);
					return end();
				}
				++first;
			}
			return iterator(ptr, NIL());
		}

		iterator insert_equal(const CJH::pair<key_type, value_type>& pair){

			RBtreeNodePtr ptr = create_node(pair.first, pair.second);

			rbtreeInsert_equal(&rbtree, ptr);
			return iterator(ptr, NIL());
		}

		iterator erase(const key_type& key){
			RBtreeNodePtr tmp = rbtree.root;
			while (tmp != NIL() && getkey(tmp) != key){
				if (key_compare(key , getkey(tmp)))
					tmp = left(tmp);
				else
					tmp = right(tmp);
			}
			if (tmp != NIL()){
				iterator it(tmp, NIL());
				++it;
				rbtreeDelete(&rbtree, tmp);
				return it;
			}
			else {
				return end();
			}
		}

		iterator erase(iterator position){
			if (position == end()) return end();
			iterator tmp(position);
			++tmp;
			rbtreeDelete(&rbtree, position._GetNode_Cur());
			return tmp;
		}

		iterator erase(iterator first, iterator last){
			for (first; first != last;)
				erase(first++);
			return last;
		}

		void travelor(){
			_travelor(rbtree.root, 0);
		}
		void inOrder(){
			inOrder(root());
		}
		void inOrder(RBtreeNodePtr root){
			if (root == NIL()) return;
			inOrder(root->left);
			//cout << root->key << "\n";
			inOrder(root->right);
		}
	private:
		RBtree rbtree;
		size_type Mycount;
		Compare key_compare;


		void _travelor(RBtreeNodePtr root, int counter){
			if (root == rbtree.NIL){
		//		cout << "(" << counter << ")";
				return;
			}
			if (root->color == black)
				++counter;
		//	printf("%d ", root->key);
			_travelor(root->left, counter);
			_travelor(root->right, counter);
		}

		void leftRotate(RBtree *T, RBtreeNodePtr x)
		{  /**
		   * left totate
		   */
			RBtreeNodePtr y;
			y = x->right;

			/**
			* jugde y left child is NIl?
			*/
			x->right = y->left;  // Node x right child is Node y's left 
			if (y->left != T->NIL){
				x->right->parent = x; 	// 等效于 y->left->parent = x;
			}
			/**
			* link x parents' node as y parents' node
			*/
			y->parent = x->parent;


			/**
			* judge x's parent is NIL?
			*/
			if (x->parent == T->NIL){
				T->root = y;
			}
			else if (x == x->parent->left){
				/**
				* judge x is x parents' left child or right child
				*/
				x->parent->left = y;
			}
			else{
				x->parent->right = y;
			}

			/**
			* link x as y left child
			*/

			y->left = x;
			/**
			* link y as x parent
			*/

			x->parent = y;

		}
		void rightRotate(RBtree *T, RBtreeNodePtr y)
		{
			/**
			* right totate
			*/
			RBtreeNodePtr x = y->left;
			y->left = x->right;

			/**
			* jugde x right child is NIl?
			*/
			if (x->right != T->NIL){
				x->right->parent = y;	//y->left->parent = y;
			}

			/**
			* link y parents' node as x parents' node
			*/
			x->parent = y->parent;
			if (y->parent == T->NIL){
				T->root = x;
			}
			else if (y->parent->left == y){
				/**
				* judge y is y parents' left child or right child
				*/
				y->parent->left = x;
			}
			else{
				y->parent->right = x;
			}
			/**
			* link y as x right child
			*/
			x->right = y;

			/**
			* link x as y parent
			*/
			y->parent = x;
		}

		bool rbtreeInsert_unique(RBtree *T, RBtreeNodePtr node)
		{
			RBtreeNodePtr x, y;
			y = T->NIL;
			x = T->root;
			while (x != T->NIL){
				y = x;
				if (key_compare(node->key, x->key)){
					x = x->left;
				}
				else if (CJH::equal<key_type>()(node->key, x->key)){
					return false;
				}
				else{
					x = x->right;
				}
			}

			node->parent = y;
			if (y == T->NIL){  //  This is root Node. 
				T->root = node;
				T->NIL->left = node;
				T->NIL->right = node;
				/*node->color = black;
				return;*/
			}
			else if (key_compare(node->key, y->key)){
				y->left = node;
				if (y == T->NIL->left)
					T->NIL->left = node;
			}
			else if (CJH::equal<key_type>()(node->key, y->key)){
				return false;
			}
			else{
				y->right = node;
				if (y == T->NIL->right)
					T->NIL->right = node;
			}

			node->left = T->NIL;
			node->right = T->NIL;
			node->color = red;
			rbtreeInsertFixup(T, node);
			++Mycount;
			return true;
		}

		bool rbtreeInsert_equal(RBtree *T, RBtreeNodePtr node)
		{
			RBtreeNodePtr x, y;
			y = T->NIL;
			x = T->root;
			while (x != T->NIL){
				y = x;
				if (key_compare(node->key, x->key)){  //node->key < x->key  key_compare(node->key, x->key)
					x = x->left;
				}
				else {
					x = x->right;
				}
			}

			node->parent = y;
			if (y == T->NIL){  //  This is root Node. 
				T->root = node;
				T->NIL->left = node;
				T->NIL->right = node;
				/*node->color = black;
				return;*/
			}
			else if (key_compare(node->key, y->key)){  //当时在写代码的时候因为这个地方吧key_compare(node->key, y->key) 学成了key_compare(node->key, x->key)重而导致浪费了一整天的时间来检查错误
				y->left = node;
				if (y == T->NIL->left)
					T->NIL->left = node;
			}
			else{
				y->right = node;
				if (y == T->NIL->right)
					T->NIL->right = node;
			}
			++Mycount;
			node->left = T->NIL;
			node->right = T->NIL;
			node->color = red;
			return rbtreeInsertFixup(T, node);
		}
		 
		bool rbtreeInsertFixup(RBtree *T, RBtreeNodePtr node)
		{
			RBtreeNodePtr y;
			while (node->parent->color == red){  // all solution that parents' node is red

				/**
				* if the parents node is the grandparents left node, it decided to execute the rightRotate at last.
				*/
				if (node->parent == node->parent->parent->left){// Its parent is  the grandparents left node
					y = node->parent->parent->right;
					if (y->color == red){ //case 1: Its uncle's node is red
						node->parent->color = black;
						y->color = black;
						node->parent->parent->color = red;
						node = node->parent->parent;
						continue;
					}

					/**
					* The following solution is that uncle's node is black
					*/
					if (node == node->parent->right){ // case 2:Its uncle's node is black, it is the its parents right node
						node = node->parent;
						leftRotate(T, node);
					}
					// case 3:Its uncle's node is black,  it is the parents left node
					node->parent->color = black;
					node->parent->parent->color = red;
					rightRotate(T, node->parent->parent);
				}

				/**
				* if the parents node is the grandparents right node, it decided to execute the leftRotate at last.
				*/
				else{ // same as then clause with "right" and "left" exchanged.
					y = node->parent->parent->left;
					if (y != T->NIL && y->color == red){ // //case 1: Its uncle's node is red, 
						node->parent->color = black;
						y->color = black;
						node->parent->parent->color = red;
						node = node->parent->parent;
						continue;
					}

					if (node == node->parent->left){ //case 2:Its uncle's node is black, it is the its parents left node
						node = node->parent;
						rightRotate(T, node);
					}
					// case 3:Its uncle's node is black,  it is the parents right node
					node->parent->color = black;
					node->parent->parent->color = red;
					leftRotate(T, node->parent->parent);
				}
			}
			T->root->color = black;
			return true;
		}
		void rbtreeTransplant(RBtree *T, RBtreeNodePtr u, RBtreeNodePtr v)
		{
			/**
			This function link u's parent to v, but, u's left child and right child don't link to v
			*/
			if (u->parent == T->NIL){
				T->root = v;
			}
			else if (u == u->parent->left){
				u->parent->left = v;
			}
			else{
				u->parent->right = v;
			}
			v->parent = u->parent;// This will be executed in no condition, but node v may be node NIL
		}

		RBtreeNode* treeMinimum(RBtreeNodePtr node)
		{
			while (node->left != NIL())
				node = node->left;

			return node;
		}

		void rbtreeDelete(RBtree *T, RBtreeNodePtr node)
		{

			iterator first = begin(), last = end();
			--last;
			RBtreeNodePtr y = node, x;
			Color yOriginalColor = y->color;
			if (node->left == T->NIL){ //case 1:Its left child is NIL
				x = node->right;
				rbtreeTransplant(T, node, node->right);
			}
			else if (node->right == T->NIL){ // case 2:Its right child is NIL
				x = node->left;
				rbtreeTransplant(T, node, node->left);
			}
			else{ // case 3:It has two child.
				y = treeMinimum(node->right); // y's left child must is NIL
				yOriginalColor = y->color;
				x = y->right;
				if (y->parent == node)  // case 3-1: It's next node is its right child
					x->parent = y;
				else{   //case 3-2: It's next node isn't its right child
					// y's right child moves up.
					rbtreeTransplant(T, y, y->right);
					// y node represents the node which is deleted.
					y->right = node->right;
					y->right->parent = y;
				}
				// y node move up
				rbtreeTransplant(T, node, y);

				// update the its left child. 
				y->left = node->left;
				y->left->parent = y;
				y->color = node->color;
			}


			if (yOriginalColor == black){
				rbtreeDeleteFixup(T, x);
			}

			if (first._GetNode_Cur() == node){
				rbtree.NIL->left = rbtree.root;
				while (rbtree.NIL->left->left != rbtree.NIL)
					rbtree.NIL->left = rbtree.NIL->left->left;
			}
			if (last._GetNode_Cur() == node){
				rbtree.NIL->right = rbtree.root;
				while (rbtree.NIL->right->right != rbtree.NIL)
					rbtree.NIL->right = rbtree.NIL->right->right;
			}
			--Mycount;
			destroy_node(node);
		}

		void rbtreeDeleteFixup(RBtree *T, RBtreeNodePtr node)
		{
			RBtreeNodePtr w;
			while (node != T->root && node->color == black){
				if (node == node->parent->left){

					w = node->parent->right;   //  w is brothers' node
					if (w->color == red){  // case 1 its brother's node is red
						w->color = black;
						node->parent->color = red;
						leftRotate(T, node->parent);
						w = node->parent->right;
					}

					if (w->left->color == black && w->right->color == black){ //case 2: brothers' node is black and brothers' left right node is black
						w->color = red;
						node = node->parent;
					}
					else{
						if (w->right->color == black){//  case 3: brothers' node is black, its right child is black, its left child is red.
							w->left->color = black;
							w->color = red;
							rightRotate(T, w);
							w = node->parent->right;
						}
						w->color = node->parent->color; //   case 4:  brothers' node is black, and its right is red
						node->parent->color = black;
						w->right->color = black;
						leftRotate(T, node->parent);
						node = T->root;
					}
				}
				else{
					w = node->parent->left;   //  w is brothers' node
					if (w->color == red){  // case 1 its brother's node is red
						w->color = black;
						node->parent->color = red;
						rightRotate(T, node->parent);
						w = node->parent->left;
					}

					if (w->right->color == black && w->left->color == black){ //case 2: brothers' node is black and brothers' left right node is black
						w->color = red;
						node = node->parent;
					}
					else{
						if (w->left->color == black){//  case 3: brothers' node is black, its right child is black, its left child is red.
							w->right->color = black;
							w->color = red;
							leftRotate(T, w);
							w = node->parent->left;
						}
						w->color = node->parent->color; //   case 4:  brothers' node is black, and its right is red
						node->parent->color = black;
						w->left->color = black;
						rightRotate(T, node->parent);
						node = T->root;
					}
				}
			}
			node->color = black;
		}
	};

__CJH_END



#endif //__RBTREE_H__