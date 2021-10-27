#include "..\public\Light_Manager.h"
#include "Light.h"

IMPLEMENT_SINGLETON(CLight_Manager)

CLight_Manager::CLight_Manager()
{
}

HRESULT CLight_Manager::Reserve_LightManager(_int iNumLight)
{
	m_Lights.reserve(iNumLight);



	return S_OK;
}

HRESULT CLight_Manager::Add_Light(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context, const LIGHT_DESC & LightDesc, _bool isActive)
{
	if (nullptr == pDevice ||
		nullptr == pDevice_Context)
		return E_FAIL;

	CLight*		pLight = CLight::Create(pDevice, pDevice_Context, LightDesc, isActive);
	if (nullptr == pLight)
		return E_FAIL;

	m_Lights.push_back(pLight);

	return S_OK;
}

const LIGHT_DESC & CLight_Manager::Get_LightDesc(_int iLightIndex) const
{
	return m_Lights[iLightIndex]->Get_LightDesc();
}

HRESULT CLight_Manager::Render_Lights()
{

	for (auto& pLight : m_Lights)
		pLight->Render_Light();

	return S_OK;
}

void CLight_Manager::Free()
{
	for (auto& pLight : m_Lights)
		Safe_Release(pLight);

	m_Lights.clear();
}
