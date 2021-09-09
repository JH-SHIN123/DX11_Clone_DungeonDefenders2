#include "stdafx.h"
#include "..\public\MainApp.h"
#include "Level_Loading.h"
#include "Level_Logo.h"
#include "GameInstance.h"
#include "Cursor_Manager.h"
#include "Text_Manager.h"
#include "Data_Manager.h"

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

	// 알트 엔터로 전체화면을 했을 시에 대한 화면 처리를 해야한다. 
	if (FAILED(m_pGameInstance->Initialize(g_hInst, g_hWnd, CGraphic_Device::TYPE_WINMODE, g_iWinCX, g_iWinCY, &m_pDevice, &m_pDevice_Context)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Reserve_Container((_uint)ELevel::End)))
		return E_FAIL;

	if (FAILED(Ready_Component_PrototypeForStatic()))
		return E_FAIL;

	if (FAILED(Ready_UI_Texture()))
		return E_FAIL;

	if (FAILED(Ready_Cursor()))
		return E_FAIL;

	if (FAILED(Ready_DefaultLevel(ELevel::Stage1)))
		return E_FAIL;

	CText_Manager::GetInstance()->Ready_Font();
	if (FAILED(GET_GAMEINSTANCE->Add_Prototype((_uint)ELevel::Static, TEXT("Component_Texture_Font")
		, CTextures::Create(m_pDevice, m_pDevice_Context, ETextureType::Tga, TEXT("../Bin/Data/Font/font.tga")))))
		return E_FAIL;

	//ShowCursor(false);

	return S_OK;
}

_int CMainApp::Update_MainApp(_double TimeDelta)
{
	if (nullptr == m_pGameInstance)
		return -1;
	// Tick함수 내에서 매 프레임마다 갱신해야 하는 것들이 전부 들어가 있다.

	// 마우스 업뎃
	GET_CURSOR_MANAGER->Tick((_float)TimeDelta);
	if (CData_Manager::GetInstance()->Get_Tick_Stop() == true)
		TimeDelta = 0.f;

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
	// 마우스 렌더

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
	HRESULT hr = S_OK;

	hr = m_pGameInstance->Add_Prototype((_uint)ELevel::Static, TEXT("Component_Texture_Failed")
		, CTextures::Create(m_pDevice, m_pDevice_Context, ETextureType::Wic, TEXT("../Bin/Resources/Textures/Failed.png")));


	hr = m_pGameInstance->Add_Prototype((_uint)ELevel::Static, TEXT("Component_Texture_HpMp")
		, CTextures::Create(m_pDevice, m_pDevice_Context, ETextureType::Tga, TEXT("../Bin/Resources/Textures/UI/HpMp/%d.tga"), 5));


	hr = m_pGameInstance->Add_Prototype((_uint)ELevel::Static, TEXT("Component_Texture_StatusPanel")
		, CTextures::Create(m_pDevice, m_pDevice_Context, ETextureType::Tga, TEXT("../Bin/Resources/Textures/UI/StatusPanel/StatusPanel.tga")));
	hr = m_pGameInstance->Add_Prototype((_uint)ELevel::Static, TEXT("Component_Texture_ExpBar")
		, CTextures::Create(m_pDevice, m_pDevice_Context, ETextureType::Tga, TEXT("../Bin/Resources/Textures/UI/StatusPanel/ExpBar/%d.tga"), 3));
	hr = m_pGameInstance->Add_Prototype((_uint)ELevel::Static, TEXT("Component_Texture_CastingBar")
		, CTextures::Create(m_pDevice, m_pDevice_Context, ETextureType::Tga, TEXT("../Bin/Resources/Textures/UI/Casting/%d.tga"), 3));


	hr = m_pGameInstance->Add_Prototype((_uint)ELevel::Static, TEXT("Component_Texture_SkillMage")
		,CTextures::Create(m_pDevice, m_pDevice_Context, ETextureType::Tga, TEXT("../Bin/Resources/Textures/UI/Skill_Mage/Skill_%d.tga"), 10));
	hr = m_pGameInstance->Add_Prototype((_uint)ELevel::Static, TEXT("Component_Texture_CoolDown")
		,CTextures::Create(m_pDevice, m_pDevice_Context, ETextureType::Wic, TEXT("../Bin/Resources/Textures/UI/Skill_Mage/CoolDown.png")));


	hr = m_pGameInstance->Add_Prototype((_uint)ELevel::Static, TEXT("Component_Texture_EnemyKillBar")
		,CTextures::Create(m_pDevice, m_pDevice_Context, ETextureType::Tga, TEXT("../Bin/Resources/Textures/UI/StageInfo/EnemyKillBar.tga")));
	hr = m_pGameInstance->Add_Prototype((_uint)ELevel::Static, TEXT("Component_Texture_EnemyKillBarMask")
		,CTextures::Create(m_pDevice, m_pDevice_Context, ETextureType::Tga, TEXT("../Bin/Resources/Textures/UI/StageInfo/EnemyKillBarMask.tga")));


	hr = m_pGameInstance->Add_Prototype((_uint)ELevel::Static, TEXT("Component_Texture_MeterTile")
		,CTextures::Create(m_pDevice, m_pDevice_Context, ETextureType::Tga, TEXT("../Bin/Resources/Textures/UI/StageInfo/MeterTile.tga")));


	hr = m_pGameInstance->Add_Prototype((_uint)ELevel::Static, TEXT("Component_Texture_Panel_Level")
		,CTextures::Create(m_pDevice, m_pDevice_Context, ETextureType::Tga, TEXT("../Bin/Resources/Textures/UI/StageInfo/Panel_Level.tga")));
	hr = m_pGameInstance->Add_Prototype((_uint)ELevel::Static, TEXT("Component_Texture_Panel_Wave")
		,CTextures::Create(m_pDevice, m_pDevice_Context, ETextureType::Tga, TEXT("../Bin/Resources/Textures/UI/StageInfo/Panel_Wave.tga")));


	hr = m_pGameInstance->Add_Prototype((_uint)ELevel::Static, TEXT("Component_Texture_WaveInfo_Text_Wave")
		,CTextures::Create(m_pDevice, m_pDevice_Context, ETextureType::Wic, TEXT("../Bin/Resources/Textures/UI/Text/WaveInfo_Text/Wave_%d.png")));
	hr = m_pGameInstance->Add_Prototype((_uint)ELevel::Static, TEXT("Component_Texture_WaveInfo_Text_Phase")
		,CTextures::Create(m_pDevice, m_pDevice_Context, ETextureType::Wic, TEXT("../Bin/Resources/Textures/UI/Text/WaveInfo_Text/Phase_%d.png"), 2));
	hr = m_pGameInstance->Add_Prototype((_uint)ELevel::Static, TEXT("Component_Texture_WaveInfo_Text_DefenseUnits")
		,CTextures::Create(m_pDevice, m_pDevice_Context, ETextureType::Wic, TEXT("../Bin/Resources/Textures/UI/Text/WaveInfo_Text/DefenseUnits_%d.png")));
	hr = m_pGameInstance->Add_Prototype((_uint)ELevel::Static, TEXT("Component_Texture_WaveInfo_Text_Score")
		,CTextures::Create(m_pDevice, m_pDevice_Context, ETextureType::Wic, TEXT("../Bin/Resources/Textures/UI/Text/Score/Score.png")));
	

	hr = m_pGameInstance->Add_Prototype((_uint)ELevel::Static, TEXT("Component_Texture_PauseMenu")
		,CTextures::Create(m_pDevice, m_pDevice_Context, ETextureType::Tga, TEXT("../Bin/Resources/Textures/PauseMenu/%d.tga"), 3));


	hr = m_pGameInstance->Add_Prototype((_uint)ELevel::Static, TEXT("Component_Texture_Status")
		, CTextures::Create(m_pDevice, m_pDevice_Context, ETextureType::Tga, TEXT("../Bin/Resources/Textures/Status/%d.tga"), 5));
	hr = m_pGameInstance->Add_Prototype((_uint)ELevel::Static, TEXT("Component_Texture_Status_HpMp")
		, CTextures::Create(m_pDevice, m_pDevice_Context, ETextureType::Tga, TEXT("../Bin/Resources/Textures/Status/HpMp/%d.tga"), 4));
	hr = m_pGameInstance->Add_Prototype((_uint)ELevel::Static, TEXT("Component_Texture_Status_Portrait")
		, CTextures::Create(m_pDevice, m_pDevice_Context, ETextureType::Tga, TEXT("../Bin/Resources/Textures/Status/Portrait/%d.tga"), 3));
	hr = m_pGameInstance->Add_Prototype((_uint)ELevel::Static, TEXT("Component_Texture_Status_Icon")
		, CTextures::Create(m_pDevice, m_pDevice_Context, ETextureType::Tga, TEXT("../Bin/Resources/Textures/Status/Stat/%d.tga"), 4));


	hr = m_pGameInstance->Add_Prototype((_uint)ELevel::Static, TEXT("Component_Texture_Black")
		, CTextures::Create(m_pDevice, m_pDevice_Context, ETextureType::Wic, TEXT("../Bin/Resources/Textures/Black.png")));



	if (hr != S_OK)
		MSG_BOX("CMainApp::Ready_UI_Texture() is Failed");

	return hr;
}

HRESULT CMainApp::Ready_Cursor()
{
	const _tchar* TextureName = L"Component_Texture_Cursor";

	if (FAILED(m_pGameInstance->Add_Prototype((_uint)ELevel::Static, TextureName
		, CTextures::Create(m_pDevice, m_pDevice_Context, ETextureType::Tga, TEXT("../Bin/Resources/Textures/Cursor/Icon_%d.tga"), 3))))
		return E_FAIL;

	CCursor_Manager::GetInstance()->Set_Cursor(CCursor::Create(m_pDevice, m_pDevice_Context, TextureName));

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

	CText_Manager::DestroyInstance();
	CData_Manager::DestroyInstance();
	CCursor_Manager::DestroyInstance();
	/* 엔진 내에서 사용된 객체들을 정리한다. */
	CGameInstance::Release_Engine();	
	
	ShowCursor(true);
}


