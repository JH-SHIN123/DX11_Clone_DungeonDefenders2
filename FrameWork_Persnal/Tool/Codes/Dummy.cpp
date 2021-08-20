#include "stdafx.h"
#include "../Codes/Dummy.h"

CDummy::CDummy(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: CGameObject(pDevice, pDevice_Context)
{
}

CDummy::CDummy(const CGameObject & rhs)
	: CGameObject(rhs)
{
}

HRESULT CDummy::NativeConstruct_Prototype()
{
	CGameObject::NativeConstruct_Prototype();

	return S_OK;
}

HRESULT CDummy::NativeConstruct(void * pArg)
{
	CGameObject::NativeConstruct(pArg);

	return S_OK;
}

_int CDummy::Tick(_float TimeDelta)
{
	return _int();
}

_int CDummy::Late_Tick(_float TimeDelta)
{
	return _int();
}

HRESULT CDummy::Render()
{
	return S_OK;
}

HRESULT CDummy::Add_Component(_uint iLevelIndex, const _tchar * pPrototypeTag, const _tchar * pComponentTag, CComponent ** ppOut, void * pArg)
{
	return S_OK;
}

HRESULT CDummy::Ready_Component()
{
	return S_OK;
}

CDummy * CDummy::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
{
	CDummy* pInstance = new CDummy(pDevice, pDevice_Context);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX("Failed to Creating Instance (CDummy) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CDummy::Clone_GameObject(void * pArg)
{
	CDummy* pClone = new CDummy(*this);
	if (FAILED(pClone->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Creating Clone (CDummy) ");
		Safe_Release(pClone);
	}
	return pClone;
}

void CDummy::Free()
{
	CGameObject::Free();
}
