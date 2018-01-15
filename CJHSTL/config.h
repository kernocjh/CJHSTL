#ifndef __CONFIG_H__
#define __CONFIG_H__


#define __CJH_BEGIN namespace CJH{
#define __CJH_END  }

#include <crtdefs.h>

template<class _Arg1,
class _Arg2,
class _Result>
struct binary_function
{	// base class for binary functions
	typedef _Arg1 first_argument_type;
	typedef _Arg2 second_argument_type;
	typedef _Result result_type;
};

#endif