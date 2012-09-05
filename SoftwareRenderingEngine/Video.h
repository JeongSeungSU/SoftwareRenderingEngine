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
렌더링 파이프라인의 순서
1. 로컬 스페이스
2. 월드 스페이스
3. 뷰 스페이스
4. 후면 추려내기
5. 조명
6. 클리핑
7. 투영
8. 뷰표트
9. 래스터라이즈
*/

namespace JssEngine
{

	struct DebugData
	{
		int		m_iMeshNum;			//mesh 갯수
		int		m_iTriangleNum;		//삼각형 갯수
		int		m_iTextureNum;		//텍스쳐 갯수
		int		m_iVertexNum;		//버텍스 갯수
		bool	m_bZbuffer;			//z 버퍼 켜있나 껴져있나

		DWORD	m_FillMode;		//현재 채우고 있는 모드
		DWORD	m_BackfaceMode;	//백페이스 컬링 어떤 상태냐..	
		int		m_Frame;		//현재 프레임...
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

		Matrix16f				m_ViewMatrix;			//카메라 메트릭스...
		Matrix16f				m_ProjectionMatrix;		//프로젝션 메트릭스...
		Matrix16f				m_WorldMatrix;			//월드 매트릭스...
		Matrix16f				m_NDCtoScreenMatrix;	//NDC->Screen 매트릭스

		//RENDERSTATE
		ApplyModule				m_ApplyModule;


		std::stack<Matrix16f>	m_PushPopStack;			//이거뭐야 -_-? 아 푸쉬팝 구현할라고... 안해 -0-ㅋ
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

		//브리젠함 알고리즘 사용
		virtual void LineDrawing(Vector2i v1, Vector2i v2);
	public:
		Matrix16f	SetTransform(TRANSFORM_TYPE type, const Matrix16f* mat);


		void TransformView(Vector3f Up, Vector3f dir, Vector3f right, Vector3f position);

		//당연 그냥 원근 투영
		//종횡비,근단면,원단면,세타
		//아직 z값은 구현 안했음 귀찮아서 ㅋㅋ 깊이가 일정하겠죠 ㅋ?  ㅋ
		void TransformProjection(float a, float Short, float Long, float Degree );
		

		void SetFVF(DWORD fvf);
		void SetVertexBuffer(VertexBuffer* p);
		virtual void DrawPrimited(PRIMITE_TYPE type, int Start,int Number);


		void SetRenderState(RENDERSTATE state,DWORD Value);


		void CreateVertexBuffer(const unsigned int size,DWORD FVF,VertexBuffer** ppBuffer);


		//템플릿 메서드 패턴(행동패턴 적용)
		void BeginDraw();
		void EndDraw();


		//임시 -_-
		virtual void FIllRECT();

		//텍스쳐 관련
		bool CreateTexture(int id, const char* FileName );
		void BindTexture(int id);
		Texture* GetTexture(int id);
	};


	//흠냐 지금 코드가 여기저기 정신이 없네 정리한번싹하고 하자 쫌있다
	VertexBuffer& operator*(VertexBuffer& buffer,Matrix16f& mat);
	
}