#include "stdafx.h"
#include "..\public\Level_Stage1.h"
#include "GameInstance.h"
#include "Loading.h"
#include "Camera_Free.h"
#include "UI_2D.h"

CLevel_Stage1::CLevel_Stage1(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: CLevel(pDevice, pDevice_Context)
{

}

HRESULT CLevel_Stage1::NativeConstruct()
{
	CLevel::NativeConstruct();

	if (FAILED(Ready_Layer_Terrain(TEXT("Layer_Terrain"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_UI(TEXT("Layer_UI"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera_Free(TEXT("Layer_Camera_Free"))))
		return E_FAIL;
		

	return S_OK;
}

_int CLevel_Stage1::Tick(_float Timedelta)
{
	CLevel::Tick(Timedelta);


	return 0;
}

HRESULT CLevel_Stage1::Render()
{
#ifdef _DEBUG
	SetWindowText(g_hWnd, TEXT("지금 이거 로고씬."));
#endif // _DEBUG




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
	pGameInstance->Add_GameObject((_uint)ELevel::Stage1, TEXT("Prototype_StatusPanel"), (_uint)ELevel::Stage1, L"Layer_FrameUI");

	// X 67 Y 67
	// 스킬창
	UI2D_DESC UI_Desc;
	lstrcpy(UI_Desc.szTextureName, L"Component_Texture_SkillMage");
	UI_Desc.vPos = _float2(-435.2f, -211.f);
	UI_Desc.vScale = _float2(64.f, 64.f);
	pGameInstance->Add_GameObject((_uint)ELevel::Stage1, TEXT("Prototype_PlayerSkill_UI"), (_uint)ELevel::Stage1, L"Layer_UI", &UI_Desc);

	// HPMP따로 만듬
	lstrcpy(UI_Desc.szTextureName, L"Component_Texture_HpMp");
	UI_Desc.vPos = _float2(-581.f, -213.f);
	UI_Desc.vScale = _float2(64.f, 128.f);
	pGameInstance->Add_GameObject((_uint)ELevel::Stage1, TEXT("Prototype_Player_HpMp"), (_uint)ELevel::Stage1, L"Layer_UI_HpMp", &UI_Desc);

	// 해당 스테이지의 정보를 보여줌
	lstrcpy(UI_Desc.szTextureName, L"Component_Texture_Panel_Level");
	UI_Desc.vPos = _float2(510.f, -305.f);
	UI_Desc.vScale = _float2(256.f, 100.f);
	pGameInstance->Add_GameObject((_uint)ELevel::Stage1, TEXT("Prototype_WaveInfo"), (_uint)ELevel::Stage1, L"Layer_WaveInfo", &UI_Desc);



	return S_OK;
}

HRESULT CLevel_Stage1::Ready_Layer_Camera_Free(const _tchar * pLayerTag)
{
	CGameInstance* pGameInstance = GET_GAMEINSTANCE;

	CAMERA_DESC			CameraDesc;
	CameraDesc.vEye = _float3(0.f, 10.f, -7.f);
	CameraDesc.vAt = _float3(0.f, 0.f, 0.f);
	CameraDesc.vAxisY = _float3(0.f, 1.f, 0.f);

	CameraDesc.fAspect = _float(g_iWinCX) / g_iWinCY;
	CameraDesc.fFovy = XMConvertToRadians(90.0f);
	CameraDesc.fNear = 0.3f;
	CameraDesc.fFar = 300.f;

	CameraDesc.StateDesc.fSpeedPerSec = 100.f;
	CameraDesc.StateDesc.fRotatePerSec = XMConvertToRadians(90.0f);

	pGameInstance->Add_GameObject((_uint)ELevel::Stage1, TEXT("Prototype_Camera_Free"), (_uint)ELevel::Stage1, pLayerTag, &CameraDesc);

	return S_OK;
}

HRESULT CLevel_Stage1::Ready_Layer_Player(const _tchar * pLayerTag)
{
	CGameInstance* pGameInstance = GET_GAMEINSTANCE;

	MOVESTATE_DESC Data;
	Data.vPos = _float4(10.f, 0.f, 10.f, 1.f);
	Data.vScale = _float4(10.f, 10.f, 10.f, 1.f);

	pGameInstance->Add_GameObject((_uint)ELevel::Stage1, TEXT("Prototype_Player"), (_uint)ELevel::Stage1, pLayerTag, &Data);

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
