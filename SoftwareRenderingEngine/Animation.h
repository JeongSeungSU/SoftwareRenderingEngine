#pragma once
#include "Container3D.h"

//[���¼�] �ִϸ��̼� ��Ʈ�� ��ü Container3D��ü�� �����ͷ� �޾Ƽ� �ִϸ��̼� �����ϴ°�ü!!
class CAnimationControl
{
public:
	float FirstFrame;			//ù������
	float LastFrame;			//������ ������
	float TickPerFrame;			//�ð��� ���ؼ� �����Ӵ� ������
	float FrameSpeed;			//������ �ӵ�!!
	float m_fFrame;				//�̰��� ���� ������

	DWORD m_NowTime;			//�̰��� �ð�/

	bool m_bAni;				//�ִϸ��̼��� �Ұ��ΰ� ���Ұ��ΰ�
	CContainer3D* m_pControlContainer;	//��Ʈ���� �����̳�
public:
	CAnimationControl(void);
	~CAnimationControl(void);

public:
	void SetContainer(CContainer3D* pContainer);	//�����̳� ���
	void SetNowTime(DWORD time);					//����ð� �ޱ�
	void AnimationUpdate();							//�ִϸ��̼� ������Ʈ!!
	void SetAnimation(bool set);					//�ִϸ��̼��� �����ϳ� ���ϳ� ó��
};

