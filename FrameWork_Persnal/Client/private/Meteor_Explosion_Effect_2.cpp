#include "stdafx.h"
#include "..\public\Meteor_Explosion_Effect_2.h"

CMeteor_Explosion_Effect_2::CMeteor_Explosion_Effect_2(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: CBullet(pDevice, pDevice_Context)
{
}

CMeteor_Explosion_Effect_2::CMeteor_Explosion_Effect_2(const CMeteor_Explosion_Effect_2 & rhs)
	: CBullet(rhs)
{
}

HRESULT CMeteor_Explosion_Effect_2::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CMeteor_Explosion_Effect_2::NativeConstruct(void* pArg)
{
	__super::NativeConstruct(pArg);

	Set_Pivot(XMVectorSet(0.1f, 0.1f, 0.1f, 0.f));
	Set_Pivot_Rotate_Radian(XMVectorSet(90.f, 0.f, 0.f, 0.f));



	return S_OK;
}

_int CMeteor_Explosion_Effect_2::Tick(_float TimeDelta)
{
	m_fAttTime += TimeDelta;

	_vector vScale = XMVectorZero();

	vScale = XMVectorSetX(vScale, m_pMovementCom->Get_Scale(EState::Right));
	vScale = XMVectorSetZ(vScale, m_pMovementCom->Get_Scale(EState::Look));

	//if (0.25f >= m_fAttTime)
	vScale = XMVectorSetY(vScale, m_pMovementCom->Get_Scale(EState::Up) + TimeDelta * 10.f * (1.f + m_fAttTime));

	//else
	//	vScale = XMVectorSetY(vScale, m_pMovementCom->Get_Scale(EState::Up) - TimeDelta * 12.f * (1.f + m_fAttTime));

	m_pMovementCom->Set_Scale(vScale);

	return _int();
}

_int CMeteor_Explosion_Effect_2::Late_Tick(_float TimeDelta)
{
	if (0.5f <= m_fAttTime)
		return OBJECT_DEAD;


	return m_pRendererCom->Add_GameObjectToRenderer(ERenderGroup::Alpha, this);
}

HRESULT CMeteor_Explosion_Effect_2::Render()
{
	__super::Render();

	return S_OK;
}

HRESULT CMeteor_Explosion_Effect_2::Ready_Component(void * pArg)
{
	return S_OK;
}

CMeteor_Explosion_Effect_2 * CMeteor_Explosion_Effect_2::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
{
	CMeteor_Explosion_Effect_2* pInstance = new CMeteor_Explosion_Effect_2(pDevice, pDevice_Context);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX("Failed to Creating Instance (CMeteor_Explosion_Effect_2) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CMeteor_Explosion_Effect_2::Clone_GameObject(void * pArg)
{
	CMeteor_Explosion_Effect_2* pInstance = new CMeteor_Explosion_Effect_2(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Creating clone (CMeteor_Explosion_Effect_2) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMeteor_Explosion_Effect_2::Free()
{
	__super::Free();
}
