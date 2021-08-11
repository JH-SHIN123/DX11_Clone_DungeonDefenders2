#include "..\public\GameInstance.h"

IMPLEMENT_SINGLETON(CGameInstance)

CGameInstance::CGameInstance()
	: m_pGraphic_Device(CGraphic_Device::GetInstance())
	, m_pLevel_Manager(CLevel_Manager::GetInstance())
	, m_pGameObject_Manager(CGameObject_Manager::GetInstance())
	, m_pComponent_Manager(CComponent_Manager::GetInstance())
{
	Safe_AddRef(m_pComponent_Manager);
	Safe_AddRef(m_pGameObject_Manager);
	Safe_AddRef(m_pLevel_Manager);
	Safe_AddRef(m_pGraphic_Device);
}



HRESULT CGameInstance::Initialize(HWND hWnd, CGraphic_Device::WINMODE eWinMode, _uint iSizeX, _uint iSizeY, ID3D11Device** ppDevice, ID3D11DeviceContext** ppDevice_Context)
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	if (FAILED(m_pGraphic_Device->Ready_Graphic_Device(eWinMode, hWnd, iSizeX, iSizeY, ppDevice, ppDevice_Context)))
		return E_FAIL;

	return S_OK;
}

HRESULT CGameInstance::Reserve_Container(_uint iNumLevels)
{
	if (nullptr == m_pGameObject_Manager ||
		nullptr == m_pComponent_Manager)
		return E_FAIL;

	if (FAILED(m_pComponent_Manager->Reserve_Container(iNumLevels)))
		return E_FAIL;

	if (FAILED(m_pGameObject_Manager->Reserve_Container(iNumLevels)))
		return E_FAIL;

	return S_OK;
}

_int CGameInstance::Tick(_double TimeDelta)
{
	if (nullptr == m_pLevel_Manager ||
		nullptr == m_pGameObject_Manager)
		return -1;

	if (0x80000000 & m_pGameObject_Manager->Tick_GameObject(TimeDelta))
		return -1;

	return m_pLevel_Manager->Tick(TimeDelta);
}


#pragma region Graphic_Device

HRESULT CGameInstance::Clear_Back_Buffer(_float4 vClearColor)
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	return m_pGraphic_Device->Clear_Back_Buffer(vClearColor);
}

HRESULT CGameInstance::Clear_Depth_Stencil_Buffer()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	return m_pGraphic_Device->Clear_Depth_Stencil_Buffer();
}

HRESULT CGameInstance::Present()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	return m_pGraphic_Device->Present();
}
#pragma endregion 

#pragma region Level_Manager
HRESULT CGameInstance::SetUp_CurrentLevel(CLevel * pCurrentLevel)
{
	if (nullptr == m_pLevel_Manager)
		return E_FAIL;

	return m_pLevel_Manager->SetUp_CurrentLevel(pCurrentLevel);
}



HRESULT CGameInstance::Render_Level()
{
	if (nullptr == m_pLevel_Manager)
		return E_FAIL;

	return m_pLevel_Manager->Render();
}

#pragma endregion 

#pragma region GameObject_Manager


HRESULT CGameInstance::Add_Prototype(_uint iLevelIndex, const _tchar * pPrototypeTag, CGameObject * pPrototype)
{
	if (nullptr == m_pGameObject_Manager)
		return E_FAIL;

	return m_pGameObject_Manager->Add_Prototype(iLevelIndex, pPrototypeTag, pPrototype);
}

HRESULT CGameInstance::Add_GameObject(_uint iPrototypeLevelIndex, const _tchar * pPrototypeTag, _uint iLevelIndex, const _tchar * pLayerTag, void * pArg)
{
	if (nullptr == m_pGameObject_Manager)
		return E_FAIL;

	return m_pGameObject_Manager->Add_GameObject(iPrototypeLevelIndex, pPrototypeTag, iLevelIndex, pLayerTag, pArg);
}


#pragma endregion 

#pragma region Component_Manager

HRESULT CGameInstance::Add_Prototype(_uint iNumLevels, const _tchar * pComponentTag, CComponent * pPrototype)
{
	if (nullptr == m_pComponent_Manager)
		return E_FAIL;

	return m_pComponent_Manager->Add_Prototype(iNumLevels, pComponentTag, pPrototype);
}

CComponent * CGameInstance::Clone_Component(_uint iNumLevels, const _tchar * pComponentTag, void * pArg)
{
	if (nullptr == m_pComponent_Manager)
		return nullptr;

	return m_pComponent_Manager->Clone_Component(iNumLevels, pComponentTag, pArg);
}
#pragma endregion


_ulong CGameInstance::Release_Engine()
{
	/* 엔진내에서 사용된 객체들을 정리한다.  */
	_ulong		dwRefCnt = 0;

	if (dwRefCnt = CGameInstance::GetInstance()->DestroyInstance())
		MSG_BOX("Failed to Release CGameInstance");

	if (dwRefCnt = CLevel_Manager::GetInstance()->DestroyInstance())
		MSG_BOX("Failed to Release CLevel_Manager");

	if (dwRefCnt = CGameObject_Manager::GetInstance()->DestroyInstance())
		MSG_BOX("Failed to Release CGameObject_Manager");

	if (dwRefCnt = CComponent_Manager::GetInstance()->DestroyInstance())
		MSG_BOX("Failed to Release CComponent_Manager");

	if (dwRefCnt = CGraphic_Device::GetInstance()->DestroyInstance())
		MSG_BOX("Failed to Release CGraphic_Device");

	return dwRefCnt;
}

void CGameInstance::Free()
{
	// 감소
	Safe_Release(m_pLevel_Manager);
	Safe_Release(m_pGameObject_Manager);
	Safe_Release(m_pComponent_Manager);

	/* 게임인스턴스 안에 선언된 멤버함수의 정리를 수행한다. */
	Safe_Release(m_pGraphic_Device);
}
