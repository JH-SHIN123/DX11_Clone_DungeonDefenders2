#include "..\public\Cell.h"

CCell::CCell(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: m_pDevice(pDevice)
	, m_pDevice_Context(pDevice_Context)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pDevice_Context);
}

HRESULT CCell::NativeContruct(const _float3 * pPoints)
{
	memcpy(m_vPoints, pPoints, sizeof(_float3) * 3);

	return S_OK;
}

CCell * CCell::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context, const _float3 * pPoints)
{
	CCell*		pInstance = new CCell(pDevice, pDevice_Context);
	if (FAILED(pInstance->NativeContruct(pPoints)))
	{
		MSG_BOX("Failed to Creating Instance (CCell) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CCell::Free()
{
	Safe_Release(m_pDevice_Context);
	Safe_Release(m_pDevice);
}
