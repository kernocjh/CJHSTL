#include "vector.h"
#include "list.h"
#include "deque.h"
#include <ctime>
#include <iostream>
#include <list>
#include <vector>
#include <string>
#include <set>
#include <deque>
#include <map>
#include <iterator>
#include <unordered_set>

using namespace std;

#define CIRNUM 10

int main(){
	clock_t start, end;
	start = clock();
//	std::deque<int, CJH::allocator<int>, 500000> q(100000000, 1);
	std::deque<int>q(10000000, 1);
	
//	std::vector<int>v(100000000, 1);
//	CJH::deque<int, CJH::allocator<int>, 500000>::iterator it = q.begin();
	end = clock();
	int i = 0;
	//for (it; it != q.end(); ++it){
	//	++i;
	////	cout << ++i << "\t" << *it << endl;
	//}
	cout << end - start << "\t" << i;
//	CJH::copy(q.begin(), q.end(), ostream_iterator<int>(cout, " "));
	return 0;
}
