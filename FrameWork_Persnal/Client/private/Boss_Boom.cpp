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

	Set_Pivot(XMVectorSet(0.1f, 0.1f, 0.1f, 0.f));
	Set_Pivot_Rotate_Radian(XMVectorSet(90.f, 0.f, 90.f, 0.f));



	return S_OK;
}

_int CBoss_Boom::Tick(_float TimeDelta)
{
	_int iReturn = 0;
	if (iReturn = __super::Tick(TimeDelta))
		return iReturn;

	m_fAlphaTime -= TimeDelta * 10.f;



	//_vector vDir = XMLoadFloat3(&m_vGoDir);
	//m_pMovementCom->Go_Dir_Vector(TimeDelta, vDir);
	m_pColliderCom_Attack->Set_Scale_Tick_Linear(_float3(2.5f, 2.5f, 2.5f), TimeDelta * 5.f);

	/*
	m_ColliderDesc.vScale.x += (vScale.x - m_ColliderDesc.vScale.x) * TimeDelta;
	m_ColliderDesc.vScale.y += (vScale.y - m_ColliderDesc.vScale.y) * TimeDelta;
	m_ColliderDesc.vScale.z += (vScale.z - m_ColliderDesc.vScale.z) * TimeDelta;

	*/

	_vector vMyScale = XMVectorZero();
	vMyScale.m128_f32[0] = m_pMovementCom->Get_Scale(EState::Right);
	vMyScale.m128_f32[1] = m_pMovementCom->Get_Scale(EState::Up);
	vMyScale.m128_f32[2] = m_pMovementCom->Get_Scale(EState::Look);

	vMyScale.m128_f32[0] += (8.f - vMyScale.m128_f32[0]) * TimeDelta * 5.f;
	vMyScale.m128_f32[1] += (8.f - vMyScale.m128_f32[1]) * TimeDelta * 5.f;
	vMyScale.m128_f32[2] += (8.f - vMyScale.m128_f32[2]) * TimeDelta * 5.f;

	m_pMovementCom->Set_Scale(vMyScale);

	if (nullptr != m_pColliderCom_Attack)
	{
		//if (true == m_pColliderCom_Attack->Get_IsCollide())
		//	return OBJECT_DEAD;

		m_pColliderCom_Attack->Update_Collider(m_pMovementCom->Get_WorldMatrix());
	}

	return iReturn;
}

_int CBoss_Boom::Late_Tick(_float TimeDelta)
{
	//if (true == m_pColliderCom_Attack->Get_IsCollide())
	//	return OBJECT_DEAD;

	m_fLifeTime -= TimeDelta;
	if (0.f >= m_fLifeTime)
		m_IsDelete_This = true;

	if (true == m_IsDelete_This)
		return OBJECT_DEAD;

	return m_pRendererCom->Add_GameObjectToRenderer(ERenderGroup::Alpha, this);
}

HRESULT CBoss_Boom::Render()
{
	m_pModelCom->Bind_VIBuffer();

	m_pModelCom->Set_Variable("g_PivotMatrix", &XMMatrixTranspose(XMLoadFloat4x4(&m_PivotMatrix)), sizeof(_matrix));
	m_pModelCom->Set_Variable("WorldMatrix", &XMMatrixTranspose(m_pMovementCom->Get_WorldMatrix()), sizeof(_matrix));
	m_pModelCom->Set_Variable("ViewMatrix", &XMMatrixTranspose(GET_VIEW_SPACE), sizeof(_matrix));
	m_pModelCom->Set_Variable("ProjMatrix", &XMMatrixTranspose(GET_PROJ_SPACE), sizeof(_matrix));

	_float4 vColor = { 5.f, 0.3f, 0.05f, m_fAlphaTime };
	m_pModelCom->Set_Variable("g_vColor", &vColor, sizeof(_float4));

	_uint iNumMaterials = m_pModelCom->Get_NumMaterials();

	for (_uint i = 0; i < iNumMaterials; ++i)
	{
		if (FAILED(m_pModelCom->Set_ShaderResourceView("g_DiffuseTexture", i, aiTextureType::aiTextureType_DIFFUSE)))
			return E_FAIL;
		//if (FAILED(m_pModelCom->Set_ShaderResourceView("g_DiffuseTexture", i, aiTextureType::aiTextureType_NORMALS)))
		//	return E_FAIL;
		//if (FAILED(m_pModelCom->Set_ShaderResourceView("g_DiffuseTexture", i, aiTextureType::aiTextureType_SPECULAR)))
		//	return E_FAIL;

		m_pModelCom->Render_Model(i, 8);
	}


	return S_OK;


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
