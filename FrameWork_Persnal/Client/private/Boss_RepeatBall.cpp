#include "stdafx.h"
#include "..\public\Boss_RepeatBall.h"

CBoss_RepeatBall::CBoss_RepeatBall(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: CBullet(pDevice, pDevice_Context)
{
}

CBoss_RepeatBall::CBoss_RepeatBall(const CBoss_RepeatBall & rhs)
	: CBullet(rhs)
{
}

HRESULT CBoss_RepeatBall::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CBoss_RepeatBall::NativeConstruct(void * pArg)
{
	__super::NativeConstruct(pArg);

	Ready_Component(pArg);

	Set_Pivot(XMVectorSet(0.01f, 0.01f, 0.01f, 0.f));

	return S_OK;
}

_int CBoss_RepeatBall::Tick(_float TimeDelta)
{
	_int iReturn = 0;
	if (iReturn = __super::Tick(TimeDelta))
		return iReturn;


	_vector vDir = (XMLoadFloat3(&m_vGoDir) - m_pMovementCom->Get_State(EState::Position));
	vDir = XMVectorSetW(vDir, 0.f);
	vDir *= 0.25f;
	m_pMovementCom->Go_Dir(TimeDelta, vDir);


	if (nullptr != m_pColliderCom_Attack)
	{
		if (true == m_pColliderCom_Attack->Get_IsCollide())
			return OBJECT_DEAD;

		m_pColliderCom_Attack->Update_Collider(m_pMovementCom->Get_WorldMatrix());
	}

	return iReturn;
}

_int CBoss_RepeatBall::Late_Tick(_float TimeDelta)
{
	if (true == m_pColliderCom_Attack->Get_IsCollide())
		return OBJECT_DEAD;

	return __super::Late_Tick(TimeDelta);
}

HRESULT CBoss_RepeatBall::Render()
{
	__super::Render();


#ifdef _DEBUG
	m_pColliderCom_Attack->Render_Collider();
#endif // _DEBUG


	return S_OK;
}

HRESULT CBoss_RepeatBall::Ready_Component(void * pArg)
{
	return S_OK;
}

CBoss_RepeatBall * CBoss_RepeatBall::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
{
	CBoss_RepeatBall*		pInstance = new CBoss_RepeatBall(pDevice, pDevice_Context);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX("Failed to Creating Instance (CBoss_RepeatBall) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CBoss_RepeatBall::Clone_GameObject(void * pArg)
{
	CBoss_RepeatBall*		pInstance = new CBoss_RepeatBall(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Cloned Instance (CStrikerTower_Bullet) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CBoss_RepeatBall::Free()
{
	__super::Free();
}
