#include "..\public\GameInstance.h"

IMPLEMENT_SINGLETON(CGameInstance)

CGameInstance::CGameInstance()
	: m_pGraphic_Device(CGraphic_Device::GetInstance())
	, m_pLevel_Manager(CLevel_Manager::GetInstance())
	, m_pGameObject_Manager(CGameObject_Manager::GetInstance())
	, m_pComponent_Manager(CComponent_Manager::GetInstance())
	, m_pPipeline_Manager(CPipeline_Manager::GetInstance())
	, m_pInputDev_Manager(CInputDev::GetInstance())
	, m_pTimer_Manager(CTimer_Manager::GetInstance())
	, m_pFrame_Manger(CFrame_Manger::GetInstance())
	, m_pLight_Manager(CLight_Manager::GetInstance())
	, m_pSound_Manager(CSound_Manager::GetInstance())
{
	Safe_AddRef(m_pComponent_Manager);
	Safe_AddRef(m_pGameObject_Manager);
	Safe_AddRef(m_pLevel_Manager);
	Safe_AddRef(m_pGraphic_Device);
	Safe_AddRef(m_pPipeline_Manager);
	Safe_AddRef(m_pLight_Manager);
}

HRESULT CGameInstance::Initialize(HINSTANCE hInst, HWND hWnd, CGraphic_Device::WINMODE eWinMode, _uint iSizeX, _uint iSizeY, ID3D11Device** ppDevice, ID3D11DeviceContext** ppDevice_Context)
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	if (FAILED(m_pGraphic_Device->Ready_Graphic_Device(eWinMode, hWnd, iSizeX, iSizeY, ppDevice, ppDevice_Context)))
		return E_FAIL;

	if (nullptr != hInst)
	{
		if (FAILED(m_pInputDev_Manager->Ready_InputDev(hInst, hWnd)))
			return E_FAIL;
	}

	m_pSound_Manager->Initialize();
	m_pSound_Manager->LoadSoundFile();

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

_int CGameInstance::Tick(_float TimeDelta)
{
	if (nullptr == m_pLevel_Manager ||
		nullptr == m_pGameObject_Manager)
		return -1;

	_int iEvent = 0;

	m_pInputDev_Manager->Update_InputDev();

	if (iEvent = m_pGameObject_Manager->Tick_GameObject(TimeDelta))
		return iEvent;

	m_pPipeline_Manager->Tick();
	m_pSound_Manager->UpdateSound();
	//return 0;

	return m_pLevel_Manager->Tick(TimeDelta);
}

void CGameInstance::Clear_This_Level(_uint iLevelIndex)
{
	if (nullptr == m_pComponent_Manager ||
		nullptr == m_pGameObject_Manager)
		return;

	m_pComponent_Manager->Clear(iLevelIndex);
	m_pGameObject_Manager->Clear(iLevelIndex);
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
D3D11_VIEWPORT CGameInstance::Get_ViewPort(D3D11_VIEWPORT* ViewPort)
{
	return m_pGraphic_Device->Get_ViewPort(ViewPort);
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

CLevel * CGameInstance::Get_Scene()
{
	if (nullptr == m_pLevel_Manager)
		return nullptr;

	return m_pLevel_Manager->Get_Scene();
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

CGameObject* CGameInstance::Add_Create_Clone(_uint iPrototypeLevelIndex, const _tchar * pPrototypeTag, _uint iLevelIndex, void * pArg)
{
	if (nullptr == m_pGameObject_Manager)
		return nullptr;

	return m_pGameObject_Manager->Add_Create_Clone(iPrototypeLevelIndex, pPrototypeTag, iLevelIndex, pArg);
}

CGameObject * CGameInstance::Get_GameObject(_uint iLevelIndex, const _tchar * pLayerTag)
{
	if (nullptr == m_pGameObject_Manager)
		return nullptr;

	return m_pGameObject_Manager->Get_GameObject(iLevelIndex, pLayerTag);
}

CLayer * CGameInstance::Get_Layer(_uint iLevelIndex, const _tchar * pLayerTag)
{
	if (nullptr == m_pGameObject_Manager)
		return nullptr;

	return m_pGameObject_Manager->Get_Layer(iLevelIndex, pLayerTag);
}

const list<CGameObject*>* CGameInstance::Get_GameObject_List(_uint iLevelIndex, const _tchar * pLayerTag)
{
	if (nullptr == m_pGameObject_Manager)
		return nullptr;

	return nullptr;
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
_matrix CGameInstance::Get_Transform(ETransformState eState)
{
	if (nullptr == m_pPipeline_Manager)
		return XMMatrixIdentity();

	return m_pPipeline_Manager->Get_Transform(eState);
}

_vector CGameInstance::Get_CamPosition() const
{
	if (nullptr == m_pPipeline_Manager)
		return _vector();

	return m_pPipeline_Manager->Get_CamPosition();
}

_fvector CGameInstance::Get_CamRight_Dir() const
{
	if (nullptr == m_pPipeline_Manager)
		return _fvector();

	return m_pPipeline_Manager->Get_CamRight_Dir();
}

_fvector CGameInstance::Get_CamUp_Dir() const
{
	if (nullptr == m_pPipeline_Manager)
		return _fvector();

	return m_pPipeline_Manager->Get_CamUp_Dir();
}

_fvector CGameInstance::Get_CamLook_Dir() const
{
	if (nullptr == m_pPipeline_Manager)
		return _fvector();

	return m_pPipeline_Manager->Get_CamLook_Dir();
}

#pragma endregion 

#pragma region Light_Manager

HRESULT CGameInstance::Reserve_Container_Light(_uint iNumLight)
{
	if (nullptr == m_pLight_Manager)
		return E_FAIL;

	m_pLight_Manager->Reserve_Container((iNumLight));

	return S_OK;
}
HRESULT CGameInstance::Add_Light(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context, const LIGHT_DESC & Light_Desc)
{
	if (nullptr == m_pLight_Manager)
		return E_FAIL;

	m_pLight_Manager->Add_Light(pDevice, pDevice_Context, Light_Desc);
	return S_OK;
}
LIGHT_DESC * CGameInstance::Get_LightDesc(_int iLightIndex)
{
	if (nullptr == m_pLight_Manager)
		return nullptr;

	return m_pLight_Manager->Get_LightDesc(iLightIndex);
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

	if (dwRefCnt = CLight_Manager::GetInstance()->DestroyInstance())
		MSG_BOX("Failed to Release CPipeline_Manager");

	if (dwRefCnt = CPipeline_Manager::GetInstance()->DestroyInstance())
		MSG_BOX("Failed to Release CPipeline_Manager");

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
	Safe_Release(m_pLight_Manager);

	// 삭제
	Safe_Release(m_pPipeline_Manager);
	Safe_Release(m_pInputDev_Manager);
	Safe_Release(m_pTimer_Manager);
	Safe_Release(m_pFrame_Manger);
	Safe_Release(m_pSound_Manager);

	/* 게임인스턴스 안에 선언된 멤버함수의 정리를 수행한다. */
	Safe_Release(m_pGraphic_Device);
}
