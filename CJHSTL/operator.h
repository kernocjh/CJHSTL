#ifndef __OPRATOR_H__
#define __OPRATOR_H__
#include "config.h"
__CJH_BEGIN

	template<class T1, class T2>
	struct pair{
		typedef T1 first_type;
		typedef T2 second_type;
		typedef pair<T1, T2> self;
		T1 first;
		T2 second;
		pair() :first(T1()), second(T2()){

		}

		pair(const T1& a, const T2& b) :first(a), second(b){

		}

		pair(const self& p) :first(p.first), second(p.second) {
		
		}

		self& operator= (const self& x){
			if (this != &x){
				first = x.first;
				second = x.second;
			}
			return *this;
		}
	};

	template<class _Ty = void>
	class less{
	public:
		bool operator() (const _Ty& t1, const _Ty& t2) const{
			return t1 < t2;
		}
	};

	template<class _Ty = void>
	class equal{
	public:
		bool operator() (const _Ty& t1, const _Ty& t2) const{
			return t1 == t2;
		}
	};

	template<class _Ty = void>
	class greater{
	public:
		bool operator() (const _Ty& t1, const _Ty& t2) const{
			return t1 > t2;
		}
	};


	//template<class _Ty>
	//struct identity{
	//	typedef _Ty type;
	//	const _Ty& operator()(const _Ty& _Left) const  //主要利用隐式转化
	//	{	// apply identity operator to operand
	//		return (_Left);
	//	}

	//	template<class T = CJH::pair<_Ty, ...> >
	//	const _Ty& operator()(const T& _Left) const  //主要利用隐式转化
	//	{	// apply identity operator to operand
	//		return (_Left.first);
	//	}
	//};
__CJH_END
#endif  //__OPRATOR_H__