#include <iostream>
#include <cstdio>
#include <vector>
#include <cstdlib> 
#include <iterator>
#include <set>
#include <map>
#include "vector.h"
#include "set.h"
#include "rbtree.h"
using namespace std;
class A{
public:

	static int i;
	A(){
		++i;
		cout << "默认构造函数\n";
	}
	A& operator=(const A& a){
		cout << "Copy Assignment函数\n";
		++i;
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
		cout << "复制构造函数\n";
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
		cout << "析构函数" << i << "\n";
	}
};

int A::i = 0;
int main(){

	int i;
	A ia[6];
	cout << "sdafghjgfds"<<ia[0].i << endl;
	CJH::set<A> iset(ia, ia + 5);
	cout << "size=" << iset.size() << endl;
	cout << "3count = " << iset.count(ia[3]) << endl;
	iset.insert(ia[3]);
	cout << "size=" << iset.size() << endl;
	cout << "3count = " << iset.count(ia[3]) << endl;
	iset.insert(ia[5]);
	cout << "size=" << iset.size() << endl;
	cout << "3count = " << iset.count(ia[3]) << endl;
	iset.erase(ia[1]);
	cout << "3count = " << iset.count(ia[3]) << endl;
	cout << "1count = " << iset.count(ia[1]) << endl;
	CJH::set<A>::iterator ite1 = iset.begin();
	CJH::set<A>::iterator ite2 = iset.end();
	for (; ite1 != ite2; ++ite1)
		cout << (*ite1).i << " ";
	cout << endl;
	ite1 = iset.find(ia[1]);
	if (ite1 == iset.end())
		cout << "1 don't found\n";
	iset.~set();
//	*ite1 = 9;
	return 0;
}