#include "..\public\Light_Manager.h"
#include "Light.h"

IMPLEMENT_SINGLETON(CLight_Manager)

CLight_Manager::CLight_Manager()
{
}

HRESULT CLight_Manager::Reserve_Container(_uint iNumLight)
{
	m_Lights.reserve(iNumLight);

	return S_OK;
}

HRESULT CLight_Manager::Add_Light(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context, const LIGHT_DESC & Light_Desc)
{
	if (nullptr == pDevice ||
		nullptr == pDevice_Context)
		return E_FAIL;

	CLight*		pLight = CLight::Create(pDevice, pDevice_Context, Light_Desc);
	if (nullptr == pLight)
		return E_FAIL;

	m_Lights.push_back(pLight);

	return S_OK;
}

LIGHT_DESC* CLight_Manager::Get_LightDesc(_int iLightIndex) 
{
	if (m_Lights.size() <= iLightIndex)
		return nullptr;

	return m_Lights[iLightIndex]->Get_LightDesc();
}

void CLight_Manager::Free()
{
	for (auto& pLight : m_Lights)
		Safe_Release(pLight);

	m_Lights.clear();
}
