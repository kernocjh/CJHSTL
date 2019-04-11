This is my library which is like C++ STL, it improves efficiency of releasing space.
for the vector, it improve the efficiency of pushing elements, but it sacrified the efficiency of the release of space.
for the deque, it improves hard the efficiency of pushing elements, And it reduce hard the time of releasing space.


# 接下来我将会叫你这么部署我的库文件
# 首先你必须有git 工具

# 然后调用git输入下面命令
	>>> git clone git://github.com/kernocjh/CJHSTL.git

# 在调用了git后你将会一个文件文件名叫做 CJHSTL
# 文件结构如下
	CJHSTL
		CJHSTL
		README.md

# 将库文件加入到 /usr/include/  
# 执行如下
cd CJHSTL
cp -r ./CJHSTL /usr/include/cjh

# 查看在/usr/include/cjh有没有库文件


本程序运行要在c++11以上坏境运行



# 验证
# 执行下面程序
```c++
#include <iostream>
#include <cjh/vector.h>
#include <iterator>
using namespace std;

int main(){
	CJH::vector<int> v;
	for(int i = 0; i < 100; ++i)
		v.push_back(i);
	CJH::copy(v.begin(), v.end(), ostream_iterator<int>(cout, " \n"));
	return 0;
}

```

# 编译
g++ main.cc  -std=c++11

