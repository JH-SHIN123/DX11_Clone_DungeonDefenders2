#include "..\public\AnimChannel.h"

CAnimChannel::CAnimChannel()
{
}

HRESULT CAnimChannel::NativeConstruct(const char * pName)
{
	strcpy(m_szName, pName);

	return S_OK;
}

HRESULT CAnimChannel::Add_KeyFrame(KEYFRAME * pKeyFrame)
{
	if (nullptr == pKeyFrame)
		return E_FAIL;

	m_KeyFrames.push_back(pKeyFrame);

	return S_OK;
}

CAnimChannel * CAnimChannel::Create(const char * pName)
{
	CAnimChannel*		pInstance = new CAnimChannel();
	if (FAILED(pInstance->NativeConstruct(pName)))
	{
		MSG_BOX("Failed to Creating Instance (CAnimChannel) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CAnimChannel::Free()
{
	for (auto& pKeyFrame : m_KeyFrames)
		Safe_Delete(pKeyFrame);

	m_KeyFrames.clear();
}

