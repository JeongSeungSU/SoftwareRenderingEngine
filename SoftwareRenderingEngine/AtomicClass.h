#pragma once
#include <cmath>
#include <vector>
using namespace std;
//[정승수] 현재 파서제작 및 애니메이션 구현, 버텍스및 텍스쳐를 위한 기본 자료구조

typedef vector<int> ViewList;


#define NULL 0

class CMesh;
class CMaterial;
struct MeshContainer;
//벡터
struct Vector3 
{
	Vector3()
	{}
	float _x,_y,_z;
	Vector3(float x,float y,float z)
		: _x(x),_y(y),_z(z)
	{}
};
//4차원벡터
struct Vector4
{
	float x,y,z,w;
	Vector4()
	{}
	Vector4(float _x,float _y,float _z,float _w)
		: x(_x),y(_y),z(_z),w(_w)
	{}
	
};

//4*4행렬임
struct Matrix16
{
	float _0_0,_0_1,_0_2,_0_3,
		_1_0,_1_1,_1_2,_1_3,
		_2_0,_2_1,_2_2,_2_3,
		_3_0,_3_1,_3_2,_3_3;
	float& operator[] (int i)
	{
		switch (i)
		{
		case 0:
			return _0_0;
		case 1:
			return _1_0;
		case 2:
			return _2_0;
		case 3:
			return _3_0;
		case 4:
			return _0_1;
		case 5:
			return _1_1;
		case 6:
			return _2_1;
		case 7:
			return _3_1;
		case 8:
			return _0_2;
		case 9:
			return _1_2;
		case 10:
			return _2_2;
		case 11:
			return _3_2;
		case 12:
			return _0_3;
		case 13:
			return _1_3;
		case 14:
			return _2_3;
		case 15:
			return _3_3;
		}
	}
};
//사원수 계산을 위한 자료구조
class CQuaternion
{
public:
	float x,y,z,w;
public:
	CQuaternion()
	{

	}

	CQuaternion(float x, float y, float z, float w)
	{
		this->x =x;
		this->y =y;
		this->z =z;
		this->w =w;
	}
};
//[정승수] 각 벡터들을 연산자 오버로딩 
Vector3 operator+=(Vector3& lhs,Vector3 rhs);
Vector3 operator* (Vector3& vt, Matrix16& mat);
Vector3 operator- (Vector3& lhs,Vector3& rhs);
Vector3 operator+ (Vector3& lhs,Vector3& rhs);
Vector3 operator* (float lsh, Vector3 rhs);
Vector3 operator/ (Vector3 lhs,float rhs );

//[정승수] 4*4매트릭스를 오픈지엘에 맞게 변환 시켜주는 함수 
void		ParserInMatrix(float* pfloat , Matrix16 mat);

Matrix16	operator* (Matrix16& lhs,Matrix16& rhs);
void		MatrixIdentity(Matrix16* mat);					//매트릭스 초기화
float		Vec3Length(Vector3 vec);						//벡터 길이 norm 연산
int			Vec3Normalize(Vector3* pOut, Vector3* pV);		//벡터 정규화!!
Matrix16*	MatrixInverse(Matrix16* pOut,Matrix16* pMat);	//매트릭스 역행렬
Matrix16*	MatrixMultiply(Matrix16 *pout,  Matrix16 *pm1,  Matrix16 *pm2);	//매트릭스 곱
float		MatrixfDeterminant(const Matrix16 *pm);			//행렬식 구하기 역행렬을 위한 
Vector4*	Vec4Cross(Vector4 *pout, const Vector4 *pv1, const Vector4 *pv2, const Vector4 *pv3);	//4차원 벡터 크로스곱


Vector3*	Vec3TransformCoord(Vector3* pOut, Vector3 *pV,Matrix16 *pM);		//매트릭스변환에 벡터를 곱하여 변환해주는 함수
//쿼터니언
CQuaternion* QuaternionMultiply(CQuaternion * pout, const CQuaternion *pq1, const CQuaternion* pq2);		//쿼터니언 곱
CQuaternion* QuaternionSlerp(CQuaternion *pout, const CQuaternion *pq1, const CQuaternion *pq2, float t);	//구면 선형 보간
Matrix16*	MatrixRotationQuaternion(Matrix16 *pout, const CQuaternion *pq);								//매트릭스에 쿼터니언 변환넣어주기
float		QuaternionDot(const CQuaternion *pq1, const CQuaternion *pq2);									//쿼터니언 내적

bool		IsZero(float a);

//텍스쳐 좌표계
struct TEXCOORDFLOAT {
	union {
		float u;
		float uz; 
	};
	union {
		float v;
		float vz;
	};
	float oz;
};

//면
struct TRIANGLE {
	int VertexIndex[3];
	unsigned long VertexColor[3];
	Vector3 VertexNormal[3];
	Vector3 FaceNormal;
	TEXCOORDFLOAT VertexTexture[3];
	int MaterialID;
	bool bView;
	void Init();
};

//ASE파서중 로테이트 관련 애니메이션을 위한 자료구조 쿼터니언으로 구성되어있다
struct ROT_TRACK	
{
	int Index;
	CQuaternion qR;
	float x,y,z,w;

	ROT_TRACK* m_pNext;
	ROT_TRACK* m_pPrev;

	ROT_TRACK()
		:m_pPrev(NULL),
		m_pNext(NULL),
		qR(0,0,0,0)
	{}
	~ROT_TRACK()
	{
		if(m_pNext)delete m_pNext;
	}
};
//위와 같이 이동에 관한것
struct POS_TRACK
{
	int Index;
	Vector3	   Pos;
	POS_TRACK* m_pPrev;
	POS_TRACK* m_pNext;

	POS_TRACK()
		:m_pPrev(NULL),
		m_pNext(NULL)
	{}
	~POS_TRACK()
	{
		if(m_pNext)delete m_pNext;
	}
};
//크기에 관한것 거의 쓰지 않아서 버렸음
struct SCALE_TRACK
{
	int Index;
	float fX, fY, fZ;
	float aX, aY, aZ;
	float Ang;

	SCALE_TRACK* m_pNext;
	SCALE_TRACK* m_pPrev;

	SCALE_TRACK()
		:m_pPrev(NULL),
		m_pNext(NULL)
	{}

	~SCALE_TRACK()
	{
		if(m_pNext)delete m_pNext;
	}
};
//애니메이션 위의 3개를 포함한 자료
struct ANIMATION
{
	char Name[256];
	POS_TRACK* m_Pos;
	ROT_TRACK* m_Rot;
	SCALE_TRACK* m_Scale;

	ANIMATION()
		:m_Pos(NULL),
		m_Rot(NULL),
		m_Scale(NULL)
	{}
	~ANIMATION()
	{
		if(m_Pos)delete m_Pos;
		if(m_Rot)delete m_Rot;
		if(m_Scale)delete m_Scale;
	}
public:
	SCALE_TRACK* GetScaleTrack(float Frame);
	ROT_TRACK* GetRotTrack(float Frame);
	POS_TRACK* GetPosTrack(float Frame);
};

//메쉬 컨테이너 각 메쉬가 차일드 자식을 가지기 위한 자료구조
struct MeshContainer
{
	CMesh*		   pData;
	MeshContainer* m_pPrev;
	MeshContainer* m_pNext;



	MeshContainer():
	m_pPrev(NULL),
		m_pNext(NULL)
	{
	}
	~MeshContainer()
	{
		if(m_pNext)delete m_pNext;
	}
	void Add(MeshContainer* pMC)
	{
		if(m_pNext)
			m_pNext->Add(pMC);
		else
		{
			m_pNext = pMC;
			pMC->m_pPrev = this;
		}
	}
};
