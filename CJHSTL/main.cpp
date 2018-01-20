#include "vector.h"
#include "deque.h"
#include "list.h"
#include "stack.h"
#include "queue.h"
#include <iostream>
#include <list>
#include <vector>
#include <queue>
#include <string>
#include <set>
#include <deque>
#include <ctime>
#include <map>
#include <stack>
#include <iterator>
#include <unordered_set>

using namespace std;

class A{
public:
	static int a;
	A(){
		cout << "默认构造函数\n";
	}

	A& operator=(const A& a){
		cout << "Copy Assignment\n";
		return *this;
	}

	A(const A& a){
		cout << "复制构造函数\n";
	}
	~A(){
		cout << "析构函数\n";
	}
};

int A::a = 5;

#define CIRNUM 500000
int main(){
	set<int> s;
	clock_t start, end;
	start = clock(); CJH::vector<int> v;
	
//	CJH::priority_queue<int,CJH::vector<int>,CJH::greater<int> > pq;
	std::priority_queue<int> pq;
	srand(time(0));
	for (int i = 0; i < CIRNUM; ++i){
		int num = rand() % CIRNUM;
	//	cout << num << " ";
		pq.push(num);
	}
	cout << endl;
	for (int i = 0; i < CIRNUM; ++i){
	//	cout << pq.top() << "  size = " << pq.size() << endl;
		pq.pop();
	}
//	pq.pop();	I
 	end = clock();
	cout << "time = " << end - start << endl;
	cout << "size = " << pq.size() << endl;
	return 0;
}