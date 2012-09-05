#include "Vector.h"



namespace JssEngine
{


	Vector3f operator- (Vector3f& lhs,Vector3f& rhs)
	{
		Vector3f tmp;
		tmp._x = lhs._x - rhs._x;
		tmp._y = lhs._y - rhs._y;
		tmp._z = lhs._z - rhs._z;

		return tmp;
	}
	Vector3f operator* (float lsh, Vector3f rhs)
	{
		Vector3f tmp(rhs._x*lsh,rhs._y*lsh,rhs._z * lsh);
		return tmp;
	}

}