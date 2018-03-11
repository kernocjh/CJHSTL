#include <iostream>
#include <cstdio>
#include <vector>
#include <cstdlib> 
#include <iterator>
#include <set>
#include <queue>
#include <ctime>
#include <hash_set>
#include <hash_map>
#include <fstream>
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
	char ch;
	int *p;
	A(){
		++i;
		p = new int[10000];
	//	cout << "Ĭ�Ϲ��캯��  " << i << "\n";
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
	//	cout << "���ƹ��캯��  " << i << "\n";
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
#define CIRNUM 5000
#define NUM 15000
int main(){
	string str;
	str.c_str();
	fstream filestream;
	filestream.open("deque.txt", ios::out);
	A a;
	int counter = 0;
	cout << "��һ�д����׼������ʱ��  \n�ڶ��б�׼������ʱ��  \n��һ�д���CJHSTL����ʱ��  \n�ڶ���CJHSTL����ʱ��\n\n";
	for (int k = 0; k < CIRNUM;++k){
		clock_t start, end;
		srand((unsigned int)time(0));
		std::deque<A> v;
		CJH::deque<A> l;
		start = clock();
		for (int i = 0; i < NUM; ++i){
			v.push_back(a);
		}
		end = clock();
		cout << end - start << "\t";

		/*start = clock();
		for (std::vector<A>::iterator it = v.begin(); it != v.end(); ++it)
			;
		end = clock();
		cout << end - start << "\t";*/

		start = clock();
		v.clear();
		end = clock();
		cout << end - start << "\t";

	////-------------------------------------
		start = clock();
		for (int i = 0; i < NUM; ++i){
			l.push_back(a);
		}
		end = clock();
		cout << end - start << "\t";

		/*start = clock();
		for (CJH::vector<A>::iterator it = l.begin(); it != l.end(); ++it)
			;
		end = clock();
		cout << end - start << "\t";*/

		start = clock();
		l.clear();
		end = clock();
		cout << end - start << "\t";
		cout << "\n";

		if (k % 10 == 0)filestream.flush();
	}
	if (A::i == 1){
		cout << "���ڴ�й©\t" << A::i << endl;
	}
	else{
		cout << "���ڴ�й©" << A::i << endl;
	}
	filestream.close();
 	return 0;
}