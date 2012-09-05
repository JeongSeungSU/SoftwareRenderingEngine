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


		//Viewport 변환이 없음.. 만들어야함...

		//NDC->Screen
		void NDCToScreenMatrix(Vector2i screensize, Matrix16f& ScreenMat);

		//Mesh*Matrix;
		//Nomalize계산 안하는 변환이라면 절대로 안함...
		void TransformMesh(Mesh* pMesh,Matrix16f mat,bool NomalizeCalc);
	};
}