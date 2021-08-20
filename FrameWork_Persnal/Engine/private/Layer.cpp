#include "..\public\Layer.h"

CLayer::CLayer()
{
}

HRESULT CLayer::NativeConstruct()
{
	return S_OK;
}

HRESULT CLayer::Add_GameObject(CGameObject * pGameObject)
{
	if (nullptr == pGameObject)
		return E_FAIL;

	m_Objects.push_back(pGameObject);

	return S_OK;
}

_int CLayer::Tick(_float TimeDelta)
{
	_int iProgress = 0;

	for (auto& iter : m_Objects)
	{
		iProgress = iter->Tick(TimeDelta);

		if (iProgress & 0x80000000)
			return iProgress;
	}

	return iProgress;
}

_int CLayer::Late_Tick(_float TimeDelta)
{
	_int		iProgress = 0;

	for (auto& pGameObject : m_Objects)
	{
		iProgress = pGameObject->Late_Tick(TimeDelta);

		if (iProgress & 0x80000000)
			return iProgress;
	}

	return iProgress;
}

CLayer * CLayer::Create()
{
	CLayer*		pInstance = new CLayer;

	if (FAILED(pInstance->NativeConstruct()))
	{
		MSG_BOX("Failed to Creating Instance (CLayer) ");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CLayer::Free()
{
	for (auto& pGameObject : m_Objects)
		Safe_Release(pGameObject);

	m_Objects.clear();
}

