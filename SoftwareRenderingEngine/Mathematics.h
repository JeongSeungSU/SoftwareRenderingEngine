#pragma once

#include "Matrix.h"
#include "Vector.h"
//#include <boost/math_fwd.hpp>


namespace JssEngine
{
	typedef unsigned char		BYTE;
	typedef unsigned long       DWORD;

	const float PI = 3.141592f;

	//////////////////////////한번 만들어 보실까요 ???/////////////////////
	inline bool IsZero(float a)
	{
		if( 0.00001f >= a && -0.00001f <= a)
			return true;
		else
			return false;
	}

	inline DWORD ColorA8R8G8B8(BYTE a, BYTE r,BYTE g, BYTE b)
	{
		DWORD tmp = 0;
		tmp= a<<24;
		tmp |= r<<16;
		tmp |= g<<8;
		tmp |= b;
		return tmp;
	}
	enum COLOR_ELEMENT{ A, R, G, B};
	inline BYTE A8R8G8B8ToByte(DWORD color,COLOR_ELEMENT element)
	{
		int		garage = 0;
		DWORD	compare = 0x800000;
		BYTE	tmp = 0;
		switch(element)
		{
		case A:
			garage = 0;
			break;
		case R:
			garage = 8;
			break;
		case G:
			garage = 16;
			break;
		case B:
			garage = 24;
			break;
		}

		color = color<<garage;

		for(int i =0; i< 8; i++)
		{
			if( (color & compare) == compare)
				tmp |= 0x01;
			else
				tmp &= 0xfe;

			if(i == 7)
				break;
			tmp		= tmp<<1;
			color	= color<<1;
		}

		return tmp;
	}

	Vector3f operator* (Vector3f& vt, Matrix16f& mat);
	Vector4f operator* (Vector4f& vt, Matrix16f& mat);

	template<typename T>
	void swap(T& lhs, T& rhs)
	{
		T tmp = lhs;
		lhs = rhs;
		rhs = tmp;
	}

	float Degree(int x0,int y0, int x1,int y1);


	void RotationX(Matrix16f& mat, int Degree);
	void RotationY(Matrix16f& mat, int Degree);
	void RotationZ(Matrix16f& mat, int Degree);
	void Translate(Matrix16f& mat, Vector3f position);
	void Scale(Matrix16f& mat, Vector3f scale); 

}