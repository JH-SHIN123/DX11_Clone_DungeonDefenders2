#include "stdafx.h"
#include "..\public\Boss_Hand.h"

CBoss_Hand::CBoss_Hand(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: CMonster(pDevice, pDevice_Context)
{
}

CBoss_Hand::CBoss_Hand(const CBoss_Hand & rhs)
	: CMonster(rhs)
{
}

HRESULT CBoss_Hand::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CBoss_Hand::NativeConstruct(void * pArg)
{
	return S_OK;
}

_int CBoss_Hand::Tick(_float TimeDelta)
{
	return _int();
}

_int CBoss_Hand::Late_Tick(_float TimeDelta)
{
	return _int();
}

HRESULT CBoss_Hand::Render()
{
	return E_NOTIMPL;
}

CBoss_Hand * CBoss_Hand::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
{
	return nullptr;
}

CGameObject * CBoss_Hand::Clone_GameObject(void * pArg)
{
	return nullptr;
}

void CBoss_Hand::Free()
{
}
