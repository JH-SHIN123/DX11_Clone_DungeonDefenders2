#include "stdafx.h"
#include "..\public\MainApp.h"
#include "Level_Loading.h"
#include "Level_Logo.h"
#include "GameInstance.h"
#include "Renderer.h"

CMainApp::CMainApp()
	: m_pGameInstance(CGameInstance::GetInstance())
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CMainApp::Ready_MainApp()
{
	SetWindowText(g_hWnd, TEXT("Dungeon Defendus"));

	if (nullptr == m_pGameInstance)
		return E_FAIL;

	if (FAILED(m_pGameInstance->Initialize(g_hWnd, CGraphic_Device::TYPE_WINMODE, g_iWinCX, g_iWinCY, &m_pDevice, &m_pDevice_Context)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Reserve_Container((_uint)ELevel::End)))
		return E_FAIL;

	if (FAILED(Ready_Component_PrototypeForStatic()))
		return E_FAIL;

	if (FAILED(Ready_DefaultLevel(ELevel::Logo)))
		return E_FAIL;
	

	return S_OK;
}

_int CMainApp::Update_MainApp(_double TimeDelta)
{
	if (nullptr == m_pGameInstance)
		return -1;

#ifdef _DEBUG
	if (GetAsyncKeyState('Q') & 0x8000)
	{

	}

#endif // _DEBUG

	return m_pGameInstance->Tick(TimeDelta);	
}

HRESULT CMainApp::Render_MainApp()
{
	if (nullptr == m_pGameInstance)
		return E_FAIL;


	m_pGameInstance->Clear_Back_Buffer(_float4(0.f, 0.f, 1.f, 1.f));
	m_pGameInstance->Clear_Depth_Stencil_Buffer();

	m_pGameInstance->Render_Level();

	m_pGameInstance->Present();

	return S_OK;
}

HRESULT CMainApp::Ready_DefaultLevel(ELevel eLevelID)
{
	if (nullptr == m_pGameInstance)
		return E_FAIL;

	if (FAILED(m_pGameInstance->SetUp_CurrentLevel(CLevel_Loading::Create(m_pDevice, m_pDevice_Context, eLevelID))))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::Ready_Component_PrototypeForStatic()
{
	if (nullptr == m_pGameInstance)
		return E_FAIL;

	/* Renderer */
	if (FAILED(m_pGameInstance->Add_Prototype((_uint)ELevel::Static, TEXT("Component_Renderer"), m_pRenderer = CRenderer::Create(m_pDevice, m_pDevice_Context))))
		return E_FAIL;
	Safe_AddRef(m_pRenderer);

	/* Transform */


	return S_OK;
}

CMainApp * CMainApp::Create()
{
	CMainApp*		pInstance = new CMainApp;

	if (FAILED(pInstance->Ready_MainApp()))
	{
		MSG_BOX("Failed to Creating Instance (CMainApp) ");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainApp::Free()
{
	Safe_Release(m_pRenderer);
	Safe_Release(m_pDevice_Context);
	Safe_Release(m_pDevice);

	Safe_Release(m_pGameInstance);

	/* 엔진 내에서 사용된 객체들을 정리한다. */
	CGameInstance::Release_Engine();	
	
}


