#include "AtomicClass.h"
#include "Materialex.h"

bool IsZero(float a)
{
	if( 0.0001f >= a && -0.0001f <= a)
		return true;
	else
		return false;
}
void MatrixIdentity(Matrix16* mat)
{
	mat->_0_0 = 1.0f;
	mat->_0_1 = 0.0f;
	mat->_0_2 = 0.0f;
	mat->_0_3 = 0.0f;
	
	mat->_1_0 = 0.0f;
	mat->_1_1 = 1.0f;
	mat->_1_2 = 0.0f;
	mat->_1_3 = 0.0f;

	mat->_2_0 = 0.0f;
	mat->_2_1 = 0.0f;
	mat->_2_2 = 1.0f;
	mat->_2_3 = 0.0f;

	mat->_3_0 = 0.0f;
	mat->_3_1 = 0.0f;
	mat->_3_2 = 0.0f;
	mat->_3_3 = 1.0f;
}
Vector3 operator* (Vector3& vt, Matrix16& mat)
{
	Vector3 tmp;

	tmp._x = vt._x * mat._0_0 + vt._y * mat._0_1 + vt._z * mat._0_2 + 1.0f * mat._0_3;
	tmp._y = vt._x * mat._1_0 + vt._y * mat._1_1 + vt._z * mat._1_2 + 1.0f * mat._1_3;
	tmp._z = vt._x * mat._2_0 + vt._y * mat._2_1 + vt._z * mat._2_2 + 1.0f * mat._2_3;

	return tmp;
}
Vector3 operator- (Vector3& lhs,Vector3& rhs)
{
	Vector3 tmp;
	tmp._x = lhs._x - rhs._x;
	tmp._y = lhs._y - rhs._y;
	tmp._z = lhs._z - rhs._z;

	return tmp;
}
Vector3 operator+ (Vector3& lhs,Vector3& rhs)
{
	Vector3 tmp;
	tmp._x = lhs._x + rhs._x;
	tmp._y = lhs._y + rhs._y;
	tmp._z = lhs._z + rhs._z;

	return tmp;
}
Vector3 operator* (float lsh, Vector3 rhs)
{
	Vector3 tmp(rhs._x*lsh,rhs._y*lsh,rhs._z * lsh);
	return tmp;
}

Matrix16 operator* (Matrix16& lhs,Matrix16& rhs)
{
	Matrix16 tmp;


	tmp._0_0 = lhs._0_0 * rhs._0_0+ lhs._0_1 * rhs._1_0 + lhs._0_2 * rhs._2_0 + lhs._0_3 * rhs._3_0;
	tmp._0_1 = lhs._0_0 * rhs._0_1+ lhs._0_1 * rhs._1_1 + lhs._0_2 * rhs._2_1 + lhs._0_3 * rhs._3_1;
	tmp._0_2 = lhs._0_0 * rhs._0_2+ lhs._0_1 * rhs._1_2 + lhs._0_2 * rhs._2_2 + lhs._0_3 * rhs._3_2;
	tmp._0_3 = lhs._0_0 * rhs._0_3+ lhs._0_1 * rhs._1_3 + lhs._0_2 * rhs._2_3 + lhs._0_3 * rhs._3_3;

	tmp._1_0 = lhs._1_0 * rhs._0_0+ lhs._1_1 * rhs._1_0 + lhs._1_2 * rhs._2_0 + lhs._1_3 * rhs._3_0;
	tmp._1_1 = lhs._1_0 * rhs._0_1+ lhs._1_1 * rhs._1_1 + lhs._1_2 * rhs._2_1 + lhs._1_3 * rhs._3_1;
	tmp._1_2 = lhs._1_0 * rhs._0_2+ lhs._1_1 * rhs._1_2 + lhs._1_2 * rhs._2_2 + lhs._1_3 * rhs._3_2;
	tmp._1_3 = lhs._1_0 * rhs._0_3+ lhs._1_1 * rhs._1_3 + lhs._1_2 * rhs._2_3 + lhs._1_3 * rhs._3_3;

	tmp._2_0 = lhs._2_0 * rhs._0_0+ lhs._2_1 * rhs._1_0 + lhs._2_2 * rhs._2_0 + lhs._2_3 * rhs._3_0;
	tmp._2_1 = lhs._2_0 * rhs._0_1+ lhs._2_1 * rhs._1_1 + lhs._2_2 * rhs._2_1 + lhs._2_3 * rhs._3_1;
	tmp._2_2 = lhs._2_0 * rhs._0_2+ lhs._2_1 * rhs._1_2 + lhs._2_2 * rhs._2_2 + lhs._2_3 * rhs._3_2;
	tmp._2_3 = lhs._2_0 * rhs._0_3+ lhs._2_1 * rhs._1_3 + lhs._2_2 * rhs._2_3 + lhs._2_3 * rhs._3_3;

	tmp._3_0 = lhs._3_0 * rhs._0_0+ lhs._3_1 * rhs._1_0 + lhs._3_2 * rhs._2_0 + lhs._3_3 * rhs._3_0;
	tmp._3_1 = lhs._3_0 * rhs._0_1+ lhs._3_1 * rhs._1_1 + lhs._3_2 * rhs._2_1 + lhs._3_3 * rhs._3_1;
	tmp._3_2 = lhs._3_0 * rhs._0_2+ lhs._3_1 * rhs._1_2 + lhs._3_2 * rhs._2_2 + lhs._3_3 * rhs._3_2;
	tmp._3_3 = lhs._3_0 * rhs._0_3+ lhs._3_1 * rhs._1_3 + lhs._3_2 * rhs._2_3 + lhs._3_3 * rhs._3_3;

	return tmp;
}

//////////////////////////////////////////////////////////


POS_TRACK* ANIMATION::GetPosTrack(float Frame)
{
	POS_TRACK* Ret = NULL;
	float TFrame=0;
	float CFrame;
	POS_TRACK* CurPos = m_Pos;
	if(CurPos == NULL)return NULL;
	if(CurPos->Index > Frame)return NULL;
	while(CurPos)
	{
		CFrame = (float)CurPos->Index;
		if(Ret==NULL)
		{
			TFrame = CFrame;
			Ret = CurPos;
		}
		else
		{
			if(fabs(TFrame - Frame) > fabs(CFrame - Frame) && CFrame <= Frame)
			{
				TFrame = CFrame;
				Ret = CurPos;
			}
		}

		CurPos = CurPos->m_pNext;
	}
	return Ret;
}

ROT_TRACK* ANIMATION::GetRotTrack(float Frame)
{
	ROT_TRACK* Ret = NULL;
	float TFrame=0;
	float CFrame;
	ROT_TRACK* CurPos = m_Rot;
	if(CurPos == NULL)return NULL;
	if(CurPos->Index > Frame)return NULL;
	while(CurPos)
	{
		CFrame = (float)CurPos->Index;
		if(Ret==NULL)
		{
			TFrame = CFrame;
			Ret = CurPos;
		}
		else
		{
			if(fabs(TFrame - Frame) > fabs(CFrame - Frame) && CFrame <= Frame)
			{
				TFrame = CFrame;
				Ret = CurPos;
			}
		}

		CurPos = CurPos->m_pNext;
	}
	return Ret;
}

SCALE_TRACK* ANIMATION::GetScaleTrack(float Frame)
{
	SCALE_TRACK* Ret = NULL;
	float TFrame=0;
	float CFrame;
	SCALE_TRACK* CurPos = m_Scale;
	while(CurPos)
	{
		CFrame = (float)CurPos->Index;
		if(Ret==NULL)
		{
			TFrame = CFrame;
			Ret = CurPos;
		}
		else
		{
			if(fabs(TFrame - Frame) > fabs(CFrame - Frame))
			{
				TFrame = CFrame;
				Ret = CurPos;
			}
		}

		CurPos = CurPos->m_pNext;
	}
	return Ret;
}

void TRIANGLE::Init()
{
	for(int i=0; i<3; i++)
	{
		VertexIndex[i] = 0;
		//VertexColor[i] = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);		//버텍스 컬러를 굳이 뽑을 필요는 없지요
		VertexNormal[i] = Vector3(0,0,0);
		FaceNormal = Vector3(0,0,0);
		VertexTexture[i].u=0;
		VertexTexture[i].v=0;
	}
	MaterialID = 0;

	bView = true;
}
///////////////////////////////////////////////////////////////////////////////////////////////////

float Vec3Length(Vector3 vec)
{
	float lenth = (vec._x*vec._x) +(vec._y*vec._y) +(vec._z*vec._z) ;
	return sqrt(lenth);
}
int Vec3Normalize(Vector3* pOut, Vector3* pV)
{
	float lenth = Vec3Length((*pV));

	pOut->_x = pV->_x / lenth;
	pOut->_y = pV->_y / lenth;
	pOut->_z = pV->_z / lenth;

	lenth = Vec3Length(*pOut);
	if(IsZero(lenth))
		return 1;
	else
		return 0;
}
Matrix16* MatrixInverse(Matrix16* pOut,Matrix16* pMat)
{
	int a, i, j;
	Vector4 v, vec[3];
	FLOAT cofactor, det;

	det =MatrixfDeterminant(pMat);
	if ( !det ) return NULL;
	for (i=0; i<4; i++)
	{
		for (j=0; j<4; j++)
		{
			if (j != i )
			{
				a = j;
				if ( j > i ) a = a-1;
				vec[a].x = (*pMat)[ (j*4)+0 ];
				vec[a].y = (*pMat)[(j*4)+1];
				vec[a].z = (*pMat)[(j*4)+2];
				vec[a].w = (*pMat)[(j*4)+3];
			}
		}
		Vec4Cross(&v, &vec[0], &vec[1], &vec[2]);
		for (j=0; j<4; j++)
		{
			switch(j)
			{
			case 0: cofactor = v.x; break;
			case 1: cofactor = v.y; break;
			case 2: cofactor = v.z; break;
			case 3: cofactor = v.w; break;
			}
			(*pOut)[(j*4)+i] = pow(-1.0f, i) * cofactor / det;
		}
	}
	return pOut;
}




float MatrixfDeterminant(const Matrix16 *pm)
{
    Vector4 minor, v1, v2, v3;
    FLOAT det;

    v1.x = pm->_0_0; v1.y = pm->_1_0; v1.z = pm->_2_0; v1.w = pm->_3_0;
    v2.x = pm->_0_1; v2.y = pm->_1_1; v2.z = pm->_2_1; v2.w = pm->_3_1;
    v3.x = pm->_0_2; v3.y = pm->_1_2; v3.z = pm->_2_2; v3.w = pm->_3_2;
    Vec4Cross(&minor,&v1,&v2,&v3);
    det =  - (pm->_0_3 * minor.x + pm->_1_3 * minor.y + pm->_2_3 * minor.z + pm->_3_3 * minor.w); 
	return det;
}

Vector4* Vec4Cross(Vector4 *pout, const Vector4 *pv1, const Vector4 *pv2, const Vector4 *pv3)
{
    pout->x = pv1->y * (pv2->z * pv3->w - pv3->z * pv2->w) - pv1->z * (pv2->y * pv3->w - pv3->y * pv2->w) + pv1->w * (pv2->y * pv3->z - pv2->z *pv3->y);
    pout->y = -(pv1->x * (pv2->z * pv3->w - pv3->z * pv2->w) - pv1->z * (pv2->x * pv3->w - pv3->x * pv2->w) + pv1->w * (pv2->x * pv3->z - pv3->x * pv2->z));
    pout->z = pv1->x * (pv2->y * pv3->w - pv3->y * pv2->w) - pv1->y * (pv2->x *pv3->w - pv3->x * pv2->w) + pv1->w * (pv2->x * pv3->y - pv3->x * pv2->y);
    pout->w = -(pv1->x * (pv2->y * pv3->z - pv3->y * pv2->z) - pv1->y * (pv2->x * pv3->z - pv3->x *pv2->z) + pv1->z * (pv2->x * pv3->y - pv3->x * pv2->y));
    return pout;
}


Vector3* Vec3TransformCoord(Vector3* pOut, Vector3 *pV,Matrix16 *pM)
{
	Vector3 tmp = *pV;
	 *pOut = (tmp) * (*pM);
	 return pOut;
}
Vector3 operator/ (Vector3 lhs,float rhs )
{
	Vector3 tmp;
	tmp._x = lhs._x/rhs;
	tmp._y = lhs._y/rhs;
	tmp._z = lhs._z/rhs;

	return tmp;
}
Vector3 operator+=(Vector3& lhs,Vector3 rhs)
{
	lhs._x += rhs._x;
	lhs._y += rhs._y;
	lhs._z += rhs._z;

	return lhs;
}
CQuaternion* QuaternionMultiply(CQuaternion * pout, const CQuaternion *pq1, const CQuaternion* pq2)
{
    pout->x = pq2->w * pq1->x + pq2->x * pq1->w + pq2->y * pq1->z - pq2->z * pq1->y;
    pout->y = pq2->w * pq1->y - pq2->x * pq1->z + pq2->y * pq1->w + pq2->z * pq1->x;
    pout->z = pq2->w * pq1->z + pq2->x * pq1->y - pq2->y * pq1->x + pq2->z * pq1->w;
    pout->w = pq2->w * pq1->w - pq2->x * pq1->x - pq2->y * pq1->y - pq2->z * pq1->z;
    return pout;
}
CQuaternion* QuaternionSlerp(CQuaternion *pout, const CQuaternion *pq1, const CQuaternion *pq2, float t)
{
	float dot, epsilon;

	epsilon = 1.0f;
    dot = QuaternionDot(pq1, pq2);
    if ( dot < 0.0f) epsilon = -1.0f;
    pout->x = (1.0f - t) * pq1->x + epsilon * t * pq2->x;
    pout->y = (1.0f - t) * pq1->y + epsilon * t * pq2->y;
    pout->z = (1.0f - t) * pq1->z + epsilon * t * pq2->z;
    pout->w = (1.0f - t) * pq1->w + epsilon * t * pq2->w;
    return pout;
}
Matrix16* MatrixRotationQuaternion(Matrix16 *pout, const CQuaternion *pq)
{

	//전치가 맞나 틀리나..아우  
    MatrixIdentity(pout);
    pout->_0_0 =  1.0f - 2.0f * (pq->y * pq->y + pq->z * pq->z);
    pout->_1_0 =  2.0f * (pq->x * pq->y - pq->z * pq->w);
    pout->_2_0 =  2.0f * (pq->x * pq->z + pq->y * pq->w);
    pout->_0_1 =  2.0f * (pq->x *pq->y + pq->z * pq->w);
    pout->_1_1 =  1.0f - 2.0f * (pq->x * pq->x + pq->z * pq->z);
    pout->_2_1 =  2.0f * (pq->y *pq->z - pq->x *pq->w);
    pout->_0_2 =  2.0f * (pq->x * pq->z - pq->y * pq->w);
    pout->_1_2 =  2.0f * (pq->y *pq->z + pq->x *pq->w);
    pout->_2_2 =  1.0f - 2.0f * (pq->x * pq->x + pq->y * pq->y);


    return pout;
}
float QuaternionDot(const CQuaternion *pq1, const CQuaternion *pq2)
{
    if ( !pq1 || !pq2 ) return 0.0f;
    return (pq1->x) * (pq2->x) + (pq1->y) * (pq2->y) + (pq1->z) * (pq2->z) + (pq1->w) * (pq2->w);
}
//어떤 방향으로 해야될지 의문이긴하지만 일단은 가로
void ParserInMatrix(float* pfloat , Matrix16 mat)
{
	pfloat[0] = mat._0_0;
	pfloat[1] = mat._1_0;
	pfloat[2] = mat._2_0;
	pfloat[3] = mat._3_0;

	pfloat[4] = mat._0_1;
	pfloat[5] = mat._1_1;
	pfloat[6] = mat._2_1;
	pfloat[7] = mat._3_1;

	pfloat[8] = mat._0_2;
	pfloat[9] = mat._1_2;
	pfloat[10] = mat._2_2;
	pfloat[11] = mat._3_2;

	pfloat[12] = mat._0_3;
	pfloat[13] = mat._1_3;
	pfloat[14] = mat._2_3;
	pfloat[15] = mat._3_3;
	////////
	//pfloat[0] = mat._0_0;
	//pfloat[1] = mat._0_1;
	//pfloat[2] = mat._0_2;
	//pfloat[3] = mat._0_3;

	//pfloat[4] = mat._1_0;
	//pfloat[5] = mat._1_1;
	//pfloat[6] = mat._1_2;
	//pfloat[7] = mat._1_3;

	//pfloat[8] = mat._2_0;
	//pfloat[9] = mat._2_1;
	//pfloat[10] = mat._2_2;
	//pfloat[11] = mat._2_3;

	//pfloat[12] = mat._3_0;
	//pfloat[13] = mat._3_1;
	//pfloat[14] = mat._3_2;
	//pfloat[15] = mat._3_3;
}

Matrix16* MatrixMultiply(Matrix16 *pout,  Matrix16 *pm1,  Matrix16 *pm2) 
{
    int i,j;

	for (i=0; i<4; i++)
	{
		for (j=0; j<4; j++)
		{
			(*pout)[(i*4)+j] = (*pm1)[(i*4)+0] * (*pm2)[(0*4)+j] + (*pm1)[(i*4)+1] * (*pm2)[(1*4)+j] + (*pm1)[(i*4)+2]
					* (*pm2)[(2*4)+j] + (*pm1)[(i*4)+3]* (*pm2)[(3*4)+j]; 
		}
	}

    return pout;
}
