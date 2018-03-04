#include <iostream>
#include <cstdio>
#include <vector>
#include <cstdlib> 
#include <iterator>
#include <set>

#include <ctime>
#include <hash_set>
#include <hash_map>
#include <map>
#include "vector.h"
#include "set.h"
#include "rbtree.h"
#include "map.h"
#include "list.h"
#include "deque.h"

#include "hashtable.h"
#include "hash_set.h"
#include "hash_map.h"
using namespace std;

//针对set multiset  hash_set hash_multiset的压力测试测试
class A{
public:

	static int i;
	int *p;
	A(){
		++i;
		p = new int[10000];
		cout << "默认构造函数  " << i << "\n";
	}
	A& operator=(const A& a){
		if (this != &a){
			this->~A();
			++i;
			p = new int[10000];
			//	cout << "Copy Assignment函数  " << i << "\n";
		}
		return *this;
	}

	bool operator==(const A& a) const{
		return this == &a;
	}

	bool operator !=(const A& a) const{
		return !(*this == a);
	}
	A(const A& a){
		++i;
		p = new int[10000];
		//cout << "复制构造函数  " << i << "\n";
	}
	bool operator <(const A& a){
		return true;
	}

	bool operator <(const A& a) const{
		return true;
	}
	bool operator >(const A& a){
		return true;
	}
	~A(){
		--i;
		delete[] p;
		//	cout << "析构函数" << i << "\n";
	}
};

int A::i = 0;
#define NUM 30000
void  fun(){
	CJH::list<int> v;
	CJH::Memorypool::showMemorypool();
	v.insert(v.end(), NUM,1);
	int i = 5;
	i = i + 1;
}
int main(){
	std::hash_map<int, int> hp;
	fun();
	CJH::Memorypool::showMemorypool();
 	return 0;
}