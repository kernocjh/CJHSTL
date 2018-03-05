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

//���set multiset  hash_set hash_multiset��ѹ�����Բ���
class A{
public:

	static int i;
	int *p;
	A(){
		++i;
		p = new int[10000];
		cout << "Ĭ�Ϲ��캯��  " << i << "\n";
	}
	A& operator=(const A& a){
		if (this != &a){
			this->~A();
			++i;
			p = new int[10000];
			//	cout << "Copy Assignment����  " << i << "\n";
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
		//cout << "���ƹ��캯��  " << i << "\n";
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
		//	cout << "��������" << i << "\n";
	}
};

int A::i = 0;
#define NUM 10000000
void  fun(){
	std::list<int> v;
	CJH::Memorypool::showMemorypool();
	for (int i = 0; i < NUM; ++i){
		v.push_back(i);
	}
	v.push_back(1);
	v.push_back(1);
	v.push_back(1);
	v.push_back(1);
	v.push_back(1);
	v.push_back(1);
	CJH::Memorypool::showMemorypool();
//	CJH::copy(v.begin(), v.end(), ostream_iterator<int>(cout, " "));
	int i = 5;
	i = i + 1;
}
int main(){
	std::hash_map<int, int> hp;
	fun();
	CJH::Memorypool::showMemorypool();
	system("pause");
 	return 0;
}