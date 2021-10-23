#include "stdafx.h"
#include "..\public\Effect_Blood.h"

CEffect_Blood::CEffect_Blood(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: CGameObject(pDevice, pDevice_Context)
{
}

CEffect_Blood::CEffect_Blood(const CEffect_Blood & rhs)
	: CGameObject(rhs)
{
}

HRESULT CEffect_Blood::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CEffect_Blood::NativeConstruct(void * pArg)
{
	return S_OK;
}

_int CEffect_Blood::Tick(_float TimeDelta)
{

	return _int();
}

_int CEffect_Blood::Late_Tick(_float TimeDelta)
{
	BillBorad();


	return 0;
}

HRESULT CEffect_Blood::Render()
{
	return S_OK;
}

HRESULT CEffect_Blood::Ready_Component(void * pArg)
{
	return S_OK;
}

void CEffect_Blood::BillBorad()
{

}

CEffect_Blood * CEffect_Blood::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
{
	return nullptr;
}

CGameObject * CEffect_Blood::Clone_GameObject(void * pArg)
{
	return nullptr;
}

void CEffect_Blood::Free()
{
}
