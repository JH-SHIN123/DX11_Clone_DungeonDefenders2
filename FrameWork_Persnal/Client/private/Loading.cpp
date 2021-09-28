 #include "stdafx.h"
#include "..\public\Loading.h"

#include "BackGround_Logo.h"
#include "MainMenu.h"
#include "VIBuffer_Terrain.h"

#include "Terrain.h"
#include "Player.h"
#include "Status_Panel.h"
#include "PlayerSkill.h"
#include "HpMp.h"
#include "PhaseInfo.h"
#include "Camera_Target.h"
#include "Camera_Free.h"
#include "PauseMenu.h"
#include "Phase_Panel.h"
#include "StatusMenu.h"
#include "BlockadeTower.h"
#include "StrikerTower.h"
#include "StrikerTower_Bullet.h"
#include "LightningTower.h"
#include "LightningTower_Bullet.h"
#include "LightningTower_Bullet_Effect.h"
#include "Goblin.h"
#include "Archer.h"
#include "Archer_Arrow.h"
#include "Skill_ManaBomb.h"
#include "Skill_Meteor.h"

USING(Engine)

CLoading::CLoading(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: m_pDevice(pDevice)
	, m_pDevice_Context(pDevice_Context)
	, m_pGameInstance(CGameInstance::GetInstance())
{
	Safe_AddRef(m_pGameInstance);
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pDevice_Context);
}

_uint APIENTRY ThreadMain(void* pArg)
{
	CLoading*		pLoading = (CLoading*)pArg;

	EnterCriticalSection(pLoading->Get_CriticalSectionPtr());

	HRESULT		hr = 0;

	switch (pLoading->Get_NextSceneID())
	{
	case ELevel::Logo:
		hr = pLoading->LoadingForLogo();
		break;
	case ELevel::Stage1:
		hr = pLoading->LoadingForStage();
		break;
	}

	if (FAILED(hr))
		return E_FAIL;

	LeaveCriticalSection(pLoading->Get_CriticalSectionPtr());

	return 0;
}

HRESULT CLoading::NativeConstruct(ELevel eNextSceneID)
{
	InitializeCriticalSection(&m_CriticalSection);

	m_eNextSceneID = eNextSceneID;

	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, ThreadMain, this, 0, nullptr);

	return S_OK;
}

HRESULT CLoading::LoadingForLogo()
{
	if (nullptr == m_pGameInstance)
		return E_FAIL;

	HRESULT hr = S_OK;

	// Component
	hr = m_pGameInstance->Add_Prototype((_uint)ELevel::Logo, TEXT("Component_Texture_Devil")
		, CTextures::Create(m_pDevice, m_pDevice_Context, ETextureType::Wic, TEXT("../Bin/Resources/Textures/Devil.png")));

	hr = m_pGameInstance->Add_Prototype((_uint)ELevel::Logo, TEXT("Component_Texture_Logo")
		, CTextures::Create(m_pDevice, m_pDevice_Context, ETextureType::Tga, TEXT("../Bin/Resources/Textures/MainMenu/Logo.tga")));

	hr = m_pGameInstance->Add_Prototype((_uint)ELevel::Logo, TEXT("Component_Texture_Generic_Panel")
		, CTextures::Create(m_pDevice, m_pDevice_Context, ETextureType::Tga, TEXT("../Bin/Resources/Textures/MainMenu/Menu_%d.tga"), 7));

	hr = m_pGameInstance->Add_Prototype((_uint)ELevel::Logo, TEXT("Component_Texture_UI_TabMenu")
		, CTextures::Create(m_pDevice, m_pDevice_Context, ETextureType::Tga, TEXT("../Bin/Resources/Textures/Tab_Menu/%d.tga"), 4));

	hr = m_pGameInstance->Add_Prototype((_uint)ELevel::Logo, TEXT("Component_Texture_UI_Stage_Select")
		, CTextures::Create(m_pDevice, m_pDevice_Context, ETextureType::Tga, TEXT("../Bin/Resources/Textures/MainMenu/Stage_Select/Stage/%d.tga"), 2));
	hr = m_pGameInstance->Add_Prototype((_uint)ELevel::Logo, TEXT("Component_Texture_UI_Stage_Select_Difficult")
		, CTextures::Create(m_pDevice, m_pDevice_Context, ETextureType::Tga, TEXT("../Bin/Resources/Textures/MainMenu/Stage_Select/Difficult/%d.tga"), 3));

	hr = m_pGameInstance->Add_Prototype((_uint)ELevel::Logo, TEXT("Component_Texture_Option")
		, CTextures::Create(m_pDevice, m_pDevice_Context, ETextureType::Tga, TEXT("../Bin/Resources/Textures/MainMenu/Option/%d.tga"), 6));
	hr = m_pGameInstance->Add_Prototype((_uint)ELevel::Logo, TEXT("Component_Texture_Option_Arrow")
		, CTextures::Create(m_pDevice, m_pDevice_Context, ETextureType::Tga, TEXT("../Bin/Resources/Textures/MainMenu/Option/Arrow/%d.tga"), 2));


	// Prototype
	hr = m_pGameInstance->Add_Prototype((_uint)ELevel::Logo, TEXT("Prototype_BackGround_Logo"), CBackGround_Logo::Create(m_pDevice, m_pDevice_Context));
	hr = m_pGameInstance->Add_Prototype((_uint)ELevel::Logo, TEXT("Prototype_MainMenu"), CMainMenu::Create(m_pDevice, m_pDevice_Context));


	if (hr != S_OK)
		MSG_BOX("LoadingForLogo Failed!");

	m_isFinished = true;

	return hr;
}

HRESULT CLoading::LoadingForStage()
{
	if (nullptr == m_pGameInstance)
		return E_FAIL;

	SetWindowText(g_hWnd, TEXT("TMXPDLWL로딩 시작"));

	HRESULT hr = S_OK;

	// 앵간하면 GameObject가 Component를 가지는 구조가 되니까 Component를 먼저 생성하는게 깔끔함
	// 터레인은 생성 되면서 높이맵과 쉐이더 그리고 해당 쉐이더의 테크닉(네임스페이스 개념)을 받고있다

	// Component
	hr = m_pGameInstance->Add_Prototype((_uint)ELevel::Stage1, TEXT("Component_VIBuffer_Terrain")
		, CVIBuffer_Terrain::Create(m_pDevice, m_pDevice_Context, L"../Bin/Resources/Textures/Height.bmp", TEXT("../Bin/Shader/Shader_Terrain.hlsl"), "DefaultTechnique"));

	hr = m_pGameInstance->Add_Prototype((_uint)ELevel::Stage1, TEXT("Component_Texture_Devil")
		, CTextures::Create(m_pDevice, m_pDevice_Context, ETextureType::Wic, TEXT("../Bin/Resources/Textures/Devil.png")));

	hr = m_pGameInstance->Add_Prototype((_uint)ELevel::Stage1, TEXT("Component_Texture_Terrain")
		, CTextures::Create(m_pDevice, m_pDevice_Context, ETextureType::Tga, TEXT("../Bin/Resources/Textures/Grass_0.tga")));

	hr = m_pGameInstance->Add_Prototype((_uint)ELevel::Static, TEXT("Component_Texture_TowerRange")
		, CTextures::Create(m_pDevice, m_pDevice_Context, ETextureType::Tga, TEXT("../Bin/Resources/Textures/UI/TowerRange/%d.tga"), 5));

	hr = m_pGameInstance->Add_Prototype((_uint)ELevel::Static, TEXT("Component_Texture_ActivateCrystal")
		, CTextures::Create(m_pDevice, m_pDevice_Context, ETextureType::Wic, TEXT("../Bin/Resources/Textures/UI/Text/ActivateCrystal/%d.png")));

	// GameObject
	hr = m_pGameInstance->Add_Prototype((_uint)ELevel::Stage1, TEXT("Prototype_Terrain"), CTerrain::Create(m_pDevice, m_pDevice_Context));

	hr = m_pGameInstance->Add_Prototype((_uint)ELevel::Static, TEXT("Prototype_StatusPanel"),		CStatus_Panel::Create(m_pDevice, m_pDevice_Context));
	hr = m_pGameInstance->Add_Prototype((_uint)ELevel::Static, TEXT("Prototype_PlayerSkill_UI"),	CPlayerSkill::Create(m_pDevice, m_pDevice_Context));
	hr = m_pGameInstance->Add_Prototype((_uint)ELevel::Static, TEXT("Prototype_Player_HpMp"),		CHpMp::Create(m_pDevice, m_pDevice_Context));
	hr = m_pGameInstance->Add_Prototype((_uint)ELevel::Static, TEXT("Prototype_PhaseInfo"),			CPhaseInfo::Create(m_pDevice, m_pDevice_Context));
	hr = m_pGameInstance->Add_Prototype((_uint)ELevel::Static, TEXT("Prototype_Phase_Panel"),		CPhase_Panel::Create(m_pDevice, m_pDevice_Context));
	hr = m_pGameInstance->Add_Prototype((_uint)ELevel::Static, TEXT("Prototype_PauseMenu"),			CPauseMenu::Create(m_pDevice, m_pDevice_Context));
	hr = m_pGameInstance->Add_Prototype((_uint)ELevel::Static, TEXT("Prototype_StatusMenu"),		CStatusMenu::Create(m_pDevice, m_pDevice_Context));

	hr = m_pGameInstance->Add_Prototype((_uint)ELevel::Static, TEXT("Prototype_Camera_Free"),	CCamera_Free::Create(m_pDevice, m_pDevice_Context));
	hr = m_pGameInstance->Add_Prototype((_uint)ELevel::Static, TEXT("Prototype_Camera_Target"), CCamera_Target::Create(m_pDevice, m_pDevice_Context));

	hr = m_pGameInstance->Add_Prototype((_uint)ELevel::Static, TEXT("Prototype_Player"), CPlayer::Create(m_pDevice, m_pDevice_Context));

	hr = m_pGameInstance->Add_Prototype((_uint)ELevel::Stage1, TEXT("Prototype_BlockadeTower"), CBlockadeTower::Create(m_pDevice, m_pDevice_Context));
	hr = m_pGameInstance->Add_Prototype((_uint)ELevel::Stage1, TEXT("Prototype_StrikerTower"), CStrikerTower::Create(m_pDevice, m_pDevice_Context));
	hr = m_pGameInstance->Add_Prototype((_uint)ELevel::Stage1, TEXT("Prototype_LightningTower"), CLightningTower::Create(m_pDevice, m_pDevice_Context));

	hr = m_pGameInstance->Add_Prototype((_uint)ELevel::Stage1, TEXT("Prototype_StrikerTower_Bullet"), CStrikerTower_Bullet::Create(m_pDevice, m_pDevice_Context));
	hr = m_pGameInstance->Add_Prototype((_uint)ELevel::Stage1, TEXT("Prototype_LightningTower_Bullet"), CLightningTower_Bullet::Create(m_pDevice, m_pDevice_Context));
	hr = m_pGameInstance->Add_Prototype((_uint)ELevel::Stage1, TEXT("Prototype_LightningTower_Bullet_Effect"), CLightningTower_Bullet_Effect::Create(m_pDevice, m_pDevice_Context));

	hr = m_pGameInstance->Add_Prototype((_uint)ELevel::Stage1, TEXT("Prototype_Goblin"), CGoblin::Create(m_pDevice, m_pDevice_Context));
	hr = m_pGameInstance->Add_Prototype((_uint)ELevel::Stage1, TEXT("Prototype_Archer"), CArcher::Create(m_pDevice, m_pDevice_Context));


	hr = m_pGameInstance->Add_Prototype((_uint)ELevel::Stage1, TEXT("Prototype_Archer_Arrow"), CArcher_Arrow::Create(m_pDevice, m_pDevice_Context));

	hr = m_pGameInstance->Add_Prototype((_uint)ELevel::Stage1, TEXT("Prototype_Skill_ManaBomb"), CSkill_ManaBomb::Create(m_pDevice, m_pDevice_Context));
	hr = m_pGameInstance->Add_Prototype((_uint)ELevel::Stage1, TEXT("Prototype_Skill_Meteor"), CSkill_Meteor::Create(m_pDevice, m_pDevice_Context));


	// Model
	//hr = m_pGameInstance->Add_Prototype((_uint)ELevel::Stage1, TEXT("Component_Mesh_Level_1"),
	//	CModel::Create(m_pDevice, m_pDevice_Context, "../Bin/Resources/Mesh/Level_1/", "Magus.fbx", TEXT("../Bin/Shader/Shader_Model.hlsl"), "DefaultTechnique"));

	hr = m_pGameInstance->Add_Prototype((_uint)ELevel::Stage1, TEXT("Component_Mesh_Mage"),
		CModel::Create(m_pDevice, m_pDevice_Context, "../Bin/Resources/Mesh/Mage/", "Mage.fbx", TEXT("../Bin/Shader/Shader_Model.hlsl"), "DefaultTechnique"));

	hr = m_pGameInstance->Add_Prototype((_uint)ELevel::Stage1, TEXT("Component_Mesh_BlockcadeTower"),
		CModel::Create(m_pDevice, m_pDevice_Context, "../Bin/Resources/Mesh/Tower/Blockade/", "BlockcadeTower.fbx", TEXT("../Bin/Shader/Shader_Model.hlsl"), "DefaultTechnique"));

	hr = m_pGameInstance->Add_Prototype((_uint)ELevel::Stage1, TEXT("Component_Mesh_StrikerTower"),
		CModel::Create(m_pDevice, m_pDevice_Context, "../Bin/Resources/Mesh/Tower/Striker/", "StrikerTower.fbx", TEXT("../Bin/Shader/Shader_Model.hlsl"), "DefaultTechnique"));

	hr = m_pGameInstance->Add_Prototype((_uint)ELevel::Stage1, TEXT("Component_Mesh_StrikerTower_Bullet"),
		CModel::Create(m_pDevice, m_pDevice_Context, "../Bin/Resources/Mesh/Tower/Bullet/Striker/", "StrikerTower_Bullet.fbx", TEXT("../Bin/Shader/Shader_Model.hlsl"), "DefaultTechnique"));

	hr = m_pGameInstance->Add_Prototype((_uint)ELevel::Stage1, TEXT("Component_Mesh_LightningTower"),
		CModel::Create(m_pDevice, m_pDevice_Context, "../Bin/Resources/Mesh/Tower/Lightning/", "LightningTower.fbx", TEXT("../Bin/Shader/Shader_Model.hlsl"), "DefaultTechnique"));

	hr = m_pGameInstance->Add_Prototype((_uint)ELevel::Stage1, TEXT("Component_Mesh_LightningTower_Bullet"),
		CModel::Create(m_pDevice, m_pDevice_Context, "../Bin/Resources/Mesh/Tower/Bullet/Lightning/", "LightningTower_Bullet.fbx", TEXT("../Bin/Shader/Shader_Model.hlsl"), "DefaultTechnique"));
	hr = m_pGameInstance->Add_Prototype((_uint)ELevel::Stage1, TEXT("Component_Mesh_LightningTower_Bullet_Effect_1"),
		CModel::Create(m_pDevice, m_pDevice_Context, "../Bin/Resources/Mesh/Tower/Bullet/Lightning/Effects/", "Effect1.fbx", TEXT("../Bin/Shader/Shader_Model.hlsl"), "DefaultTechnique"));
	hr = m_pGameInstance->Add_Prototype((_uint)ELevel::Stage1, TEXT("Component_Mesh_LightningTower_Bullet_Effect_2"),
		CModel::Create(m_pDevice, m_pDevice_Context, "../Bin/Resources/Mesh/Tower/Bullet/Lightning/Effects/", "Effect2.fbx", TEXT("../Bin/Shader/Shader_Model.hlsl"), "DefaultTechnique"));
	hr = m_pGameInstance->Add_Prototype((_uint)ELevel::Stage1, TEXT("Component_Mesh_LightningTower_Bullet_Effect_3"),
		CModel::Create(m_pDevice, m_pDevice_Context, "../Bin/Resources/Mesh/Tower/Bullet/Lightning/Effects/", "Effect3.fbx", TEXT("../Bin/Shader/Shader_Model.hlsl"), "DefaultTechnique"));

	hr = m_pGameInstance->Add_Prototype((_uint)ELevel::Stage1, TEXT("Component_Mesh_Goblin"),
		CModel::Create(m_pDevice, m_pDevice_Context, "../Bin/Resources/Mesh/Monster/Goblin/", "Goblin.fbx", TEXT("../Bin/Shader/Shader_Model.hlsl"), "DefaultTechnique"));

	hr = m_pGameInstance->Add_Prototype((_uint)ELevel::Stage1, TEXT("Component_Mesh_Archer"),
		CModel::Create(m_pDevice, m_pDevice_Context, "../Bin/Resources/Mesh/Monster/Archer/", "Archer.fbx", TEXT("../Bin/Shader/Shader_Model.hlsl"), "DefaultTechnique"));
	hr = m_pGameInstance->Add_Prototype((_uint)ELevel::Stage1, TEXT("Component_Mesh_Archer_Arrow"),
		CModel::Create(m_pDevice, m_pDevice_Context, "../Bin/Resources/Mesh/Monster/Archer/Arrow/", "Archer_Arrow.fbx", TEXT("../Bin/Shader/Shader_Model.hlsl"), "DefaultTechnique"));

	hr = m_pGameInstance->Add_Prototype((_uint)ELevel::Stage1, TEXT("Component_Mesh_Staff_0"),
		CModel::Create(m_pDevice, m_pDevice_Context, "../Bin/Resources/Mesh/Staff/0/", "Staff_0.fbx", TEXT("../Bin/Shader/Shader_Model.hlsl"), "DefaultTechnique"));
	hr = m_pGameInstance->Add_Prototype((_uint)ELevel::Stage1, TEXT("Component_Mesh_Staff_1"),
		CModel::Create(m_pDevice, m_pDevice_Context, "../Bin/Resources/Mesh/Staff/1/", "Staff_1.fbx", TEXT("../Bin/Shader/Shader_Model.hlsl"), "DefaultTechnique"));

	hr = m_pGameInstance->Add_Prototype((_uint)ELevel::Stage1, TEXT("Component_Mesh_Skill_ManaBomb"),
		CModel::Create(m_pDevice, m_pDevice_Context, "../Bin/Resources/Mesh/Skill/ManaBomb/", "ManaBomb.fbx", TEXT("../Bin/Shader/Shader_Model.hlsl"), "DefaultTechnique"));
	hr = m_pGameInstance->Add_Prototype((_uint)ELevel::Stage1, TEXT("Component_Mesh_Skill_Meteor"),
		CModel::Create(m_pDevice, m_pDevice_Context, "../Bin/Resources/Mesh/Skill/Meteor/", "Meteor.fbx", TEXT("../Bin/Shader/Shader_Model.hlsl"), "DefaultTechnique"));


	if (hr != S_OK)
		MSG_BOX("LoadingForStage Fail");

	m_isFinished = true;

	return hr;
}

HRESULT CLoading::Loading_UI()
{
	return E_NOTIMPL;
}

CLoading * CLoading::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context, ELevel eNextSceneID)
{
	CLoading*		pInstance = new CLoading(pDevice, pDevice_Context);

	if (FAILED(pInstance->NativeConstruct(eNextSceneID)))
	{
		MSG_BOX("Failed to Creating Instance (CLoading) ");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CLoading::Free()
{
	Safe_Release(m_pGameInstance);
	Safe_Release(m_pDevice);
	Safe_Release(m_pDevice_Context);

	WaitForSingleObject(m_hThread, INFINITE);

	DeleteCriticalSection(&m_CriticalSection);

	CloseHandle(m_hThread);
}
