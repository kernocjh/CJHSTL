#ifndef __CONFIG_H__
#define __CONFIG_H__


#define __CJH_BEGIN namespace CJH{
#define __CJH_END  }


#if defined(WIN32)
#include <crtdefs.h>
#else
typedef int ptrdiff_t;
#endif
__CJH_BEGIN

template<class _Arg1,
class _Arg2,
class _Result>
struct binary_function
{	// base class for binary functions
	typedef _Arg1 first_argument_type;
	typedef _Arg2 second_argument_type;
	typedef _Result result_type;
};


//用于构建hashtable

static const size_t MM_OFFSET = 8;  //表示相邻内存池桶的偏移量
static const size_t MM_HASHSIZE = 1572869;
static const size_t MM_HASHCRISIS = 10240;
static const int __stl_nun_primes = 28;  //质数数量
static const unsigned long __stl_prime_list[__stl_nun_primes] = {  //质数数组
	53, 97, 193, 389, 769,
	1543, 3079, 6151, 12289, 24593,
	49157, 98317, 196613, 393241, 786433,
	1572869, 3145739, 6291469, 12582917, 25165843,
	50331653, 100663319, 201326611, 402653189, 805306457,
	1610612741, 3221225473ul, 4294967291ul
};



//template<class _Ty>
//struct identity{
//	typedef _Ty type;
//	const _Ty& operator()(const _Ty& _Left) const  //主要利用隐式转化
//	{	// apply identity operator to operand
//		return (_Left);
//	}
//};

template<class Key>
struct hash{

};

inline size_t __stl_hash_string(const char * s){
	unsigned long h = 0;
	for (; *s; ++s){
		h = 5 * h + *s;
	}

	return size_t(h);
}

template<>
struct hash<char*>
{
	size_t operator()(const char *s) const{
		return __stl_hash_string(s);
	}
};

template<>
struct hash<const char*>
{
	size_t operator()(const char *s) const{
		return __stl_hash_string(s);
	}
};

//template<>
//struct hash<std::string>
//{
//	size_t operator()(const std::string& s) const{
//		return __stl_hash_string(s.c_str());
//	}
//};

template<>
struct hash<char>
{
	size_t operator()(char s) const{
		return s;
	}
};

template<>
struct hash<unsigned char>
{
	size_t operator()(unsigned char s) const{
		return s;
	}
};

template<>
struct hash<signed char>
{
	size_t operator()(unsigned char s) const{
		return s;
	}
};



template<>
struct hash<short>
{
	size_t operator()(short s) const{
		return s;
	}
};

template<>
struct hash<unsigned short>
{
	size_t operator()(unsigned short s) const{
		return s;
	}
};

template<>
struct hash<int>
{
	size_t operator()(int s) const{
		return s;
	}
};

template<>
struct hash<unsigned int>
{
	size_t operator()(unsigned int s) const{
		return s;
	}
};

template<>
struct hash<long>
{
	size_t operator()(long s) const{
		return s;
	}
};

template<>
struct hash<unsigned long>
{
	size_t operator()(unsigned long s) const{
		return s;
	}
};

__CJH_END
#endif
