#include "Animation.h"


CAnimationControl::CAnimationControl(void)
{
	FirstFrame = 0;
	LastFrame = 0;
	TickPerFrame = 0;
	FrameSpeed = 0;
	m_fFrame = 0;

	m_NowTime = 0;

	m_bAni = false;
	m_pControlContainer = NULL;
}
CAnimationControl::~CAnimationControl(void)
{
}
void CAnimationControl::SetNowTime(DWORD time)
{
	m_NowTime = time;
}
void CAnimationControl::AnimationUpdate()
{

	float fElapsedTime = (GetTickCount() - m_NowTime)/1000.0f;			//시간을 받는다!! 초단위므로 1000을 나눔
	m_fFrame += fElapsedTime * FrameSpeed* TickPerFrame;				//프레임 계산 시간차 * 프레임 속도 * 각 프레임당 시간
	if(m_fFrame >= LastFrame* TickPerFrame ) m_fFrame = 0;				//현 프레임이 마지막 프레임을 초과하면 초기화!

	Matrix16 mat;
	MatrixIdentity(&mat);												//애니메이션을 위한 메트릭스

	m_pControlContainer->BoneAnimation(mat, m_fFrame);					//본애니메이션
	m_pControlContainer->Animation(mat, m_fFrame);						//본애니메이션 계산후 본에 붙은 메쉬들을 계산해준다!
	

	m_NowTime = GetTickCount();											
}
void CAnimationControl::SetAnimation(bool set)
{
	m_pControlContainer->SetControl(set);
}
void CAnimationControl::SetContainer(CContainer3D* pContainer)
{
	m_pControlContainer = pContainer;
}