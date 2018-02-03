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
	A(){
		++i;
		cout << "默认构造函数  " << i << "\n";
	}
	A& operator=(const A& a){
		if (this != &a){
			this->~A();
			++i;
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
		//	cout << "析构函数" << i << "\n";
	}
};

int A::i = 0;
#define NUM 500000
int main(){
	srand(time(0));
	clock_t start, end;
	CJH::multiset<int> mp;
	A a;
	srand((unsigned int)time(0));
	CJH::vector<int> v;
	for (int i = 0; i < NUM; ++i){
		v.push_back(rand()%NUM);
	}
	start = clock();
	mp.insert(v.begin(), v.end());
	//	CJH::copy(mp.begin(), mp.end(), ostream_iterator<int>(cout, " \n"));
	cout << endl;
	end = clock();
	cout << mp.size() << endl;
	cout << "time = " << end - start << endl;
	cout << a.i << endl;
	//	mp.t.travelor();
//	mp.t.inOrder();
	//int n = mp.size();
	//for (int i = 0; i < n; ++i){
	//	mp.erase(mp.begin());
	//}
	mp.clear();
	v.clear();


	
	return 0;
}