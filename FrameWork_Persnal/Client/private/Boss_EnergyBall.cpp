#include "stdafx.h"
#include "..\public\Boss_EnergyBall.h"

CBoss_EnergyBall::CBoss_EnergyBall(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: CBullet(pDevice, pDevice_Context)
{
}

CBoss_EnergyBall::CBoss_EnergyBall(const CBoss_EnergyBall & rhs)
	: CBullet(rhs)
{
}

HRESULT CBoss_EnergyBall::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CBoss_EnergyBall::NativeConstruct(void * pArg)
{
	__super::NativeConstruct(pArg);

	Ready_Component(pArg);

	Set_Pivot(XMVectorSet(0.01f, 0.01f, 0.01f, 0.f));

	return S_OK;
}

_int CBoss_EnergyBall::Tick(_float TimeDelta)
{
	_int iReturn = 0;
	if (iReturn = __super::Tick(TimeDelta))
		return iReturn;


	_vector vDir = XMLoadFloat3(&m_vGoDir);
	m_pMovementCom->Go_Dir_Vector(TimeDelta, vDir);


	if (nullptr != m_pColliderCom_Attack)
	{
		if (true == m_pColliderCom_Attack->Get_IsCollide())
			return OBJECT_DEAD;

		m_pColliderCom_Attack->Update_Collider(m_pMovementCom->Get_WorldMatrix());
	}

	return iReturn;
}

_int CBoss_EnergyBall::Late_Tick(_float TimeDelta)
{
	if (true == m_pColliderCom_Attack->Get_IsCollide())
		return OBJECT_DEAD;

	return __super::Late_Tick(TimeDelta);
}

HRESULT CBoss_EnergyBall::Render()
{
	__super::Render();


#ifdef _DEBUG
	m_pColliderCom_Attack->Render_Collider();
#endif // _DEBUG


	return S_OK;
}

HRESULT CBoss_EnergyBall::Ready_Component(void * pArg)
{
	return S_OK;
}

CBoss_EnergyBall * CBoss_EnergyBall::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
{
	CBoss_EnergyBall*		pInstance = new CBoss_EnergyBall(pDevice, pDevice_Context);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX("Failed to Creating Instance (CBoss_EnergyBall) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CBoss_EnergyBall::Clone_GameObject(void * pArg)
{
	CBoss_EnergyBall*		pInstance = new CBoss_EnergyBall(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Cloned Instance (CStrikerTower_Bullet) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CBoss_EnergyBall::Free()
{
	__super::Free();
}
