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
	__super::NativeConstruct(pArg);

	Ready_Component(pArg);

	Set_Pivot(XMVectorSet(0.01f, 0.01f, 0.01f, 0.f));

	return S_OK;
}

_int CSkill_ManaBomb::Tick(_float TimeDelta)
{
	if (__super::Tick(TimeDelta))
	{

		return OBJECT_DEAD;
	}

	if (true == m_IsFall)
	{
		m_pMovementCom->Go_Up(-TimeDelta * 13.f);

	}

	else
	{
		m_fCastTime -= TimeDelta;
		m_pMovementCom->Go_Up(TimeDelta);
		
		if (0.f >= m_fCastTime)
			m_IsFall = true;
	}

	if (1) // ºÎ‹HÇû´Ù¸é
	{
		//Æã
	}

	return 0;
}

_int CSkill_ManaBomb::Late_Tick(_float TimeDelta)
{
	return __super::Late_Tick(TimeDelta);
}

HRESULT CSkill_ManaBomb::Render()
{
	__super::Render();

	return S_OK;
}

HRESULT CSkill_ManaBomb::Ready_Component(void * pArg)
{
	HRESULT hr = S_OK;



	if (S_OK != hr)
		MSG_BOX("CSkill_ManaBomb::Ready_Component");

	return hr;
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
	CSkill_ManaBomb* pInstance = new CSkill_ManaBomb(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Creating clone (CSkill_ManaBomb) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CSkill_ManaBomb::Free()
{
	__super::Free();
}
