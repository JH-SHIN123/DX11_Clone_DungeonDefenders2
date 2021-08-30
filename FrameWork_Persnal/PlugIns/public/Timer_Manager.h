#pragma once

#ifndef __TIMER_MANAGER_H__

#include "Timer.h"

BEGIN(Engine)
class ENGINE_DLL CTimer_Manager final : public CBase
{
	DECLARE_SINGLETON(CTimer_Manager)

private:
	explicit CTimer_Manager();
	virtual ~CTimer_Manager();

public:
	_float		Get_TimeDelta(const _tchar* pTimerTag);
	void		Set_TimeDelta(const _tchar* pTimerTag);

public:
	HRESULT		Ready_Timer(const _tchar* pTimerTag);

private:
	CTimer*		Find_Timer(const _tchar* pTimerTag);
	_bool		m_IsFirst = true;

private:
	unordered_map<const _tchar*, CTimer*>		m_mapTimers; // 1프레임당 시간을 공유하는것이 아닌 사운드 매니저 처럼 채널을 설정하는것과 비슷

public:
	virtual void	Free();
};

END
#define __TIMER_MANAGER_H__
#endif // !__TIMER_MANAGER_H__
