#include "stdafx.h"
#include "..\public\Skill_Meteor.h"

CSkill_Meteor::CSkill_Meteor(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: CBullet(pDevice, pDevice_Context)
{
}

CSkill_Meteor::CSkill_Meteor(const CSkill_Meteor & rhs)
	: CBullet(rhs)
{
}

HRESULT CSkill_Meteor::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CSkill_Meteor::NativeConstruct(void * pArg)
{
	__super::NativeConstruct(pArg);

	Ready_Component(pArg);

	Set_Pivot(XMVectorSet(0.05f, 0.05f, 0.05f, 0.f));

	return S_OK;
}

_int CSkill_Meteor::Tick(_float TimeDelta)
{
	if (__super::Tick(TimeDelta))
	{

		return OBJECT_DEAD;
	}

	if (true == m_IsFall)
	{
		// 이거 몬스터 방향으로 
		m_pMovementCom->Go_Straight(TimeDelta * 13.f);

	}

	else
	{
		m_fCastTime -= TimeDelta;

		if (0.f >= m_fCastTime)
			m_IsFall = true;
	}

	if (1) // 부딫혔다면
	{
		//펑
	}

	return 0;
}

_int CSkill_Meteor::Late_Tick(_float TimeDelta)
{
	return __super::Late_Tick(TimeDelta);
}

HRESULT CSkill_Meteor::Render()
{
	__super::Render();

	return S_OK;
}

HRESULT CSkill_Meteor::Ready_Component(void * pArg)
{
	HRESULT hr = S_OK;

	COLLIDER_DESC Data;
	Data.vScale = { 2.f, 2.f, 2.f };

	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Collider_Sphere"), TEXT("Com_Collider"), (CComponent**)&m_pColliderCom, &Data);

	if (S_OK != hr)
		MSG_BOX("CSkill_Meteor::Ready_Component");

	return hr;
}

CSkill_Meteor * CSkill_Meteor::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
{
	CSkill_Meteor* pInstance = new CSkill_Meteor(pDevice, pDevice_Context);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX("Failed to Creating Instance (CSkill_Meteor) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CSkill_Meteor::Clone_GameObject(void * pArg)
{
	CSkill_Meteor* pInstance = new CSkill_Meteor(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Creating clone (CSkill_Meteor) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CSkill_Meteor::Free()
{
	__super::Free();
}
