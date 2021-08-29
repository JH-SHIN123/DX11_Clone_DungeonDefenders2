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

	for (auto& iter = m_Objects.begin(); iter != m_Objects.end();)
	{
		if (iProgress = (*iter)->Tick(TimeDelta))
		{
			switch (iProgress)
			{
			case OBJECT_DEAD:
				Safe_Release(*iter);
				iter = m_Objects.erase(iter);
				break;
			case SCENE_CHANGE:
				return iProgress;
			default:
				return iProgress;
			}
		}
		else ++iter;
	}

	return iProgress;
}

_int CLayer::Late_Tick(_float TimeDelta)
{
	_int iProgress = 0;

	for (auto& iter = m_Objects.begin(); iter != m_Objects.end();)
	{
		if (iProgress = (*iter)->Late_Tick(TimeDelta))
		{
			switch (iProgress)
			{
			case OBJECT_DEAD:
				Safe_Release(*iter);
				iter = m_Objects.erase(iter);
				break;
			case SCENE_CHANGE:
				return iProgress;
			default:
				return iProgress;
			}
		}
		else ++iter;
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

