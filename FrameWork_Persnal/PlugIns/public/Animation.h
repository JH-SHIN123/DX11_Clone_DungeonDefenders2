#pragma once

#ifndef __ANIMATION_H__

#include "Base.h"

BEGIN(Engine)
class CAnimation final : public CBase
{
	typedef struct tagFrameLerp
	{
		_float3			vSrcScale;
		_float3			vSrcPosition;
		_float4			vSrcRotation;
	}FRAME_LERP;
private:
	CAnimation();
	virtual ~CAnimation() = default;

public:
	_bool Get_IsFinishedAnimaion() { return m_IsFinished; }

public:
	void Set_LastTime(_float fLastTime);
	void Set_CurrentTime(_float fCurrentTime) { m_fCurrentTime = fCurrentTime; }
	void Set_AnimationIndex_Start(_float fAnimationStart, _float fAnimationStart_Term);

public:
	HRESULT NativeConstruct(const char* pName, _float Duration, _float TickPerSecond);
	HRESULT Add_Channel(class CAnimChannel* pAnimChannel);
	HRESULT Update_Transform(_float TimeDelta);


private:
	_bool Change_Animation_Check(_float TimeDelta);

private:
	char			m_szName[MAX_PATH] = "";
	_float			m_TickPerSecond = 0.f;

	
	_float			m_fDuration		= 0.f;		/* 애니메이션의 총 시간(길이)*/
	_float			m_fCurrentTime	= 0.f;		/* 애니메이션을 재생하고 있는 현재의 시간. */
	_float			m_fLastTime		= 0.f;		/* 애니메이션의 마지막 키프레임의 시간. */

	
	_bool			m_IsFinished = false;
	_bool			m_isEnd = false;
	_bool			m_IsChange = false;
	_float			m_fStartTime			= 0.f;
	_float			m_fStartTime_Term		= 0.f;
	_float			m_fAnimationLerpTime	= 0.f;

	FRAME_LERP*		m_pFrameLerp = { nullptr }; 
	// 보간한 현재의 내 프레임을 저장할 배열
	// 채널은 여러개지만 지금 쓰고있는 애니메이션은 한개가 아닌가

private:
	vector<class CAnimChannel*>			m_Channels;
	// 채널은 38개 
	// 그 안에 있는 애니메이션 정보는 1921개 ㅅㅂ

public:
	static CAnimation* Create(const char* pName, _float Duration, _float TickPerSecond);
	virtual void Free() override;
};
END
#define __ANIMATION_H__
#endif // !__ANIMATION_H__
