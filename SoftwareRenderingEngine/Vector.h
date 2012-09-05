#pragma once

#include <math.h>

namespace JssEngine
{

	template<class T> 
	class Vector2
	{
	public:
		T _x,_y;
	public:
		Vector2()
		{}
		Vector2(T x,T y)
			: _x(x),_y(y)
		{}
	};

	template<class T>
	class Vector3 
	{
	public:
		T _x,_y,_z;
	public:
		Vector3()
		{}
		Vector3(T x,T y,T z)
			: _x(x),_y(y),_z(z)
		{}
	};

	template<class T>
	class Vector4
	{
	public:
		T _x,_y,_z,_w;
	public:
		Vector4()
		{}
		Vector4(T x,T y,T z,T w)
			: _x(x),_y(y),_z(z),_w(w)
		{}

	};


	typedef		Vector2<int> Vector2i;
	typedef		Vector3<int> Vector3i;
	typedef		Vector4<int> Vector4i;

	typedef		Vector2<float> Vector2f;
	typedef		Vector3<float> Vector3f;
	typedef		Vector4<float> Vector4f;

	template<class T>
	Vector3<T> VectorCross(Vector3<T> lhs, Vector3<T> rhs)
	{
		Vector3<T> tmp;
		tmp._x = lhs._y*rhs._z - lhs._z*rhs._y;
		tmp._y = lhs._z*rhs._x - lhs._x*rhs._z;
		tmp._z = lhs._x*rhs._y - lhs._y*rhs._x;
		return tmp;
	}

	template<class T>
	void Nomalize(Vector3<T>& vt)
	{
		T d = sqrt(vt._x * vt._x +vt._y* vt._y+ vt._z*vt._z);

		vt._x /=d;
		vt._y /=d;
		vt._z /=d;
	}

	template<class T>
	T Norm(Vector3<T> vt)
	{
		return sqrt(vt._x*vt._x + vt._y*vt._y + vt._z*vt._z);
	}


	
	template<class T>
	T VectorInnerProduct(Vector3<T> lhs, Vector3<T> rhs)
	{
		T tmp;
		tmp = (lhs._x * rhs._x) + (lhs._y * rhs._y) + (lhs._z * rhs._z);
		return tmp;
	}



	Vector3f operator- (Vector3f& lhs,Vector3f& rhs);

	Vector3f operator* (float lsh, Vector3f rhs);

}