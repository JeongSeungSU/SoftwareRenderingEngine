#pragma once

#include "Mathematics.h"
#include "Mesh.h"
#include "ApplyModule.h"

namespace JssEngine
{
	class TransformModule
	{
	public:
		TransformModule(void);
		~TransformModule(void);

	public:
		//View
		void TransformView(Vector3f Up, Vector3f dir, Vector3f right, Vector3f position,Matrix16f& viewmat);
	
		//projection
		void TransformProjection(float a, float Short, float Long, float Degree, Matrix16f& Projection);


		//Viewport ��ȯ�� ����.. ��������...

		//NDC->Screen
		void NDCToScreenMatrix(Vector2i screensize, Matrix16f& ScreenMat);

		//Mesh*Matrix;
		//Nomalize��� ���ϴ� ��ȯ�̶�� ����� ����...
		void TransformMesh(Mesh* pMesh,Matrix16f mat,bool NomalizeCalc);
	};
}