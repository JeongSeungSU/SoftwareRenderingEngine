#pragma once
#include <cmath>
#include <vector>
using namespace std;
//[���¼�] ���� �ļ����� �� �ִϸ��̼� ����, ���ؽ��� �ؽ��ĸ� ���� �⺻ �ڷᱸ��

typedef vector<int> ViewList;


#define NULL 0

class CMesh;
class CMaterial;
struct MeshContainer;
//����
struct Vector3 
{
	Vector3()
	{}
	float _x,_y,_z;
	Vector3(float x,float y,float z)
		: _x(x),_y(y),_z(z)
	{}
};
//4��������
struct Vector4
{
	float x,y,z,w;
	Vector4()
	{}
	Vector4(float _x,float _y,float _z,float _w)
		: x(_x),y(_y),z(_z),w(_w)
	{}
	
};

//4*4�����
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
//����� ����� ���� �ڷᱸ��
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
//[���¼�] �� ���͵��� ������ �����ε� 
Vector3 operator+=(Vector3& lhs,Vector3 rhs);
Vector3 operator* (Vector3& vt, Matrix16& mat);
Vector3 operator- (Vector3& lhs,Vector3& rhs);
Vector3 operator+ (Vector3& lhs,Vector3& rhs);
Vector3 operator* (float lsh, Vector3 rhs);
Vector3 operator/ (Vector3 lhs,float rhs );

//[���¼�] 4*4��Ʈ������ ���������� �°� ��ȯ �����ִ� �Լ� 
void		ParserInMatrix(float* pfloat , Matrix16 mat);

Matrix16	operator* (Matrix16& lhs,Matrix16& rhs);
void		MatrixIdentity(Matrix16* mat);					//��Ʈ���� �ʱ�ȭ
float		Vec3Length(Vector3 vec);						//���� ���� norm ����
int			Vec3Normalize(Vector3* pOut, Vector3* pV);		//���� ����ȭ!!
Matrix16*	MatrixInverse(Matrix16* pOut,Matrix16* pMat);	//��Ʈ���� �����
Matrix16*	MatrixMultiply(Matrix16 *pout,  Matrix16 *pm1,  Matrix16 *pm2);	//��Ʈ���� ��
float		MatrixfDeterminant(const Matrix16 *pm);			//��Ľ� ���ϱ� ������� ���� 
Vector4*	Vec4Cross(Vector4 *pout, const Vector4 *pv1, const Vector4 *pv2, const Vector4 *pv3);	//4���� ���� ũ�ν���


Vector3*	Vec3TransformCoord(Vector3* pOut, Vector3 *pV,Matrix16 *pM);		//��Ʈ������ȯ�� ���͸� ���Ͽ� ��ȯ���ִ� �Լ�
//���ʹϾ�
CQuaternion* QuaternionMultiply(CQuaternion * pout, const CQuaternion *pq1, const CQuaternion* pq2);		//���ʹϾ� ��
CQuaternion* QuaternionSlerp(CQuaternion *pout, const CQuaternion *pq1, const CQuaternion *pq2, float t);	//���� ���� ����
Matrix16*	MatrixRotationQuaternion(Matrix16 *pout, const CQuaternion *pq);								//��Ʈ������ ���ʹϾ� ��ȯ�־��ֱ�
float		QuaternionDot(const CQuaternion *pq1, const CQuaternion *pq2);									//���ʹϾ� ����

bool		IsZero(float a);

//�ؽ��� ��ǥ��
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

//��
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

//ASE�ļ��� ������Ʈ ���� �ִϸ��̼��� ���� �ڷᱸ�� ���ʹϾ����� �����Ǿ��ִ�
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
//���� ���� �̵��� ���Ѱ�
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
//ũ�⿡ ���Ѱ� ���� ���� �ʾƼ� ������
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
//�ִϸ��̼� ���� 3���� ������ �ڷ�
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

//�޽� �����̳� �� �޽��� ���ϵ� �ڽ��� ������ ���� �ڷᱸ��
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
