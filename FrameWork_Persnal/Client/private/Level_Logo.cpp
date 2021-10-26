#include "stdafx.h"
#include "..\public\Level_Logo.h"
#include "GameInstance.h"
#include "Level_Loading.h"
#include "MainMenu.h"
#include "Data_Manager.h"

CLevel_Logo::CLevel_Logo(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: CLevel(pDevice, pDevice_Context)
{

}

HRESULT CLevel_Logo::NativeConstruct()
{
	CSound_Manager::GetInstance()->StopSoundAll();

	CLevel::NativeConstruct();

	GET_GAMEINSTANCE->Clear_This_Level((_uint)ELevel::Loading);

	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;

	Ready_Layer_MainMenu(L"Layer_MainMenu");



	GET_GAMEINSTANCE->Add_GameObject((_uint)ELevel::Static, L"Prototype_Fade", (_uint)ELevel::Logo, L"Layer_Fade");
	CSound_Manager::GetInstance()->PlayBGM(L"Menu.mp3", CHANNEL_BGM);

	return S_OK;
}

_int CLevel_Logo::Tick(_float Timedelta)
{
	CLevel::Tick(Timedelta);
	CSound_Manager::GetInstance()->Set_Volume(CHANNEL_BGM, CData_Manager::GetInstance()->Get_SoundVolume_BGM());

	return 0;
}

HRESULT CLevel_Logo::Render()
{
#ifdef _DEBUG
	//SetWindowText(g_hWnd, TEXT("지금 이거 로고씬."));
#endif // _DEBUG

	if (m_IsChange == true)
	{
		CGameInstance* pGameInstance = GET_GAMEINSTANCE;

		if (nullptr == pGameInstance)
			return -1;

		if (FAILED(pGameInstance->SetUp_CurrentLevel(CLevel_Loading::Create(m_pDevice, m_pDevice_Context, m_eNextLevel))))
			return -1;

		pGameInstance->Clear_This_Level((_uint)ELevel::Logo);
	}

	return S_OK;
}

void CLevel_Logo::Scene_Change(ELevel eLevel)
{
	m_IsChange = true;
	m_eNextLevel = eLevel;
}

HRESULT CLevel_Logo::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	//pGameInstance->Add_GameObject((_uint)ELevel::Logo, TEXT("Prototype_BackGround_Logo"), (_uint)ELevel::Logo, pLayerTag);

	return S_OK;
}

HRESULT CLevel_Logo::Ready_Layer_MainMenu(const _tchar * pLayerTag)
{
	CGameInstance* pGameInstance = GET_GAMEINSTANCE;

	UI2D_DESC UI_Desc;

	lstrcpy(UI_Desc.szTextureName, L"Component_Texture_Logo");
	UI_Desc.Movement_Desc.vPos = _float4(0.2f, 50.f, 0.f, 1.f);
	UI_Desc.Movement_Desc.vScale = _float4(512.f, 256.f, 0.f, 0.f);
	UI_Desc.eLevel = ELevel::Logo;
	pGameInstance->Add_GameObject((_uint)ELevel::Logo, TEXT("Prototype_MainMenu"), (_uint)ELevel::Logo, L"Layer_UI", &UI_Desc);


	return S_OK;
}

CLevel_Logo * CLevel_Logo::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
{
	CLevel_Logo*		pInstance = new CLevel_Logo(pDevice, pDevice_Context);

	if (FAILED(pInstance->NativeConstruct()))
	{
		MSG_BOX("Failed to Creating Instance (CLevel_Logo) ");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Logo::Free()
{
	CLevel::Free();


}
