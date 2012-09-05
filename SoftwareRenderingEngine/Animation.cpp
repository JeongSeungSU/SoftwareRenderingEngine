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

	float fElapsedTime = (GetTickCount() - m_NowTime)/1000.0f;			//�ð��� �޴´�!! �ʴ����Ƿ� 1000�� ����
	m_fFrame += fElapsedTime * FrameSpeed* TickPerFrame;				//������ ��� �ð��� * ������ �ӵ� * �� �����Ӵ� �ð�
	if(m_fFrame >= LastFrame* TickPerFrame ) m_fFrame = 0;				//�� �������� ������ �������� �ʰ��ϸ� �ʱ�ȭ!

	Matrix16 mat;
	MatrixIdentity(&mat);												//�ִϸ��̼��� ���� ��Ʈ����

	m_pControlContainer->BoneAnimation(mat, m_fFrame);					//���ִϸ��̼�
	m_pControlContainer->Animation(mat, m_fFrame);						//���ִϸ��̼� ����� ���� ���� �޽����� ������ش�!
	

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