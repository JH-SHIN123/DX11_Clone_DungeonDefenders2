#include "..\public\Component.h"

CComponent::CComponent(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: m_pDevice(pDevice)
	, m_pDevice_Context(pDevice_Context)
	, m_IsClone(false)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pDevice_Context);
}

CComponent::CComponent(const CComponent & rhs)
	: m_pDevice(rhs.m_pDevice)
	, m_pDevice_Context(rhs.m_pDevice_Context)
	, m_IsClone(true)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pDevice_Context);
}

HRESULT CComponent::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CComponent::NativeConstruct(void * pArg)
{
	return S_OK;
}

void CComponent::Free()
{
	Safe_Release(m_pDevice_Context);
	Safe_Release(m_pDevice);
}
