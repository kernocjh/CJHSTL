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

#define CIRNUM 51304515


int main(){
	clock_t start, end;
	start = clock();
	CJH::deque<int,CJH::allocator<int>,100>q;
	CJH::deque<int>::iterator it;
	int front = 0;
	int back = 0;
	srand(time(0));
	for (int i = 1; i <= CIRNUM; ++i){
		if (i == CIRNUM)
			i = i;
		if (i % 2 == 1){
			front++;
			q.push_back(1);
		}
		else{
			back++;
			q.push_front(1);
		}
	}

	/*for (int j = 0; j < 10; ++j){
		for (int i = 1; i <= CIRNUM; ++i){
			if (i + 1 == CIRNUM)
				i = i;
			q.push_front(i);
		}
		for (int i = 1; i <= CIRNUM; ++i){
			if (i + 1 == CIRNUM)
				i = i;
			q.push_back(i);
		}
	}*/
	end = clock();
	cout << "time = "<<end - start << endl;
	cout << "front = " << front << "\t" << "back = " << back << endl;
//	it = q.begin();
	int i = 0;
	/*for (it; it != q.end(); ++it){
		++i;
	}*/
//	cout << i << endl;
//	CJH::copy(q.begin(), q.end(), ostream_iterator<int>(cout, "\n"));
	cout << "time = "<<end - start << endl;
	return 0;
}