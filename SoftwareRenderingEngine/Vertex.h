#pragma once
#include "Mathematics.h"

namespace JssEngine
{
	struct Vertex
	{
		Vector3f m_XYZ;
		Vector3f m_Normal;
		Vector2f m_UV1;
		DWORD	 m_Color;
	};
}