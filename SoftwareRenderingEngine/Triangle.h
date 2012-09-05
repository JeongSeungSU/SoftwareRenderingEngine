#pragma once
#include "Vertex.h"
namespace JssEngine
{

	struct Triangle
	{
		Vertex		m_vt[3];
		Vector3f	m_TriangleNormal;
	};
}