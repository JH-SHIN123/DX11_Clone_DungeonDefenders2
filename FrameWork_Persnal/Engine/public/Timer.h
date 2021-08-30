#pragma once

#ifndef __TIMER_H__
#define __TIMER_H__

#include "Engine_Defines.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CTimer : public CBase
{
private:
	explicit CTimer();
	virtual ~CTimer();

public:
	_float		Get_TimeDelta() { return m_fTimeDelta; }

public:
	HRESULT		Ready_Timer();
	void		SetUp_TimeDelta();	// update ÇÔ¼ö

private:
	LARGE_INTEGER		m_FrameTime;
	LARGE_INTEGER		m_FixTime;
	LARGE_INTEGER		m_LastTime;

	LARGE_INTEGER		m_CpuTick;

private:
	_float				m_fTimeDelta;

public:
	static	CTimer*	 Create();
	virtual void	 Free() override;
};

END
#endif // __TIMER_H__
