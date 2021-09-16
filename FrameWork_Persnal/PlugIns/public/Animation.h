#pragma once

#ifndef __ANIMATION_H__

#include "Base.h"

BEGIN(Engine)
class CAnimation final : public CBase
{
private:
	CAnimation();
	virtual ~CAnimation() = default;
public:
	void Set_LastTime(_float fLastTime) { m_fLastTime = fLastTime; }
	void Set_CurrentTime(_float fCurrentTime) { m_fCurrentTime = fCurrentTime; }

public:
	HRESULT NativeConstruct(const char* pName, _float Duration, _float TickPerSecond);
	HRESULT Add_Channel(class CAnimChannel* pAnimChannel);
	HRESULT Update_Transform(_float TimeDelta, _float fEndTime, _float fNextCurrentTime);

private:
	char			m_szName[MAX_PATH] = "";
	_float			m_TickPerSecond = 0.0;

	
	_float			m_fDuration		= 0.0;		/* 애니메이션의 총 시간(길이)*/
	_float			m_fCurrentTime	= 0.0;		/* 애니메이션을 재생하고 있는 현재의 시간. */
	_float			m_fLastTime		= 0.0;		/* 애니메이션의 마지막 키프레임의 시간. */

	

	_bool			m_isEnd = false;

private:
	vector<class CAnimChannel*>			m_Channels;

public:
	static CAnimation* Create(const char* pName, _float Duration, _float TickPerSecond);
	virtual void Free() override;
};
END
#define __ANIMATION_H__
#endif // !__ANIMATION_H__
