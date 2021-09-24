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

	//_vector vScale = XMVectorSet(0.05f, 0.05f, 0.05f, 0.f);
	//m_pMovementCom->Set_Scale_LinearRotate(vScale);

	m_vecEffectMesh.resize(m_iBoundCount);

	Set_Pivot(XMVectorSet(0.05f, 0.05f, 0.05f, 0.f));


	Ready_Component(pArg);

	return S_OK;
}

_int CLightningTower_Bullet::Tick(_float TimeDelta)
{
	__super::Tick(TimeDelta);

	Spawn_Check(TimeDelta);

	for (auto& iter : m_vecEffectMesh)
	{
		if (nullptr != iter)
		{
			iter->Late_Tick(TimeDelta);
			iter->Set_Rotate(m_pMovementCom->Get_WorldMatrix());
			iter->Set_Position(m_pMovementCom->Get_State(EState::Position));
			iter->Tick(TimeDelta);
		}
	}
	

	return _int();
}

_int CLightningTower_Bullet::Late_Tick(_float TimeDelta)
{
	for (auto& iter : m_vecEffectMesh)
	{
		if(nullptr != iter)
			iter->Late_Tick(TimeDelta);
	}
	


	return __super::Late_Tick(TimeDelta);
}

HRESULT CLightningTower_Bullet::Render()
{
	__super::Render();

	return S_OK;
}

HRESULT CLightningTower_Bullet::Ready_Component(void * pArg)
{
	HRESULT hr = S_OK;

	COLLIDER_DESC Data;
	Data.vScale = { 2.f, 2.f, 2.f };

	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Collider_Sphere"), TEXT("Com_Collider"), (CComponent**)&m_pColliderCom, &Data);


	if (S_OK != hr)
		MSG_BOX("CLightningTower_Bullet::Ready_Component Filed!");

	return S_OK;
}

void CLightningTower_Bullet::Spawn_Check(_float TimeDelta)
{
	m_fScaleTime += TimeDelta;
	_float3 vScale;
	vScale.x = TimeDelta * 0.05f;

	if (vScale.x >= m_vScale_SizeUp.x)
		m_IsMoveable = true;

	if (nullptr == m_vecEffectMesh[0] && 0.25f <= m_fScaleTime)
	{
		Create_Effect();
	}

	if (false == m_IsMoveable)
	{
		vScale.y = TimeDelta * 0.05f;
		vScale.z = TimeDelta * 0.05f;

		Set_Pivot(XMVectorSet(vScale.x, vScale.y, vScale.z, 0.f));
	}

	else
	{
		m_pMovementCom->Go_Up(-TimeDelta);
	}


}

void CLightningTower_Bullet::Create_Effect()
{
	EFFECT_DESC Data;
	Data.eEffectType = EEffectType::Mesh;
	Data.eResourceLevel = ELevel::Stage1;
	Data.iShaderPass = 2;
	XMStoreFloat4(&Data.Move_Desc.vPos, m_pMovementCom->Get_State(EState::Position));
	Data.Move_Desc.vScale = { 0.03f,  0.03f , 0.03f, 0.f };
	_vector vAxis = XMVectorSet(1.f, 0.f, 0.f, 0.f);
	
	lstrcpy(Data.szResourceName, L"Component_Mesh_LightningTower_Bullet_Effect_1");
	m_vecEffectMesh[0] = (CLightningTower_Bullet_Effect*)GET_GAMEINSTANCE->Add_Create_Clone((_uint)ELevel::Stage1, L"Prototype_LightningTower_Bullet_Effect", (_uint)ELevel::Stage1, &Data);
	
	lstrcpy(Data.szResourceName, L"Component_Mesh_LightningTower_Bullet_Effect_2");
	m_vecEffectMesh[1] = (CLightningTower_Bullet_Effect*)GET_GAMEINSTANCE->Add_Create_Clone((_uint)ELevel::Stage1, L"Prototype_LightningTower_Bullet_Effect", (_uint)ELevel::Stage1, &Data);
	m_vecEffectMesh[1]->Set_Rotate_Axis(vAxis, XMConvertToDegrees(70.f));

	vAxis = XMVectorSet(1.f, 0.f, 0.f, 0.f);
	lstrcpy(Data.szResourceName, L"Component_Mesh_LightningTower_Bullet_Effect_3");
	m_vecEffectMesh[2] = (CLightningTower_Bullet_Effect*)GET_GAMEINSTANCE->Add_Create_Clone((_uint)ELevel::Stage1, L"Prototype_LightningTower_Bullet_Effect", (_uint)ELevel::Stage1, &Data);
	m_vecEffectMesh[2]->Set_Rotate_Axis(vAxis, XMConvertToDegrees(90.f));

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
	Safe_Release_Vector(m_vecEffectMesh);


	__super::Free();
}
