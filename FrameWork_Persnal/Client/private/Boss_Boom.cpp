#include "stdafx.h"
#include "..\public\Boss_Boom.h"

CBoss_Boom::CBoss_Boom(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: CBullet(pDevice, pDevice_Context)
{
}

CBoss_Boom::CBoss_Boom(const CBoss_Boom & rhs)
	: CBullet(rhs)
{
}

HRESULT CBoss_Boom::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CBoss_Boom::NativeConstruct(void * pArg)
{
	__super::NativeConstruct(pArg);

	Ready_Component(pArg);

	Set_Pivot(XMVectorSet(0.01f, 0.01f, 0.01f, 0.f));

	return S_OK;
}

_int CBoss_Boom::Tick(_float TimeDelta)
{
	_int iReturn = 0;
	if (iReturn = __super::Tick(TimeDelta))
		return iReturn;




	//_vector vDir = XMLoadFloat3(&m_vGoDir);
	//m_pMovementCom->Go_Dir_Vector(TimeDelta, vDir);

	m_pColliderCom_Attack->Set_Scale_Tick_Linear(_float3(30.f, 30.f, 30.f), TimeDelta * 5.f);

	if (nullptr != m_pColliderCom_Attack)
	{
		if (true == m_pColliderCom_Attack->Get_IsCollide())
			return OBJECT_DEAD;

		m_pColliderCom_Attack->Update_Collider(m_pMovementCom->Get_WorldMatrix());
	}

	return iReturn;
}

_int CBoss_Boom::Late_Tick(_float TimeDelta)
{
	if (true == m_pColliderCom_Attack->Get_IsCollide())
		return OBJECT_DEAD;

	return __super::Late_Tick(TimeDelta);
}

HRESULT CBoss_Boom::Render()
{
	__super::Render();


#ifdef _DEBUG
	m_pColliderCom_Attack->Render_Collider();
#endif // _DEBUG


	return S_OK;
}

HRESULT CBoss_Boom::Ready_Component(void * pArg)
{
	return S_OK;
}

CBoss_Boom * CBoss_Boom::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
{
	CBoss_Boom*		pInstance = new CBoss_Boom(pDevice, pDevice_Context);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX("Failed to Creating Instance (CBoss_Boom) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CBoss_Boom::Clone_GameObject(void * pArg)
{
	CBoss_Boom*		pInstance = new CBoss_Boom(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Cloned Instance (CStrikerTower_Bullet) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CBoss_Boom::Free()
{
	__super::Free();
}
