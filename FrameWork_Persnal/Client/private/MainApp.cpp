#include "stdafx.h"
#include "..\public\MainApp.h"
#include "Level_Loading.h"
#include "Level_Logo.h"
#include "GameInstance.h"

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

	if (FAILED(m_pGameInstance->Initialize(g_hInst, g_hWnd, CGraphic_Device::TYPE_WINMODE, g_iWinCX, g_iWinCY, &m_pDevice, &m_pDevice_Context)))
		return E_FAIL;

	// Level의 수에 맞게 컨테이너의 크기를 다시 잡음
	if (FAILED(m_pGameInstance->Reserve_Container((_uint)ELevel::End)))
		return E_FAIL;

	if (FAILED(Ready_Component_PrototypeForStatic()))
		return E_FAIL;

	if (FAILED(Ready_UI_Texture()))
		return E_FAIL;

	if (FAILED(Ready_DefaultLevel(ELevel::Logo)))
		return E_FAIL;
	

	return S_OK;
}

_int CMainApp::Update_MainApp(_double TimeDelta)
{
	if (nullptr == m_pGameInstance)
		return -1;
	// Tick함수 내에서 매 프레임마다 갱신해야 하는 것들이 전부 들어가 있다.
	m_pGameInstance->Get_InputDev()->Update_InputDev();

	return m_pGameInstance->Tick((_float)TimeDelta);
}

HRESULT CMainApp::Render_MainApp()
{
	if (nullptr == m_pGameInstance)
		return E_FAIL;


	m_pGameInstance->Clear_Back_Buffer(_float4(0.f, 0.f, 1.f, 1.f));
	m_pGameInstance->Clear_Depth_Stencil_Buffer();

	m_pRenderer->Draw_Renderer();
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
	if (FAILED(m_pGameInstance->Add_Prototype((_uint)ELevel::Static, TEXT("Component_Renderer")
		, m_pRenderer = CRenderer::Create(m_pDevice, m_pDevice_Context))))
		return E_FAIL;
	Safe_AddRef(m_pRenderer);


	/* Transform */
	if (FAILED(m_pGameInstance->Add_Prototype((_uint)ELevel::Static, TEXT("Component_Transform")
		, CTransform::Create(m_pDevice, m_pDevice_Context))))
		return E_FAIL;

	/* Movement */
	if (FAILED(m_pGameInstance->Add_Prototype((_uint)ELevel::Static, TEXT("Component_Movement")
		, CMovement::Create(m_pDevice, m_pDevice_Context))))
		return E_FAIL;


	// For. VIBuffer_RECT
	if (FAILED(m_pGameInstance->Add_Prototype((_uint)ELevel::Static, TEXT("Component_VIBuffer_Rect")
		, CVIBuffer_Rect::Create(m_pDevice, m_pDevice_Context, TEXT("../Bin/Shader/Shader_Default.hlsl"), "DefaultTechnique"))))
		return E_FAIL;

	// For. VIBuffer_RECT Model
	if (FAILED(m_pGameInstance->Add_Prototype((_uint)ELevel::Static, TEXT("Component_VIBuffer_Rect_Model")
		, CVIBuffer_Rect::Create(m_pDevice, m_pDevice_Context, TEXT("../Bin/Shader/Shader_Model.hlsl"), "DefaultTechnique"))))
		return E_FAIL;

	// 이건 나중에 해결해 보자 텍스처 UV를 _float3로 못씀
	//if (FAILED(m_pGameInstance->Add_Prototype((_uint)ELevel::Static, TEXT("Component_VIBuffer_Cube")
	//	, CVIBuffer_Cube::Create(m_pDevice, m_pDevice_Context, TEXT("../Bin/Shader/Shader_Cube.hlsl"), "DefaultTechnique"))))
	//	return E_FAIL;


	return S_OK;
}

HRESULT CMainApp::Ready_UI_Texture()
{
	// For.Texture Load Failed Texture
	if (FAILED(m_pGameInstance->Add_Prototype((_uint)ELevel::Static, TEXT("Component_Texture_Failed")
		, CTextures::Create(m_pDevice, m_pDevice_Context, ETextureType::Wic, TEXT("../Bin/Resources/Textures/Failed.png")))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype((_uint)ELevel::Static, TEXT("Component_Texture_HpMp")
		, CTextures::Create(m_pDevice, m_pDevice_Context, ETextureType::Tga, TEXT("../Bin/Resources/Textures/UI/HpMp/%d.tga"), 4))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype((_uint)ELevel::Static, TEXT("Component_Texture_StatusPanel")
		, CTextures::Create(m_pDevice, m_pDevice_Context, ETextureType::Tga, TEXT("../Bin/Resources/Textures/UI/StatusPanel/StatusPanel.tga")))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype((_uint)ELevel::Static, TEXT("Component_Texture_Panel_newBar")
		, CTextures::Create(m_pDevice, m_pDevice_Context, ETextureType::Tga, TEXT("../Bin/Resources/Textures/UI/StatusPanel/Panel_newBar.tga")))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype((_uint)ELevel::Static, TEXT("Component_Texture_newBarMask")
		, CTextures::Create(m_pDevice, m_pDevice_Context, ETextureType::Tga, TEXT("../Bin/Resources/Textures/UI/StatusPanel/newBarMask.tga")))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype((_uint)ELevel::Static, TEXT("Component_Texture_SkillMage")
		, CTextures::Create(m_pDevice, m_pDevice_Context, ETextureType::Tga, TEXT("../Bin/Resources/Textures/UI/Skill_Mage/Skill_%d.tga"), 10))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype((_uint)ELevel::Static, TEXT("Component_Texture_CoolDown")
		, CTextures::Create(m_pDevice, m_pDevice_Context, ETextureType::Wic, TEXT("../Bin/Resources/Textures/UI/Skill_Mage/CoolDown.png")))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype((_uint)ELevel::Static, TEXT("Component_Texture_EnemyKillBar")
		, CTextures::Create(m_pDevice, m_pDevice_Context, ETextureType::Tga, TEXT("../Bin/Resources/Textures/UI/StageInfo/EnemyKillBar.tga")))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype((_uint)ELevel::Static, TEXT("Component_Texture_EnemyKillBarMask")
		, CTextures::Create(m_pDevice, m_pDevice_Context, ETextureType::Tga, TEXT("../Bin/Resources/Textures/UI/StageInfo/EnemyKillBarMask.tga")))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype((_uint)ELevel::Static, TEXT("Component_Texture_MeterTile")
		, CTextures::Create(m_pDevice, m_pDevice_Context, ETextureType::Tga, TEXT("../Bin/Resources/Textures/UI/StageInfo/MeterTile.tga")))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype((_uint)ELevel::Static, TEXT("Component_Texture_Panel_Level")
		, CTextures::Create(m_pDevice, m_pDevice_Context, ETextureType::Tga, TEXT("../Bin/Resources/Textures/UI/StageInfo/Panel_Level.tga")))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype((_uint)ELevel::Static, TEXT("Component_Texture_Panel_Wave")
		, CTextures::Create(m_pDevice, m_pDevice_Context, ETextureType::Tga, TEXT("../Bin/Resources/Textures/UI/StageInfo/Panel_Wave.tga")))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype((_uint)ELevel::Static, TEXT("Component_Texture_WaveInfo_Text_Wave")
		, CTextures::Create(m_pDevice, m_pDevice_Context, ETextureType::Wic, TEXT("../Bin/Resources/Textures/UI/Text/WaveInfo_Text/Wave_%d.png")))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype((_uint)ELevel::Static, TEXT("Component_Texture_WaveInfo_Text_Phase")
		, CTextures::Create(m_pDevice, m_pDevice_Context, ETextureType::Wic, TEXT("../Bin/Resources/Textures/UI/Text/WaveInfo_Text/Phase_%d.png"), 2))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype((_uint)ELevel::Static, TEXT("Component_Texture_WaveInfo_Text_DefenseUnits")
		, CTextures::Create(m_pDevice, m_pDevice_Context, ETextureType::Wic, TEXT("../Bin/Resources/Textures/UI/Text/WaveInfo_Text/DefenseUnits_%d.png")))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype((_uint)ELevel::Static, TEXT("Component_Texture_WaveInfo_Text_Score")
		, CTextures::Create(m_pDevice, m_pDevice_Context, ETextureType::Wic, TEXT("../Bin/Resources/Textures/UI/Text/Score/Score.png")))))
		return E_FAIL;

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


