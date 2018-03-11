#ifndef __ITERATOR_H__
#define __ITERATOR_H__

#include "config.h"

__CJH_BEGIN
	
	struct input_iterator_tag{  //���������

	};

	struct output_iterator_tag{	//���������

	};

	struct forward_iterator_tag : public input_iterator_tag{

	};

	struct bidirectinal_iterator_tag :public forward_iterator_tag{

	};

	struct randow_access_iterator_tag :public bidirectinal_iterator_tag{

	};

	//������������
	template<class Category, class _Ty, 
	class Distance = ptrdiff_t, class Pointer = _Ty*, class Reference = _Ty&>
	struct _Iterator_base{
		typedef Category iterator_category;
		typedef _Ty value_type;
		typedef Pointer pointer_type;
		typedef Reference reference_type;
		typedef Distance difference_type;
	};

	template<class _Iterator_base>  //��Ե����� ������ե֭����ȡ���е�����
	struct iterator_traits{
		typedef typename _Iterator_base::iterator_category iterator_category;
		typedef typename _Iterator_base::value_type value_type;
		typedef typename _Iterator_base::pointer_type pointer_type;
		typedef typename _Iterator_base::reference_type reference_type;
		typedef typename _Iterator_base::difference_type difference_type;
	};
	

	template<class _Ty>
	struct iterator_traits<_Ty*>{//���ԭ��ָ�룬ƫ�ػ�  ե֭������ȡ
		typedef randow_access_iterator_tag iterator_category;
		typedef _Ty value_type;
		typedef _Ty* pointer_type;
		typedef _Ty& reference_type;
		typedef ptrdiff_t difference_type;
	};

	template<class _Ty>
	struct iterator_traits<const _Ty*>{//���constԭ��ָ��(pointer-to-const)��ƫ�ػ�   ե֭������ȡ
		typedef randow_access_iterator_tag iterator_category;
		typedef _Ty value_type;
		typedef _Ty* pointer_type;
		typedef _Ty& reference_type;
		typedef ptrdiff_t difference_type;
	};


	//��ȡ������������
	template<class _Iterator_base>
	inline typename iterator_traits<_Iterator_base>::iterator_category
	iterator_category(const _Iterator_base&){
		typedef typename iterator_traits<_Iterator_base>::iterator_category category;
		return category();
	}

	template<class _Iterator_base>
	inline typename iterator_traits<_Iterator_base>::value_type*
		value_type(const _Iterator_base&){
			typedef typename iterator_traits<_Iterator_base>::value_type value_type;

			return static_cast<value_type*>(0);
		}



	template<class Category, class _Ty>
	class iterator_base :public _Iterator_base<Category, _Ty>{
	public:
		typedef _Iterator_base<Category, _Ty> _mybase;
		typedef typename _mybase::value_type value_type;
		typedef typename _mybase::pointer_type pointer_type;
		typedef typename _mybase::reference_type reference_type;
		typedef typename _mybase::difference_type difference_type;
		typedef typename _mybase::iterator_category iterator_category;
		typedef iterator_base<Category, _Ty> self;
	private:

	public:
		iterator_base(){}
		/*bool operator ==(const self& x) const;
		bool operator !=(const self& x) const;

		value_type operator *() const{ return *ptr; }
		pointer_type operator ->() const{ return ptr; }

		self& operator ++(){

		}

		self operator++ (){

		}*/
	};

__CJH_END


#endif