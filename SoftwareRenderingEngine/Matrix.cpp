#include "Matrix.h"


namespace JssEngine
{
	template<class T>
	T Matrix16<T>::operator[](int num)
	{
		switch(num)
		{
		case 0:
			return this->_0_0;
		case 1:
			return this->_0_1;
		case 2:
			return this->_0_2;
		case 3:
			return this->_0_3;
		case 4:
			return this->_1_0;
		case 5:
			return this->_1_1;
		case 6:
			return this->_1_2;
		case 7:
			return this->_1_3;
		case 8:
			return this->_2_0;
		case 9:
			return this->_2_1;
		case 10:
			return this->_2_2;
		case 11:
			return this->_2_3;
		case 12:
			return this->_3_0;
		case 13:
			return this->_3_1;
		case 14:
			return this->_3_2;
		case 15:
			return this->_3_3;
		}
	}
}