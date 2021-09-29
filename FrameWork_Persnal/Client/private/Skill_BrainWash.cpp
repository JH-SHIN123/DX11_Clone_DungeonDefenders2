#include "stdafx.h"
#include "..\public\Skill_BrainWash.h"

CSkill_BrainWash::CSkill_BrainWash(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: CBullet(pDevice, pDevice_Context)
{
}

CSkill_BrainWash::CSkill_BrainWash(const CSkill_BrainWash & rhs)
	: CBullet(rhs)
{
}

HRESULT CSkill_BrainWash::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CSkill_BrainWash::NativeConstruct(void * pArg)
{
	__super::NativeConstruct(pArg);

	Ready_Component(pArg);

	Set_Pivot(XMVectorSet(0.01f, 0.01f, 0.01f, 0.f));

	return S_OK;
}

_int CSkill_BrainWash::Tick(_float TimeDelta)
{
	_int iReturn = 0;
	if (iReturn = __super::Tick(TimeDelta))
		return iReturn;


	_vector vDir = XMLoadFloat3(&m_vGoDir);
	m_pMovementCom->Go_Dir_Vector(TimeDelta, vDir);



	return iReturn;
}

_int CSkill_BrainWash::Late_Tick(_float TimeDelta)
{
	if (true == m_pColliderCom_Attack->Get_IsCollide())
		return OBJECT_DEAD;

	return __super::Late_Tick(TimeDelta);
}

HRESULT CSkill_BrainWash::Render()
{
	__super::Render();


#ifdef _DEBUG
	m_pColliderCom_Attack->Render_Collider();
#endif // _DEBUG


	return S_OK;
}

HRESULT CSkill_BrainWash::Ready_Component(void * pArg)
{
	HRESULT hr = S_OK;



	if (S_OK != hr)
		MSG_BOX("CSkill_BrainWash::Ready_Component Failed!");

	return hr;
}

CSkill_BrainWash * CSkill_BrainWash::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
{
	CSkill_BrainWash*		pInstance = new CSkill_BrainWash(pDevice, pDevice_Context);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX("Failed to Creating Instance (CSkill_BrainWash) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CSkill_BrainWash::Clone_GameObject(void * pArg)
{
	CSkill_BrainWash*		pInstance = new CSkill_BrainWash(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Cloned Instance (CSkill_BrainWash) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CSkill_BrainWash::Free()
{
	__super::Free();
}
