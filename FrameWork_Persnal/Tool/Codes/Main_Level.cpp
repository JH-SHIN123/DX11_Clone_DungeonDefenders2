#include "stdafx.h"
#include "Main_Level.h"

CMain_Level::CMain_Level(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: CLevel(pDevice, pDevice_Context)
{
}

HRESULT CMain_Level::NativeConstruct()
{
	return S_OK;
}

_int CMain_Level::Tick(_float Timedelta)
{
	return _int();
}

HRESULT CMain_Level::Render()
{
	return S_OK;
}

HRESULT CMain_Level::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	return S_OK;
}

CMain_Level * CMain_Level::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
{
	CMain_Level*		pInstance = new CMain_Level(pDevice, pDevice_Context);

	if (FAILED(pInstance->NativeConstruct()))
	{
		MSG_BOX("Failed to Creating Instance (CLevel_Logo) ");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMain_Level::Free()
{
	CLevel::Free();
}
