#pragma once

namespace JssEngine
{
	template<class T>
	class Matrix16
	{
	public:
		T _0_0,_0_1,_0_2,_0_3,
			_1_0,_1_1,_1_2,_1_3,
			_2_0,_2_1,_2_2,_2_3,
			_3_0,_3_1,_3_2,_3_3;

		T operator[](int num);
	};

	typedef Matrix16<float> Matrix16f;
	typedef Matrix16<int>	Matrix16i;

	template<class T>
	Matrix16<T> operator* (Matrix16<T>& lhs,Matrix16<T>& rhs)
	{

		Matrix16<T> tmp;

		tmp._0_0 = lhs._0_0 * rhs._0_0+ lhs._0_1 * rhs._1_0 + lhs._0_2 * rhs._2_0 + lhs._0_3 * rhs._3_0;
		tmp._0_1 = lhs._0_0 * rhs._0_1+ lhs._0_1 * rhs._1_1 + lhs._0_2 * rhs._2_1 + lhs._0_3 * rhs._3_1;
		tmp._0_2 = lhs._0_0 * rhs._0_2+ lhs._0_1 * rhs._1_2 + lhs._0_2 * rhs._2_2 + lhs._0_3 * rhs._3_2;
		tmp._0_3 = lhs._0_0 * rhs._0_3+ lhs._0_1 * rhs._1_3 + lhs._0_2 * rhs._2_3 + lhs._0_3 * rhs._3_3;

		tmp._1_0 = lhs._1_0 * rhs._0_0+ lhs._1_1 * rhs._1_0 + lhs._1_2 * rhs._2_0 + lhs._1_3 * rhs._3_0;
		tmp._1_1 = lhs._1_0 * rhs._0_1+ lhs._1_1 * rhs._1_1 + lhs._1_2 * rhs._2_1 + lhs._1_3 * rhs._3_1;
		tmp._1_2 = lhs._1_0 * rhs._0_2+ lhs._1_1 * rhs._1_2 + lhs._1_2 * rhs._2_2 + lhs._1_3 * rhs._3_2;
		tmp._1_3 = lhs._1_0 * rhs._0_3+ lhs._1_1 * rhs._1_3 + lhs._1_2 * rhs._2_3 + lhs._1_3 * rhs._3_3;

		tmp._2_0 = lhs._2_0 * rhs._0_0+ lhs._2_1 * rhs._1_0 + lhs._2_2 * rhs._2_0 + lhs._2_3 * rhs._3_0;
		tmp._2_1 = lhs._2_0 * rhs._0_1+ lhs._2_1 * rhs._1_1 + lhs._2_2 * rhs._2_1 + lhs._2_3 * rhs._3_1;
		tmp._2_2 = lhs._2_0 * rhs._0_2+ lhs._2_1 * rhs._1_2 + lhs._2_2 * rhs._2_2 + lhs._2_3 * rhs._3_2;
		tmp._2_3 = lhs._2_0 * rhs._0_3+ lhs._2_1 * rhs._1_3 + lhs._2_2 * rhs._2_3 + lhs._2_3 * rhs._3_3;

		tmp._3_0 = lhs._3_0 * rhs._0_0+ lhs._3_1 * rhs._1_0 + lhs._3_2 * rhs._2_0 + lhs._3_3 * rhs._3_0;
		tmp._3_1 = lhs._3_0 * rhs._0_1+ lhs._3_1 * rhs._1_1 + lhs._3_2 * rhs._2_1 + lhs._3_3 * rhs._3_1;
		tmp._3_2 = lhs._3_0 * rhs._0_2+ lhs._3_1 * rhs._1_2 + lhs._3_2 * rhs._2_2 + lhs._3_3 * rhs._3_2;
		tmp._3_3 = lhs._3_0 * rhs._0_3+ lhs._3_1 * rhs._1_3 + lhs._3_2 * rhs._2_3 + lhs._3_3 * rhs._3_3;

		return tmp;

	}


	
}