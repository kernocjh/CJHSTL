#ifndef __OPRATOR_H__
#define __OPRATOR_H__
#include "config.h"
__CJH_BEGIN
	
	template<class _Ty = void>
	class less{
	public:
		bool operator() (const _Ty& t1, const _Ty& t2){
			return t1 < t2;
		}
	};

	template<class _Ty = void>
	class equal{
	public:
		bool operator() (const _Ty& t1, const _Ty& t2){
			return t1 == t2;
		}
	};

	template<class _Ty = void>
	class greater{
	public:
		bool operator() (const _Ty& t1, const _Ty& t2){
			return t1 > t2;
		}
	};
__CJH_END
#endif  //__OPRATOR_H__