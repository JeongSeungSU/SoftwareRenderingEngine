#pragma once
#include "Mesh.h"
#include "ApplyModule.h"

namespace JssEngine
{
	class CullingModule
	{
	public:
		CullingModule(void);
		~CullingModule(void);
	public:

		void BackFaceCulling(Mesh* pMesh, Vector3f CameraDir, DWORD cullmode);

	};
}