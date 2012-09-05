#include "Video.h"
//임시
#include <iostream>

namespace JssEngine
{
	VideoManager::VideoManager(Vector2i	ScreenSize)
	:m_NowFVF(0),m_pNowVertexBuffer(0),m_ScreenSize(ScreenSize),m_ppZBuffer(0)
	{
		//m_ApplyModule 초기화
		m_ApplyModule.m_bCulling	= false;
		m_ApplyModule.m_bLighting	= false;
		m_ApplyModule.m_Zbuffering  = false;
		m_ApplyModule.m_TextrueApply= false;

		m_ApplyModule.m_CULLMODE	= JSS_CULL_NONE;
		//m_ApplyModule.m_Light
		m_ApplyModule.m_FillMode	= JSS_FILL_SOLID;

	}

	VideoManager::~VideoManager(void)
	{
		TextureMap::iterator itera;
		for(itera = m_TextureMap.begin(); itera != m_TextureMap.end(); itera++)
		{
			delete (itera->second);
		}
		m_TextureMap.clear();
	}

	Matrix16f	VideoManager::SetTransform(TRANSFORM_TYPE type, const Matrix16f* mat)
	{
		Matrix16f tmp;
		
		switch(type)
		{
		case JSS_TRANS_WORLD:
			tmp = m_WorldMatrix;
			m_WorldMatrix = *mat;
			break;
		case  JSS_TRANS_VIEW:
			tmp = m_ViewMatrix;
			m_ViewMatrix = *mat;
			break;
		case JSS_TRANS_PROJECTION:
			tmp = m_ProjectionMatrix;
			m_ProjectionMatrix = *mat;
			break;
		}
		return tmp;
	}

	void VideoManager::TransformView(Vector3f Up, Vector3f dir, Vector3f right, Vector3f position)
	{
		m_TransformModule.TransformView(Up,dir,right,position,m_ViewMatrix);
	}

	void VideoManager::LineDrawing(Vector2i v1, Vector2i v2)
	{
		m_RasterizeModule.LineDraw(v1,v2);
	}
	void VideoManager::TransformProjection(float a, float Short, float Long, float Degree )
	{
		m_TransformModule.TransformProjection(a,Short,Long,Degree,m_ProjectionMatrix);
	}

	void VideoManager::CreateVertexBuffer(const unsigned int size,DWORD FVF,VertexBuffer** ppBuffer)
	{
		DWORD tmp = 0x001;
		DWORD TmpFVF = FVF;

		int sizeStructure = 0;
		
		//나중에 바꾸자 일단은 if문으로...
		if((TmpFVF & JSSFVF_XYZ) == JSSFVF_XYZ)
		{
			sizeStructure += 3;
		}
		if((TmpFVF & JSSFVF_NORMAL) == JSSFVF_NORMAL)
		{
			sizeStructure += 3;
		}
		if((TmpFVF & JSSFVF_TEX0) == JSSFVF_TEX0)
		{
			sizeStructure +=2;
		}

		*ppBuffer = new VertexBuffer();

		(*ppBuffer)->InitBuffer(size,FVF);
	}

	void VideoManager::SetFVF(DWORD fvf)
	{
		m_NowFVF = fvf;
	}
	void VideoManager::SetVertexBuffer(VertexBuffer* p)
	{
		m_pNowVertexBuffer = p;
	}
	void VideoManager::DrawPrimited(PRIMITE_TYPE type, int Start,int Number)
	{
		DWORD TmpFVF = m_NowFVF & 0x001;

		switch(type)
		{
		case JSS_PRIMITE_TRIANGLE:
			
			break;
		case JSS_PRIMITE_STRIP:
			break;
		case JSS_PRIMITE_QUAD:
			break;
		}
	}
	void VideoManager::BeginDraw()
	{
		BeginDrawBefore();
	}
	void VideoManager::EndDraw()
	{
		EndDrawBefore();
	}

	void VideoManager::BeginDrawBefore()
	{
		


	}
	void VideoManager::EndDrawBefore()
	{
		
	}
	void VideoManager::FIllRECT()
	{

	}
	void VideoManager::Rasterize(Mesh* pMesh)
	{

	}
	void VideoManager::SetRenderState(RENDERSTATE state,DWORD Value)
	{
		switch(state)
		{
		case JSS_FILLMODE:
			m_ApplyModule.m_FillMode = Value;
			break;
		case  JSS_LIGHT:
			m_ApplyModule.m_bLighting = Value;
			break;
		case  JSS_CULLMODE:
			if(Value == JSS_CULL_NONE)
				m_ApplyModule.m_bCulling = false;
			else
				m_ApplyModule.m_bCulling = true;

			m_ApplyModule.m_CULLMODE = Value;
			break;
		case JSS_ZBUFFER:
			if(Value)
				m_ApplyModule.m_Zbuffering = true;
			else
				m_ApplyModule.m_Zbuffering = false;
			break;
		case  JSS_TEXTURE:
			if(Value)
				m_ApplyModule.m_TextrueApply = true;
			else
				m_ApplyModule.m_TextrueApply = false;
			break;
		}
	}
	Mesh* VideoManager::CreateBufferToMesh(DWORD FVF,VertexBuffer* ppBuffer,PRIMITE_TYPE type,int strat, int number)
	{
		void *Buffer;
		ppBuffer->Lock(0,ppBuffer->GetBufferSize(),&Buffer);
		
		Mesh* pMesh = new Mesh(Buffer,FVF,ppBuffer->GetBufferSize(),type,strat,number);

		return pMesh;

	}
	
	void VideoManager::FixedPipeLine(Mesh * pMesh)
	{
		//World변환...
		m_TransformModule.TransformMesh(pMesh,m_WorldMatrix,true);

		//View 변환
		m_TransformModule.TransformMesh(pMesh,m_ViewMatrix,true);
		
		//CULLING
		if(m_ApplyModule.m_bCulling)
			m_CullingModule.BackFaceCulling(pMesh,Vector3f(0,0,1.0f),m_ApplyModule.m_CULLMODE);

		#ifdef DEBUGDATA
		for(int i =0; i< pMesh->m_TriangleNum; i++)
		{
			if(pMesh->m_pBackFaceCulling[i] == false)
			{
				m_DebugData.m_iTriangleNum ++;
				m_DebugData.m_iVertexNum   += 3;
			}
		}
		#endif

		//미적용...
		//조명
		//if(m_ApplyModule.m_bLighting)
		//m_LightModule

		//클리핑...
		//m_CullingModule
		
		//투영 노말 계산안함 당근...
		m_TransformModule.TransformMesh(pMesh,m_ProjectionMatrix,false);

		//NDC->Screen
		m_TransformModule.NDCToScreenMatrix(m_ScreenSize,m_NDCtoScreenMatrix);
		m_TransformModule.TransformMesh(pMesh,m_NDCtoScreenMatrix,false);

		Rasterize(pMesh);
	}
	
	bool VideoManager::CreateTexture(int id, const char* FileName )
	{
		Texture* pTex = new Texture();
		pTex->setID(id);
		if(!pTex->setFilename(FileName))
			return false;
		m_TextureMap[id] = pTex;

		return true;
	}
	void VideoManager::BindTexture(int id)
	{
		m_NowMaterial = id;
	}
	Texture* VideoManager::GetTexture(int id)
	{
		Texture *pTexture;

		TextureMap::iterator iter = m_TextureMap.find(id);
		if(m_TextureMap.end() == iter)
			pTexture = NULL;
		pTexture = iter->second;

		return pTexture;
	}

}