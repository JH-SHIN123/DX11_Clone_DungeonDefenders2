#include "stdafx.h"
#include "..\public\Archer_Arrow.h"

CArcher_Arrow::CArcher_Arrow(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: CBullet(pDevice, pDevice_Context)
{
}

CArcher_Arrow::CArcher_Arrow(const CArcher_Arrow & rhs)
	: CBullet(rhs)
{
}

HRESULT CArcher_Arrow::NativeConstruct_Prototype()
{
	return E_NOTIMPL;
}

HRESULT CArcher_Arrow::NativeConstruct(void * pArg)
{
	return E_NOTIMPL;
}

_int CArcher_Arrow::Tick(_float TimeDelta)
{
	return _int();
}

_int CArcher_Arrow::Late_Tick(_float TimeDelta)
{
	return _int();
}

HRESULT CArcher_Arrow::Render()
{
	return E_NOTIMPL;
}

HRESULT CArcher_Arrow::Ready_Component(void * pArg)
{
	return E_NOTIMPL;
}

CArcher_Arrow * CArcher_Arrow::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
{
	return nullptr;
}

CGameObject * CArcher_Arrow::Clone_GameObject(void * pArg)
{
	return nullptr;
}

void CArcher_Arrow::Free()
{
}
