#include "stdafx.h"
#include "..\public\Level_Stage1.h"
#include "GameInstance.h"
#include "Loading.h"
#include "Camera_Free.h"
#include "UI_2D.h"
#include "Data_Manager.h"
#include "Player.h"
#include "DefenceTower.h"
#include "Monster.h"

CLevel_Stage1::CLevel_Stage1(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: CLevel(pDevice, pDevice_Context)
{

}

HRESULT CLevel_Stage1::NativeConstruct()
{
	CLevel::NativeConstruct();

	if (FAILED(Ready_Light()))
		return E_FAIL;

	//if (FAILED(Ready_Layer_Terrain(TEXT("Layer_Terrain"))))
	//	return E_FAIL;

	if (FAILED(Ready_Layer_UI(TEXT("Layer_UI"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera_Free(TEXT("Layer_Camera_Free"))))
		return E_FAIL;
	
	if (FAILED(Ready_Layer_Player(L"Layer_Player")))
		return E_FAIL;
		

	// Test
	TOWER_DESC Data;
	lstrcpy(Data.szModelName, L"Component_Mesh_BlockcadeTower");
	Data.eTowerRange = ETowerRange::Quarter;
	Data.MoveState_Desc.fRotatePerSec = 0.5f;
	Data.MoveState_Desc.vPos = { -10.f,0.f,20.f,1.f };
	Data.MoveState_Desc.vScale = _float4(1.f, 1.f, 1.f, 0.f); 
	GET_GAMEINSTANCE->Add_GameObject((_uint)ELevel::Stage1, L"Prototype_BlockadeTower", (_uint)ELevel::Stage1, L"Layer_Tower", &Data);


	lstrcpy(Data.szModelName, L"Component_Mesh_StrikerTower");
	Data.eTowerRange = ETowerRange::Quarter;
	Data.MoveState_Desc.fRotatePerSec = 0.5f;
	Data.MoveState_Desc.vPos = { 0.f,0.f,20.f,1.f };
	Data.MoveState_Desc.vScale = _float4(1.f, 1.f, 1.f, 0.f);
	GET_GAMEINSTANCE->Add_GameObject((_uint)ELevel::Stage1, L"Prototype_StrikerTower", (_uint)ELevel::Stage1, L"Layer_Tower", &Data);


	lstrcpy(Data.szModelName, L"Component_Mesh_LightningTower");
	Data.eTowerRange = ETowerRange::Quarter;
	Data.MoveState_Desc.fRotatePerSec = 0.5f;
	Data.MoveState_Desc.vPos = { 10.f,0.f,20.f,1.f };
	Data.MoveState_Desc.vScale = _float4(1.f, 1.f, 1.f, 0.f);
	GET_GAMEINSTANCE->Add_GameObject((_uint)ELevel::Stage1, L"Prototype_LightningTower", (_uint)ELevel::Stage1, L"Layer_Tower", &Data);


	MONSTER_DESC MonData;
	lstrcpy(MonData.szModelName, L"Component_Mesh_Goblin");
	MonData.eLevel = ELevel::Stage1;
	MonData.Movement_Desc.vPos = { 0.f, 0.f, 30.f, 1.f };
	MonData.Movement_Desc.vScale = { 1.f, 1.f, 1.f, 0.f };
	GET_GAMEINSTANCE->Add_GameObject((_uint)ELevel::Stage1, L"Prototype_Goblin", (_uint)ELevel::Stage1, L"Layer_Monster", &MonData);


	return S_OK;
}

_int CLevel_Stage1::Tick(_float Timedelta)
{
	CLevel::Tick(Timedelta);

	if (GET_KEY_INPUT(DIK_F1) && true == CData_Manager::GetInstance()->Get_Tick_Stop())
	{

		CData_Manager::GetInstance()->Set_Tick_Stop(false);
	}

	if (GET_KEY_INPUT(DIK_F2) && false == CData_Manager::GetInstance()->Get_Tick_Stop())
	{
		// 탭키 퍼즈
		UI2D_DESC UI_Desc;

		lstrcpy(UI_Desc.szTextureName, L"Component_Texture_PauseMenu");
		UI_Desc.Movement_Desc.vPos = _float4(0.f, 50.f, 0.f, 1.f);
		UI_Desc.Movement_Desc.vScale = _float4(512.f, 512.f, 0.f, 0.f);
		UI_Desc.eLevel = ELevel::Static;
		GET_GAMEINSTANCE->Add_GameObject((_uint)ELevel::Static, TEXT("Prototype_PauseMenu"), (_uint)ELevel::Stage1, L"Layer_Pause", &UI_Desc);

		CData_Manager::GetInstance()->Set_Tick_Stop(true);
	}

	if (GET_KEY_INPUT(DIK_F3))
		CData_Manager::GetInstance()->Set_NowPhase(EPhaseState::Build);

	if (GET_KEY_INPUT(DIK_F4))
		CData_Manager::GetInstance()->Set_NowPhase(EPhaseState::Combat);

	return 0;
}

HRESULT CLevel_Stage1::Render()
{
#ifdef _DEBUG
	SetWindowText(g_hWnd, TEXT("지금 이거 로고씬."));
#endif // _DEBUG




	return S_OK;
}

HRESULT CLevel_Stage1::Ready_Light()
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	if (nullptr == pGameInstance)
		return E_FAIL;

	Safe_AddRef(pGameInstance);

	pGameInstance->Reserve_Container_Light(2);

	/* For.Directional */
	LIGHT_DESC			LightDesc;
	LightDesc.vDirection = XMFLOAT3(1.f, -1.f, 1.f);
	LightDesc.vDiffuse = XMFLOAT4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vAmbient = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.f);
	LightDesc.vSpecular = XMFLOAT4(1.f, 1.f, 1.f, 1.f);

	if (FAILED(pGameInstance->Add_Light(m_pDevice, m_pDevice_Context, LightDesc)))
		return E_FAIL;

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLevel_Stage1::Ready_Layer_Terrain(const _tchar * pLayerTag)
{
	CGameInstance* pGameInstance = GET_GAMEINSTANCE;

	pGameInstance->Add_GameObject((_uint)ELevel::Stage1, TEXT("Prototype_Terrain"), (_uint)ELevel::Stage1, pLayerTag);

	return S_OK;
}

HRESULT CLevel_Stage1::Ready_Layer_UI(const _tchar * pLayerTag)
{
	CGameInstance* pGameInstance = GET_GAMEINSTANCE;

	// 스킬창 프레임
	pGameInstance->Add_GameObject((_uint)ELevel::Static, TEXT("Prototype_StatusPanel"), (_uint)ELevel::Stage1, L"Layer_FrameUI");

	// X 67 Y 67
	// 스킬창
	UI2D_DESC UI_Desc;
	lstrcpy(UI_Desc.szTextureName, L"Component_Texture_SkillMage");
	UI_Desc.Movement_Desc.vPos = _float4(-435.2f, -211.f, 0.f, 1.f);
	UI_Desc.Movement_Desc.vScale = _float4(64.f, 64.f, 0.f, 0.f);
	pGameInstance->Add_GameObject((_uint)ELevel::Static, TEXT("Prototype_PlayerSkill_UI"), (_uint)ELevel::Stage1, L"Layer_UI", &UI_Desc);

	// HPMP따로 만듬
	lstrcpy(UI_Desc.szTextureName, L"Component_Texture_HpMp");
	UI_Desc.Movement_Desc.vPos = _float4(-581.f, -213.f, 0.f, 1.f);
	UI_Desc.Movement_Desc.vScale = _float4(64.f, 128.f, 0.f, 0.f);
	pGameInstance->Add_GameObject((_uint)ELevel::Static, TEXT("Prototype_Player_HpMp"), (_uint)ELevel::Stage1, L"Layer_UI_HpMp", &UI_Desc);

	// 해당 스테이지의 정보를 보여줌
	lstrcpy(UI_Desc.szTextureName, L"Component_Texture_Panel_Level");
	UI_Desc.Movement_Desc.vPos = _float4(510.f, -305.f, 0.f, 1.f);
	UI_Desc.Movement_Desc.vScale = _float4(256.f, 100.f, 0.f, 0.f);
	pGameInstance->Add_GameObject((_uint)ELevel::Static, TEXT("Prototype_PhaseInfo"), (_uint)ELevel::Stage1, L"Layer_PhaseInfo", &UI_Desc);



	return S_OK;
}

HRESULT CLevel_Stage1::Ready_Layer_Camera_Free(const _tchar * pLayerTag)
{
	CGameInstance* pGameInstance = GET_GAMEINSTANCE;

	CAMERA_DESC			CameraDesc;
	CameraDesc.vEye = _float3(0.f, 10.f, -7.f);
	CameraDesc.vAt = _float3(0.f, -5.f, 0.f);
	CameraDesc.vAxisY = _float3(0.f, 1.f, 0.f);

	CameraDesc.vTargetAxis = _float3(0.3f, 0.9f, -1.1f);
	CameraDesc.fXRotationLock_Min = 70.f;
	CameraDesc.fXRotationLock_Max = 155.f;
	CameraDesc.fDis = 10.f;

	CameraDesc.fAspect = _float(g_iWinCX) / g_iWinCY;
	CameraDesc.fFovy = XMConvertToRadians(90.0f);
	CameraDesc.fNear = 0.3f;
	CameraDesc.fFar = 430.f;

	CameraDesc.StateDesc.fSpeedPerSec = 50.f;
	CameraDesc.StateDesc.fRotatePerSec = XMConvertToRadians(90.0f);

	//pGameInstance->Add_GameObject((_uint)ELevel::Static, TEXT("Prototype_Camera_Free"), (_uint)ELevel::Stage1, pLayerTag, &CameraDesc);
	CameraDesc.vAt = _float3(5.f, 5.f, 10.f);
	pGameInstance->Add_GameObject((_uint)ELevel::Static, TEXT("Prototype_Camera_Target"), (_uint)ELevel::Stage1, pLayerTag, &CameraDesc);

	return S_OK;
}

HRESULT CLevel_Stage1::Ready_Layer_Player(const _tchar * pLayerTag)
{
	CGameInstance* pGameInstance = GET_GAMEINSTANCE;

	GAMEOBJ_DESC Data;
	Data.Movement_Desc.vPos = _float4(0.f, 0.f, 0.f, 1.f);
	Data.Movement_Desc.vScale = _float4(1.f, 1.f, 1.f, 0.f);
	Data.Movement_Desc.fSpeedPerSec = 20.f;

	Data.Status_Desc.fAttSpeed = 2.f;
	Data.Status_Desc.fCritical = 10.f;
	Data.Status_Desc.iAtt_Basic = 50;
	Data.Status_Desc.iExp = 0;
	Data.Status_Desc.iExp_Max = 100;
	Data.Status_Desc.iHp_Max = 300;
	Data.Status_Desc.iLevel = 1;
	Data.Status_Desc.iMp_Max = 100;

	lstrcpy(Data.szModelName, L"Component_Mesh_Mage");

	pGameInstance->Add_GameObject((_uint)ELevel::Static, TEXT("Prototype_Player"), (_uint)ELevel::Stage1, pLayerTag, &Data);

	return S_OK;
}

CLevel_Stage1 * CLevel_Stage1::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
{
	CLevel_Stage1*		pInstance = new CLevel_Stage1(pDevice, pDevice_Context);

	if (FAILED(pInstance->NativeConstruct()))
	{
		MSG_BOX("Failed to Creating Instance (CLevel_Stage1) ");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Stage1::Free()
{
	CLevel::Free();
}
