#include "stdafx.h"
#include "Meteor_Explosion_Effect.h"

CMeteor_Explosion_Effect::CMeteor_Explosion_Effect(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: CBullet(pDevice, pDevice_Context)
{
}

CMeteor_Explosion_Effect::CMeteor_Explosion_Effect(const CMeteor_Explosion_Effect & rhs)
	: CBullet(rhs)
{
}

HRESULT CMeteor_Explosion_Effect::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CMeteor_Explosion_Effect::NativeConstruct(void* pArg)
{
	__super::NativeConstruct(pArg);

	Set_Pivot(XMVectorSet(0.05f, 0.05f, 0.05f, 0.f));
	Set_Pivot_Rotate_Radian(XMVectorSet(90.f, 0.f, 0.f, 0.f));

	return S_OK;
}

_int CMeteor_Explosion_Effect::Tick(_float TimeDelta)
{
	m_fAttTime -= TimeDelta;

	_vector vScale = XMVectorZero();

	vScale = XMVectorSetX(vScale, m_pMovementCom->Get_Scale(EState::Right) + TimeDelta * 8.f);
	vScale = XMVectorSetY(vScale, m_pMovementCom->Get_Scale(EState::Up) + TimeDelta * 8.f);
	vScale = XMVectorSetZ(vScale, m_pMovementCom->Get_Scale(EState::Look) + TimeDelta * 8.f);

	m_pMovementCom->Set_Scale(vScale);


	vScale = XMVectorSetX(vScale, m_pMovementCom->Get_Scale(EState::Right) / 8.f);

	_float3 vScale_Convert;
	XMStoreFloat3(&vScale_Convert, vScale);
	m_pColliderCom_Attack->Set_Scale(vScale_Convert);




	m_pColliderCom_Attack->Update_Collider(m_pMovementCom->Get_WorldMatrix());
	return _int();
}

_int CMeteor_Explosion_Effect::Late_Tick(_float TimeDelta)
{
	if (0 >= m_fAttTime)
		return OBJECT_DEAD;


	return m_pRendererCom->Add_GameObjectToRenderer(ERenderGroup::NoneAlpha, this);
}

HRESULT CMeteor_Explosion_Effect::Render()
{
	__super::Render();

#ifdef _DEBUG
	m_pColliderCom_Attack->Render_Collider();
#endif // _DEBUG

	return S_OK;
}

HRESULT CMeteor_Explosion_Effect::Ready_Component(void * pArg)
{
	return S_OK;
}

CMeteor_Explosion_Effect * CMeteor_Explosion_Effect::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
{
	CMeteor_Explosion_Effect* pInstance = new CMeteor_Explosion_Effect(pDevice, pDevice_Context);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX("Failed to Creating Instance (CMeteor_Explosion_Effect) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CMeteor_Explosion_Effect::Clone_GameObject(void * pArg)
{
	CMeteor_Explosion_Effect* pInstance = new CMeteor_Explosion_Effect(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Creating clone (CMeteor_Explosion_Effect) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMeteor_Explosion_Effect::Free()
{
	__super::Free();
}
