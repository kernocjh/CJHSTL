#ifndef __AUTO_PTR_H__
#define __AUTO_PTR_H__

template<class _Ty>
class myauto_ptr{
public:
	typedef _Ty element_type;

	explicit myauto_ptr(_Ty* t = 0) :pointer(t){
	}

	myauto_ptr(myauto_ptr<_Ty>& rh) : myauto_ptr(rh.release()){

		cout << "复制构造函数\n";
	}

	~myauto_ptr(){
		if (pointer != NULL)
			delete pointer;
	}

	myauto_ptr<_Ty>& operator= (myauto_ptr<_Ty>& _rh){

		if (this != &_rh)
			reset(_rh.release());

		return *this;
	}
	_Ty& operator*(){
		return *pointer;
	}

	_Ty* operator->(){
		return pointer;
	}

	_Ty* release(){
		_Ty* tmp = pointer;
		pointer = 0;
		return tmp;
	}
	void reset(_Ty *h){
		if (h != pointer)
			delete pointer;
		pointer = h;
	}

	_Ty* get(){
		return pointer;
	}
private:
	_Ty *pointer;
};

#endif