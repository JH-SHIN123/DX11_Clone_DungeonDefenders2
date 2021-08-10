#include "..\public\Level_Manager.h"
#include "Level.h"

IMPLEMENT_SINGLETON(CLevel_Manager)

CLevel_Manager::CLevel_Manager()
{
}

HRESULT CLevel_Manager::SetUp_CurrentLevel(CLevel * pCurrentLevel)
{
	if (nullptr == pCurrentLevel)
		return E_FAIL;

	Safe_Release(m_pCurrentLevel);

	m_pCurrentLevel = pCurrentLevel;

	return S_OK;
}

_int CLevel_Manager::Tick(_double TimeDelta)
{
	if (nullptr == m_pCurrentLevel)
		return -1;

	return m_pCurrentLevel->Tick(TimeDelta);	
}

HRESULT CLevel_Manager::Render()
{
	if (nullptr == m_pCurrentLevel)
		return S_OK;

	return m_pCurrentLevel->Render();
}

void CLevel_Manager::Free()
{
	Safe_Release(m_pCurrentLevel);
}
