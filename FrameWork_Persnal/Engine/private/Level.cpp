#include "..\public\Level.h"

CLevel::CLevel(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: m_pDevice(pDevice)
	, m_pDevice_Context(pDevice_Context)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pDevice_Context);
}

HRESULT CLevel::NativeConstruct()
{
	return S_OK;
}

int CLevel::Tick(_double Timedelta)
{
	return 0;
}

HRESULT CLevel::Render()
{
	return S_OK;
}

void CLevel::Free()
{
	Safe_Release(m_pDevice);
	Safe_Release(m_pDevice_Context);
}
