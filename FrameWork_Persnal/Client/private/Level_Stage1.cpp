#include "stdafx.h"
#include "..\public\Level_Stage1.h"
#include "GameInstance.h"
#include "Loading.h"

CLevel_Stage1::CLevel_Stage1(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: CLevel(pDevice, pDevice_Context)
{

}

HRESULT CLevel_Stage1::NativeConstruct()
{
	CLevel::NativeConstruct();

	if (FAILED(Ready_Layer_Terrain(TEXT("Layer_Terrain"))))
		return E_FAIL;

	return S_OK;
}

_int CLevel_Stage1::Tick(_double Timedelta)
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
	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	pGameInstance->Add_GameObject((_uint)ELevel::Stage1, TEXT("Prototype_Terrain"), (_uint)ELevel::Stage1, pLayerTag);

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
