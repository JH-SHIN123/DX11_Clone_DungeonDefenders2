#include "Timer_Manager.h"

USING(Engine)
IMPLEMENT_SINGLETON(CTimer_Manager)

Engine::CTimer_Manager::CTimer_Manager(void)
{
}

Engine::CTimer_Manager::~CTimer_Manager(void)
{
	Free();
}

HRESULT Engine::CTimer_Manager::Ready_Timer(const _tchar* pTimerTag)
{
	CTimer*	pTimer = Find_Timer(pTimerTag);

	if (nullptr != pTimer)
		return E_FAIL;

	pTimer = CTimer::Create();
	if (nullptr == pTimer)
	{
		MSG_BOX("Failed Create Timer");
		return E_FAIL;
	}

	m_mapTimers.emplace(pTimerTag, pTimer);

	return S_OK;
}

Engine::CTimer* Engine::CTimer_Manager::Find_Timer(const _tchar* pTimerTag)
{
	// auto	iter = m_mapTimers.find(pTimerTag);

	auto	iter = find_if(m_mapTimers.begin(), m_mapTimers.end(), CTagFinder(pTimerTag));
	// 함수 포인터, 함수 객체, 람다

	if (iter == m_mapTimers.end())
		return nullptr;

	return iter->second;
}


Engine::_float	Engine::CTimer_Manager::Get_TimeDelta(const _tchar* pTimerTag)
{
	CTimer*	pTimer = Find_Timer(pTimerTag);

	if (nullptr == pTimer)
		return 0.f;

	return pTimer->Get_TimeDelta();
}
void		Engine::CTimer_Manager::Set_TimeDelta(const _tchar* pTimerTag)
{
	CTimer*	pTimer = Find_Timer(pTimerTag);

	if (nullptr == pTimer)
		return;

	pTimer->SetUp_TimeDelta();
}

void Engine::CTimer_Manager::Free(void)
{
	for_each(m_mapTimers.begin(), m_mapTimers.end(), CDeleteMap());
	m_mapTimers.clear();
}