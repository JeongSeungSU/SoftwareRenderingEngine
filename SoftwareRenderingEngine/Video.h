#pragma once
#include "Mathematics.h"
//#include <Windows.h>
#include <stack>
#include <list>
#include <map>
#include "Buffer.h"
#include "Texture.h"
#include "Type.h"
#include "Mesh.h"
#include "ApplyModule.h"
#include "CullingModule.h"
#include "LightModule.h"
#include "RasterizeModule.h"
#include "TransformModule.h"
#include "Material.h"

#define  DEBUGDATA
/*
������ ������������ ����
1. ���� �����̽�
2. ���� �����̽�
3. �� �����̽�
4. �ĸ� �߷�����
5. ����
6. Ŭ����
7. ����
8. ��ǥƮ
9. �����Ͷ�����
*/

namespace JssEngine
{

	struct DebugData
	{
		int		m_iMeshNum;			//mesh ����
		int		m_iTriangleNum;		//�ﰢ�� ����
		int		m_iTextureNum;		//�ؽ��� ����
		int		m_iVertexNum;		//���ؽ� ����
		bool	m_bZbuffer;			//z ���� ���ֳ� �����ֳ�

		DWORD	m_FillMode;		//���� ä��� �ִ� ���
		DWORD	m_BackfaceMode;	//�����̽� �ø� � ���³�..	
		int		m_Frame;		//���� ������...
	};


	typedef std::map<int, Texture*>		TextureMap;
	typedef std::map<int, Material*>	MaterialMap;
	//typedef TextureMap::iterator	Textureiter;

	class VideoManager
	{
	protected:

		//Module
		TransformModule			m_TransformModule;
		CullingModule			m_CullingModule;
		LightModule				m_LightModule;
		RasterizeModule			m_RasterizeModule;

		Matrix16f				m_ViewMatrix;			//ī�޶� ��Ʈ����...
		Matrix16f				m_ProjectionMatrix;		//�������� ��Ʈ����...
		Matrix16f				m_WorldMatrix;			//���� ��Ʈ����...
		Matrix16f				m_NDCtoScreenMatrix;	//NDC->Screen ��Ʈ����

		//RENDERSTATE
		ApplyModule				m_ApplyModule;


		std::stack<Matrix16f>	m_PushPopStack;			//�̰Ź��� -_-? �� Ǫ���� �����Ҷ��... ���� -0-��
		TextureMap				m_TextureMap;
		MaterialMap				m_MaterialMap;


		///////////////context////////////////////
		Vector2i				m_ScreenSize;


		/////////////NowDrawing///////////////////
		DWORD			m_NowFVF;
		VertexBuffer*	m_pNowVertexBuffer;	
		int				m_NowMaterial;

		
		//z buffering
		float**			m_ppZBuffer;


		virtual void BeginDrawBefore();
		virtual void EndDrawBefore();


		
		void FixedPipeLine(Mesh * pMesh);
		Mesh* CreateBufferToMesh(DWORD FVF,VertexBuffer* ppBuffer,PRIMITE_TYPE type,int strat, int number);

		virtual void Rasterize(Mesh* pMesh);

		//debug//
		#ifdef DEBUGDATA
		DebugData		m_DebugData;
		virtual void	PrintDebugData() = 0;
		virtual void	InitDebugData() = 0;
		#endif

	public:
		VideoManager(Vector2i	ScreenSize);
		virtual ~VideoManager(void);

		//�긮���� �˰��� ���
		virtual void LineDrawing(Vector2i v1, Vector2i v2);
	public:
		Matrix16f	SetTransform(TRANSFORM_TYPE type, const Matrix16f* mat);


		void TransformView(Vector3f Up, Vector3f dir, Vector3f right, Vector3f position);

		//�翬 �׳� ���� ����
		//��Ⱦ��,�ٴܸ�,���ܸ�,��Ÿ
		//���� z���� ���� ������ �����Ƽ� ���� ���̰� �����ϰ��� ��?  ��
		void TransformProjection(float a, float Short, float Long, float Degree );
		

		void SetFVF(DWORD fvf);
		void SetVertexBuffer(VertexBuffer* p);
		virtual void DrawPrimited(PRIMITE_TYPE type, int Start,int Number);


		void SetRenderState(RENDERSTATE state,DWORD Value);


		void CreateVertexBuffer(const unsigned int size,DWORD FVF,VertexBuffer** ppBuffer);


		//���ø� �޼��� ����(�ൿ���� ����)
		void BeginDraw();
		void EndDraw();


		//�ӽ� -_-
		virtual void FIllRECT();

		//�ؽ��� ����
		bool CreateTexture(int id, const char* FileName );
		void BindTexture(int id);
		Texture* GetTexture(int id);
	};


	//��� ���� �ڵ尡 �������� ������ ���� �����ѹ����ϰ� ���� ���ִ�
	VertexBuffer& operator*(VertexBuffer& buffer,Matrix16f& mat);
	
}