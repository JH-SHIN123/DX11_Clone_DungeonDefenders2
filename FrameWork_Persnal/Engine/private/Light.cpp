#include "..\public\Light.h"

CLight::CLight(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: m_pDevice(pDevice)
	, m_pDevice_Context(pDevice_Context)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pDevice_Context);
}

HRESULT CLight::Native_Construct(const LIGHT_DESC & LightDesc)
{
	m_LightDesc = LightDesc;
	m_LightDesc.isActive = true;

	return S_OK;
}

CLight * CLight::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context, const LIGHT_DESC & LightDesc)
{
	CLight* pInstance = new CLight(pDevice, pDevice_Context);
	if (FAILED(pInstance->Native_Construct(LightDesc)))
	{
		MSG_BOX("CLight::Create Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CLight::Free()
{
	Safe_Release(m_pDevice_Context);
	Safe_Release(m_pDevice);
}
