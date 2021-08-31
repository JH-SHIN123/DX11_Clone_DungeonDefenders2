#include "stdafx.h"
#include "..\public\Monster.h"

CMonster::CMonster(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: CGameObject(pDevice, pDevice_Context)
{
}

CMonster::CMonster(const CMonster & rhs)
	: CGameObject(rhs)
{
}

HRESULT CMonster::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CMonster::NativeConstruct(void * pArg)
{
	return S_OK;
}

_int CMonster::Tick(_float TimeDelta)
{
	return _int();
}

_int CMonster::Late_Tick(_float TimeDelta)
{
	return _int();
}

HRESULT CMonster::Render()
{
	return S_OK;
}

HRESULT CMonster::Ready_Component(void * pArg)
{
	return S_OK;
}

CMonster * CMonster::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
{
	CMonster* pInstance = new CMonster(pDevice, pDevice_Context);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX("Failed to Creating Instance (CMonster) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CMonster::Clone_GameObject(void * pArg)
{
	CMonster* pInstance = new CMonster(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Creating clone (CMonster) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMonster::Free()
{
	__super::Free();
}
