#include "..\public\Navigation.h"

CNavigation::CNavigation(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: CComponent(pDevice, pDevice_Context)
{
}

CNavigation::CNavigation(const CNavigation & rhs)
	: CComponent(rhs)
{
}

HRESULT CNavigation::NativeConstruct_Prototype(const _tchar * pNavigationDataFile)
{
	return S_OK;
}

HRESULT CNavigation::NativeConstruct(void * pArg)
{
	return S_OK;
}

CNavigation * CNavigation::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context, const _tchar * pNavigationDataFile)
{
	return nullptr;
}

CComponent * CNavigation::Clone(void * pArg)
{
	return nullptr;
}

void CNavigation::Free()
{
}
