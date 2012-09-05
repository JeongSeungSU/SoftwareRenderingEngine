#pragma once
#include "Container3D.h"

//[정승수] 애니메이션 컨트롤 객체 Container3D객체를 포인터로 받아서 애니메이션 조정하는객체!!
class CAnimationControl
{
public:
	float FirstFrame;			//첫프래임
	float LastFrame;			//마지막 프레임
	float TickPerFrame;			//시간에 곱해서 프레임당 움직임
	float FrameSpeed;			//프레임 속도!!
	float m_fFrame;				//이것은 현재 프레임

	DWORD m_NowTime;			//이것은 시간/

	bool m_bAni;				//애니메이션을 할것인가 안할것인가
	CContainer3D* m_pControlContainer;	//컨트롤할 컨테이너
public:
	CAnimationControl(void);
	~CAnimationControl(void);

public:
	void SetContainer(CContainer3D* pContainer);	//컨테이너 등록
	void SetNowTime(DWORD time);					//현재시간 받기
	void AnimationUpdate();							//애니메이션 업데이트!!
	void SetAnimation(bool set);					//애니메이션을 동작하나 안하나 처리
};

