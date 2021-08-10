#include "..\public\Component_Manager.h"

IMPLEMENT_SINGLETON(CComponent_Manager)

CComponent_Manager::CComponent_Manager()
{
}

HRESULT CComponent_Manager::Reserve_Container(_uint iNumLevels)
{
	if (nullptr != m_pComponents)
		return E_FAIL;

	m_iNumLevels = iNumLevels;

	m_pComponents = new COMPONENTS[m_iNumLevels];

	return S_OK;
}

HRESULT CComponent_Manager::Add_Prototype(_uint iNumLevels, const _tchar * pComponentTag, CComponent * pPrototype)
{
	if (m_iNumLevels <= iNumLevels ||
		nullptr == pPrototype ||
		nullptr == m_pComponents ||
		nullptr != Find_Component(iNumLevels, pComponentTag)) // 얘는 중복값이라는 소리
		return E_FAIL;

	m_pComponents[iNumLevels].insert(COMPONENTS::value_type(pComponentTag, pPrototype));

	return S_OK;
}

CComponent * CComponent_Manager::Clone_Component(_uint iNumLevels, const _tchar * pComponentTag, void * pArg)
{
	if (m_iNumLevels <= iNumLevels ||
		nullptr == m_pComponents)
		return nullptr;

	CComponent* pComponent = Find_Component(iNumLevels, pComponentTag);

	if (nullptr == pComponent)
		return nullptr;

	return pComponent->Clone(pArg);
}

CComponent * CComponent_Manager::Find_Component(_uint iNumLevels, const _tchar * pComponentTag)
{
	auto iter = find_if(m_pComponents[iNumLevels].begin(), m_pComponents[iNumLevels].end(), CTagFinder(pComponentTag));

	if (iter == m_pComponents[iNumLevels].end())
		return nullptr;

	return iter->second;
}

void CComponent_Manager::Free()
{
	for (_uint i = 0; i < m_iNumLevels; ++i)
	{
		for (auto& Pair : m_pComponents[i])
			Safe_Release(Pair.second);
	}

	Safe_Delete_Array(m_pComponents);
}
