#include "Mathematics.h"
#include <iostream>
#include <math.h>

namespace JssEngine
{

	Vector3f operator* (Vector3f& vt, Matrix16f& mat)
	{
		Vector3f tmp;

		tmp._x = vt._x * mat._0_0 + vt._y * mat._1_0 + vt._z * mat._2_0 + 1.0f * mat._3_0;
		tmp._y = vt._x * mat._0_1 + vt._y * mat._1_1 + vt._z * mat._2_1 + 1.0f * mat._3_1;
		tmp._z = vt._x * mat._0_2 + vt._y * mat._1_2 + vt._z * mat._2_2 + 1.0f * mat._3_2;

		return tmp;
	}

	Vector4f operator* (Vector4f& vt, Matrix16f& mat)
	{
		Vector4f tmp;
		tmp._x = vt._x * mat._0_0 + vt._y * mat._1_0 + vt._z * mat._2_0 + vt._w * mat._3_0;
		tmp._y = vt._x * mat._0_1 + vt._y * mat._1_1 + vt._z * mat._2_1 + vt._w * mat._3_1;
		tmp._z = vt._x * mat._0_2 + vt._y * mat._1_2 + vt._z * mat._2_2 + vt._w * mat._3_2;
		tmp._w = vt._x * mat._0_3 + vt._y * mat._1_3 + vt._z * mat._2_3 + vt._w * mat._3_3;

		return tmp;
	}


	float Degree(int x0,int y0, int x1,int y1)
	{
		float degree;
		if((x1-x0) == 0)		//max와 mid의 기울기1
		{
			degree = 0.0f;
		}
		else
		{
			degree = ((float)(x1 - x0)) / ((float)(-y1 + y0));
		}
		return degree;
	}

	void Scale(Matrix16f& mat, Vector3f scale)
	{
		memset(&mat,0,sizeof(mat));
		mat._0_0 = scale._x;
		mat._1_1 = scale._y;
		mat._2_2 = scale._z;
		mat._3_3 = 1.f;

	}
	void RotationX(Matrix16f& mat, int Degree)
	{
		memset(&mat,0,sizeof(mat));
		mat._0_0 = 1.0f;
		mat._1_1 = cos((float)Degree/180 * PI);
		mat._2_1 = sin((float)Degree/180 * PI);
		mat._1_2 = mat._2_1*-1;
		mat._2_2 = mat._1_1;
		mat._3_3 = 1.0f;
	}
	void RotationY(Matrix16f& mat, int Degree)
	{
		memset(&mat,0,sizeof(mat));
		mat._0_0 = cos((float)Degree/180 * PI);
		mat._1_1 = 1.0f;
		mat._2_2 = mat._0_0;
		mat._0_2 = -sin((float)Degree/180 * PI);
		mat._2_0 = mat._0_2*-1;

		mat._3_3 = 1.0f;
	}
	void RotationZ(Matrix16f& mat, int Degree)
	{
		memset(&mat,0,sizeof(mat));
		mat._0_0 = cos((float)Degree/180 * PI);
		mat._1_0 = -sin((float)Degree/180 * PI);
		mat._0_1 = mat._1_0*-1;
		mat._1_1 = mat._0_0;
		mat._2_2 = 1.0f;

		mat._3_3 = 1.0f;
	}
	void Translate(Matrix16f& mat, Vector3f position)
	{
		memset(&mat,0,sizeof(mat));
		mat._0_0 = 1.0f;
		mat._1_1 = 1.0f;
		mat._2_2 = 1.0f;
		mat._3_3 = 1.0f;

		mat._3_0 = position._x;
		mat._3_1 = position._y;
		mat._3_2 = position._z;
	}
}
