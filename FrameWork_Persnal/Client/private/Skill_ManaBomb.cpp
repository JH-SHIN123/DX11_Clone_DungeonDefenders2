#include "stdafx.h"
#include "..\public\Skill_ManaBomb.h"

CSkill_ManaBomb::CSkill_ManaBomb(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: CBullet(pDevice, pDevice_Context)
{
}

CSkill_ManaBomb::CSkill_ManaBomb(const CSkill_ManaBomb & rhs)
	: CBullet(rhs)
{
}

HRESULT CSkill_ManaBomb::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CSkill_ManaBomb::NativeConstruct(void * pArg)
{
	return S_OK;
}

_int CSkill_ManaBomb::Tick(_float TimeDelta)
{
	return _int();
}

_int CSkill_ManaBomb::Late_Tick(_float TimeDelta)
{
	return _int();
}

HRESULT CSkill_ManaBomb::Render()
{
	return S_OK;
}

CSkill_ManaBomb * CSkill_ManaBomb::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
{
	CSkill_ManaBomb* pInstance = new CSkill_ManaBomb(pDevice, pDevice_Context);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX("Failed to Creating Instance (CSkill_ManaBomb) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CSkill_ManaBomb::Clone_GameObject(void * pArg)
{
	CGameObject* pInstance = new CGameObject(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Creating clone (CGameObject) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CSkill_ManaBomb::Free()
{
	__super::Free();
}
