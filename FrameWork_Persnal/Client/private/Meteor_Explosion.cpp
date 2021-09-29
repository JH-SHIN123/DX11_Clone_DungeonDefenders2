#include "stdafx.h"
#include "..\public\Meteor_Explosion.h"

CMeteor_Explosion::CMeteor_Explosion(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: CBullet(pDevice, pDevice_Context)
{
}

CMeteor_Explosion::CMeteor_Explosion(const CMeteor_Explosion & rhs)
	: CBullet(rhs)
{
}

HRESULT CMeteor_Explosion::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CMeteor_Explosion::NativeConstruct(void * pArg)
{
	__super::NativeConstruct(pArg);

	Ready_Component(pArg);

	Set_Pivot(XMVectorSet(0.025f, 0.025f, 0.025f, 0.f));
	Set_Pivot_Rotate_Radian(XMVectorSet(90.f, 0.f, 0.f, 0.f));

	return S_OK;
}

_int CMeteor_Explosion::Tick(_float TimeDelta)
{
	if (__super::Tick(TimeDelta))
	{

		return OBJECT_DEAD;
	}

	// 0.25ÃÊ ¸¶´Ù ºÒ±âµÕ Ç¢Ç¢Ç¢Ç¢

	m_fAttTime -= TimeDelta;

	if (0 >= m_fAttTime)
	{
		Create_FirePillar(); // ºÒ±âµÕ ÀÌÆåÆ®¿¡ Ãæµ¹À» ÇÒ°ÅÀÓ
		--m_iFireCount;
		m_fAttTime = 0.25f;
	}

	return 0;
}

_int CMeteor_Explosion::Late_Tick(_float TimeDelta)
{
	if (0 >= m_iFireCount)
		return OBJECT_DEAD;

	__super::Late_Tick(TimeDelta);

	return 0;
}

HRESULT CMeteor_Explosion::Render()
{
	__super::Render();

	return S_OK;
}

HRESULT CMeteor_Explosion::Ready_Component(void * pArg)
{
	HRESULT hr = S_OK;



	if (S_OK != hr)
		MSG_BOX("CMeteor_Explosion::Ready_Component");

	return hr;
}

void CMeteor_Explosion::Create_FirePillar()
{
	BULLET_DESC Data;
	ZeroMemory(&Data, sizeof(BULLET_DESC));

	Data.fLifeTime = 0.5f;
	lstrcpy(Data.szModelName, L"Component_Mesh_Skill_Meteor_Explosion_Ring");
	XMStoreFloat4(&Data.MoveState_Desc.vPos, m_pMovementCom->Get_State(EState::Position));
	Data.MoveState_Desc.vScale = { 1.f, 1.f, 1.f, 0.f };

	//Data.Attack_Collide_Desc.Attack_Desc.eDamageType = EDamageType::Direct;
	//Data.Attack_Collide_Desc.Attack_Desc.iDamage = 1;
	//Data.Attack_Collide_Desc.Attack_Desc.fHitTime = 0.f;
	//Data.Attack_Collide_Desc.vScale = { 2.f, 2.f, 2.f };
	//Data.Attack_Collide_Desc.IsCenter = true;
	GET_GAMEINSTANCE->Add_GameObject((_uint)ELevel::Stage1, TEXT("Prototype_Skill_Meteor_Explosion_Ring"), (_uint)ELevel::Stage1, L"Layer_Bullet", &Data);

	_vector vDir;
	_vector vPos = m_pMovementCom->Get_State(EState::Position);
	ZeroMemory(&Data, sizeof(BULLET_DESC));
	Data.fLifeTime = 0.5f;
	XMStoreFloat4(&Data.MoveState_Desc.vPos, vPos);
	Data.MoveState_Desc.fSpeedPerSec = 30.f;
	Data.MoveState_Desc.vScale = { 1.f, 1.f, 1.f, 0.f };

	Data.Attack_Collide_Desc.Attack_Desc.eDamageType = EDamageType::Direct;
	Data.Attack_Collide_Desc.Attack_Desc.iDamage = 10;
	Data.Attack_Collide_Desc.Attack_Desc.fHitTime = 0.f;
	Data.Attack_Collide_Desc.vScale = { 2.f, 2.f, 2.f };
	Data.Attack_Collide_Desc.IsCenter = false;

	_float fAngle = 0;
	_matrix RotateMatrix;

	for (_int i = 0; i < 10; ++i)
	{
		RotateMatrix = XMMatrixRotationAxis(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(fAngle));
		vDir = XMVector3Normalize(RotateMatrix.r[2]);
		XMStoreFloat3(&Data.vDir, vDir);
		GET_GAMEINSTANCE->Add_GameObject((_uint)ELevel::Stage1, TEXT("Prototype_Boom"), (_uint)ELevel::Stage1, TEXT("Layer_Bullet"), &Data);

		fAngle += 360.f / 10.f;
	}



	ZeroMemory(&Data, sizeof(BULLET_DESC));

	Data.fLifeTime = 0.5f;
	lstrcpy(Data.szModelName, L"Component_Mesh_Skill_Meteor_Explosion_Effect");
	XMStoreFloat4(&Data.MoveState_Desc.vPos, m_pMovementCom->Get_State(EState::Position));
	Data.MoveState_Desc.vScale = { 1.f, 1.f, 1.f, 0.f };

	GET_GAMEINSTANCE->Add_GameObject((_uint)ELevel::Stage1, TEXT("Prototype_Skill_Meteor_Explosion_Effect"), (_uint)ELevel::Stage1, L"Layer_Effect", &Data);

}

CMeteor_Explosion * CMeteor_Explosion::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
{
	CMeteor_Explosion* pInstance = new CMeteor_Explosion(pDevice, pDevice_Context);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX("Failed to Creating Instance (CMeteor_Explosion) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CMeteor_Explosion::Clone_GameObject(void * pArg)
{
	CMeteor_Explosion* pInstance = new CMeteor_Explosion(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Creating clone (CMeteor_Explosion) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMeteor_Explosion::Free()
{
	__super::Free();
}
