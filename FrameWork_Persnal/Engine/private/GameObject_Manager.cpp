#include "..\public\GameObject_Manager.h"
#include "GameObject.h"

IMPLEMENT_SINGLETON(CGameObject_Manager)

CGameObject_Manager::CGameObject_Manager()
{
}

HRESULT CGameObject_Manager::Reserve_Container(_uint iNumLevels)
{
	if (nullptr != m_pLayers)
		return E_FAIL;

	m_iNumLevels = iNumLevels;

	m_pLayers = new LAYERS[iNumLevels];
	m_pPrototypes = new PROTOTYPES[iNumLevels];

	return S_OK;
}

HRESULT CGameObject_Manager::Add_Prototype(_uint iLevelIndex, const _tchar* pPrototypeTag, CGameObject * pPrototype)
{
	if (nullptr == m_pPrototypes ||
		nullptr == pPrototype ||
		iLevelIndex >= m_iNumLevels || 
		nullptr != Find_Prototype(iLevelIndex, pPrototypeTag))
		return E_FAIL;

	m_pPrototypes[iLevelIndex].insert(PROTOTYPES::value_type(pPrototypeTag, pPrototype));

	return S_OK;
}

HRESULT CGameObject_Manager::Add_GameObject(_uint iPrototypeLevelIndex, const _tchar* pPrototypeTag, _uint iLevelIndex, const _tchar* pLayerTag, void* pArg)
{
	if (nullptr == m_pLayers || 		
		iPrototypeLevelIndex >= m_iNumLevels ||
		iLevelIndex >= m_iNumLevels)
		return E_FAIL;

	/* 현재 추가하려고하는 객체를 복제할 원형이 존재해야하낟. */
	CGameObject*	pPrototype = Find_Prototype(iPrototypeLevelIndex, pPrototypeTag);

	if (nullptr == pPrototype)
		return E_FAIL;

	/* 우너형객체를 복제하여 새객체를 만드낟.ㅏ */
	CGameObject*	pGameObject = pPrototype->Clone_GameObject(pArg);
	if (nullptr == pGameObject)
		return E_FAIL;
	
	CLayer*		pLayer = Find_Layer(iLevelIndex, pLayerTag);

	if (nullptr == pLayer)
	{
		pLayer = CLayer::Create();

		pLayer->Add_GameObject(pGameObject);

		m_pLayers[iLevelIndex].insert(LAYERS::value_type(pLayerTag, pLayer));
	}
	else
		pLayer->Add_GameObject(pGameObject);
	
	return S_OK;
}

_int CGameObject_Manager::Tick_GameObject(_double TimeDelta)
{
	_int iProgress = 0;

	for (_uint i = 0; i < m_iNumLevels; ++i)
	{
		for (auto& Pair : m_pLayers[i])
		{
			iProgress = Pair.second->Tick(TimeDelta);

			if (iProgress & 0x80000000)
				return iProgress;
		}
	}

	for (_uint i = 0; i < m_iNumLevels; ++i)
	{
		for (auto& Pair : m_pLayers[i])
		{
			iProgress = Pair.second->Late_Tick(TimeDelta);

			if (iProgress & 0x80000000)
				return iProgress;
		}
	}

	return iProgress;
}

void CGameObject_Manager::Clear(_uint iLevelIndex)
{
	for (auto& Pair : m_pLayers[iLevelIndex])
	{
		Safe_Release(Pair.second);
	}
	m_pLayers[iLevelIndex].clear();

	for (auto& Pair : m_pPrototypes[iLevelIndex])
	{
		Safe_Release(Pair.second);
	}
	m_pPrototypes[iLevelIndex].clear();
}

CGameObject * CGameObject_Manager::Find_Prototype(_uint iLevelIndex, const _tchar * pPrototypeTag)
{
	auto	iter = find_if(m_pPrototypes[iLevelIndex].begin(), m_pPrototypes[iLevelIndex].end(), CTagFinder(pPrototypeTag));

	if (iter == m_pPrototypes[iLevelIndex].end())
		return nullptr;

	return iter->second;
}

CLayer * CGameObject_Manager::Find_Layer(_uint iLevelIndex, const _tchar * pLayerTag)
{
	auto	iter  = find_if(m_pLayers[iLevelIndex].begin(), m_pLayers[iLevelIndex].end(), CTagFinder(pLayerTag));

	if (iter == m_pLayers[iLevelIndex].end())
		return nullptr;

	return iter->second;
}

void CGameObject_Manager::Free()
{
	for (_uint i = 0; i < m_iNumLevels; ++i)
	{ 
		for (auto& Pair : m_pLayers[i])		
			Safe_Release(Pair.second);		

		m_pLayers[i].clear();

		for (auto& Pair : m_pPrototypes[i])
			Safe_Release(Pair.second);

		m_pPrototypes[i].clear();
	}
	Safe_Delete_Array(m_pLayers);	
	Safe_Delete_Array(m_pPrototypes);
}
