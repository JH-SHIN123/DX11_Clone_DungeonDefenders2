#include "stdafx.h"
#include "..\public\Level_Logo.h"
#include "GameInstance.h"
#include "Level_Loading.h"

CLevel_Logo::CLevel_Logo(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: CLevel(pDevice, pDevice_Context)
{

}

HRESULT CLevel_Logo::NativeConstruct()
{
	CLevel::NativeConstruct();

	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;

	return S_OK;
}

_int CLevel_Logo::Tick(_double Timedelta)
{
	CLevel::Tick(Timedelta);

	if (GetKeyState(VK_RETURN) < 0)
	{
		CGameInstance*		pGameInstance = CGameInstance::GetInstance();
		if (nullptr == pGameInstance)
			return -1;

		if (FAILED(pGameInstance->SetUp_CurrentLevel(CLevel_Loading::Create(m_pDevice, m_pDevice_Context, ELevel::Stage1))))
			return -1;

		pGameInstance->Clear_This_Level((_uint)ELevel::Logo);
	}

	return 0;
}

HRESULT CLevel_Logo::Render()
{
#ifdef _DEBUG
	SetWindowText(g_hWnd, TEXT("지금 이거 로고씬."));
#endif // _DEBUG




	return S_OK;
}

HRESULT CLevel_Logo::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	pGameInstance->Add_GameObject((_uint)ELevel::Logo, TEXT("Prototype_BackGround_Logo"), (_uint)ELevel::Logo, pLayerTag);

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
