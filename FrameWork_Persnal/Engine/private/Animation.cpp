#include "..\public\Animation.h"
#include "AnimChannel.h"


CAnimation::CAnimation()
{
}

HRESULT CAnimation::NativeConstruct(const char * pName, _double Duration, _double TickPerSecond)
{
	strcpy(m_szName, pName);
	m_Duration = Duration;
	m_TickPerSecond = TickPerSecond;

	return S_OK;
}

HRESULT CAnimation::Add_Channel(CAnimChannel * pAnimChannel)
{
	if (nullptr == pAnimChannel)
		return E_FAIL;

	m_Channels.push_back(pAnimChannel);

	return S_OK;
}

CAnimation * CAnimation::Create(const char * pName, _double Duration, _double TickPerSecond)
{
	CAnimation*		pInstance = new CAnimation();

	if (FAILED(pInstance->NativeConstruct(pName, Duration, TickPerSecond)))
	{
		MSG_BOX("Failed to Creating Instance (CAnimation) ");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CAnimation::Free()
{
	for (auto& pChannel : m_Channels)
		Safe_Release(pChannel);

	m_Channels.clear();
}
