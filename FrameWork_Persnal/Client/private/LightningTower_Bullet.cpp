#include "stdafx.h"
#include "LightningTower_Bullet.h"
#include "LightningTower_Bullet_Effect.h"

CLightningTower_Bullet::CLightningTower_Bullet(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: CBullet(pDevice, pDevice_Context)
{
}

CLightningTower_Bullet::CLightningTower_Bullet(const CLightningTower_Bullet & rhs)
	: CBullet(rhs)
{
}

HRESULT CLightningTower_Bullet::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CLightningTower_Bullet::NativeConstruct(void * pArg)
{
	__super::NativeConstruct(pArg);

	_vector vScale = XMVectorSet(0.05f, 0.05f, 0.05f, 0.f);
	m_pMovementCom->Set_Scale_LinearRotate(vScale);

	Ready_Component(pArg);

	return S_OK;
}

_int CLightningTower_Bullet::Tick(_float TimeDelta)
{
	__super::Tick(TimeDelta);

	Spawn_Check(TimeDelta);

	if (nullptr != m_pEffectMesh)
	{
		m_pEffectMesh->Set_Rotate(m_pMovementCom->Get_WorldMatrix());
		m_pEffectMesh->Set_Position(m_pMovementCom->Get_State(EState::Position));
		m_pEffectMesh->Tick(TimeDelta);
	}

	return _int();
}

_int CLightningTower_Bullet::Late_Tick(_float TimeDelta)
{
	if (nullptr != m_pEffectMesh)
		m_pEffectMesh->Late_Tick(TimeDelta);


	return __super::Late_Tick(TimeDelta);
}

HRESULT CLightningTower_Bullet::Render()
{
	__super::Render();

	return S_OK;
}

HRESULT CLightningTower_Bullet::Ready_Component(void * pArg)
{


	return S_OK;
}

void CLightningTower_Bullet::Spawn_Check(_float TimeDelta)
{
	m_fScaleTime += TimeDelta;
	_float3 vScale;
	vScale.x = m_pMovementCom->Get_Scale(EState::Right) + TimeDelta * 0.05f;

	if (vScale.x >= m_vScale_SizeUp.x)
		m_IsMoveable = true;

	if (nullptr == m_pEffectMesh && 0.75f <= m_fScaleTime)
	{
		EFFECT_DESC Data;
		Data.eEffectType = EEffectType::Mesh;
		Data.eResourceLevel = ELevel::Stage1;
		Data.iShaderPass = 2;
		lstrcpy(Data.szResourceName, L"Component_Mesh_LightningTower_Bullet_Effect_1");
		XMStoreFloat4(&Data.Move_Desc.vPos, m_pMovementCom->Get_State(EState::Position));
		Data.Move_Desc.vScale = { 0.03f,  0.03f , 0.03f, 0.f };
		m_pEffectMesh = (CLightningTower_Bullet_Effect*)GET_GAMEINSTANCE->Add_Create_Clone((_uint)ELevel::Stage1, L"Prototype_LightningTower_Bullet_Effect", (_uint)ELevel::Stage1, &Data);


	}

	if (false == m_IsMoveable)
	{
		vScale.y = m_pMovementCom->Get_Scale(EState::Up) + TimeDelta * 0.05f;
		vScale.z = m_pMovementCom->Get_Scale(EState::Look) + TimeDelta * 0.05f;

		m_pMovementCom->Set_Scale_LinearRotate(XMLoadFloat3(&vScale));
	}

	else
	{
		m_pMovementCom->Go_Up(-TimeDelta);
	}


}

CLightningTower_Bullet * CLightningTower_Bullet::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
{
	CLightningTower_Bullet*		pInstance = new CLightningTower_Bullet(pDevice, pDevice_Context);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX("Failed to Creating Instance (CLightningTower_Bullet) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CLightningTower_Bullet::Clone_GameObject(void * pArg)
{
	CLightningTower_Bullet*		pInstance = new CLightningTower_Bullet(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Cloned Instance (CLightningTower_Bullet) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CLightningTower_Bullet::Free()
{
	Safe_Release(m_pEffectMesh);


	__super::Free();
}
