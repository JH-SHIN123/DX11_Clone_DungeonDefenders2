#include "Timer.h"

USING(Engine)

Engine::CTimer::CTimer()
	: m_fTimeDelta(0.f)
{
}

Engine::CTimer::~CTimer()
{
}

HRESULT Engine::CTimer::Ready_Timer()
{
	QueryPerformanceCounter(&m_FrameTime);	// 1012
	QueryPerformanceCounter(&m_LastTime);	// 1048
	QueryPerformanceCounter(&m_FixTime);	// 1060

	QueryPerformanceFrequency(&m_CpuTick);

	return S_OK;
}

void Engine::CTimer::SetUp_TimeDelta()
{
	QueryPerformanceCounter(&m_FrameTime);			// 1400	// 1500

													// 과거 컴퓨터 기준으로 cpu가 불안정한 상황을 대비하여 만들어 넣은 예외처리, 없어도 그만
	if (m_FrameTime.QuadPart - m_FixTime.QuadPart >= m_CpuTick.QuadPart)
	{
		QueryPerformanceFrequency(&m_CpuTick);
		m_FixTime = m_FrameTime;
	}

	m_fTimeDelta = (m_FrameTime.QuadPart - m_LastTime.QuadPart) / (_float)m_CpuTick.QuadPart;

	m_LastTime = m_FrameTime;
}

Engine::CTimer* Engine::CTimer::Create()
{
	CTimer*	pInstance = new CTimer;
	if (FAILED(pInstance->Ready_Timer()))
		Safe_Release(pInstance);
	return pInstance;
}

void Engine::CTimer::Free()
{
}



