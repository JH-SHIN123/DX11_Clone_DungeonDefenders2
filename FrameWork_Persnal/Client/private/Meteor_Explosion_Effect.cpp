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
	if (nullptr == m_pModelCom)
		return S_OK;

	m_pModelCom->Bind_VIBuffer();
	//g_vColor


	m_pModelCom->Set_Variable("g_PivotMatrix", &XMMatrixTranspose(XMLoadFloat4x4(&m_PivotMatrix)), sizeof(_matrix));
	m_pModelCom->Set_Variable("WorldMatrix", &XMMatrixTranspose(m_pMovementCom->Get_WorldMatrix()), sizeof(_matrix));
	m_pModelCom->Set_Variable("ViewMatrix", &XMMatrixTranspose(GET_VIEW_SPACE), sizeof(_matrix));
	m_pModelCom->Set_Variable("ProjMatrix", &XMMatrixTranspose(GET_PROJ_SPACE), sizeof(_matrix));

	LIGHT_DESC*		LightDesc = GET_GAMEINSTANCE->Get_LightDesc(0);

	m_pModelCom->Set_Variable("vLightPosition", &LightDesc->vPosition, sizeof(_float3));
	m_pModelCom->Set_Variable("fRange", &LightDesc->fRadius, sizeof(_float));

	m_pModelCom->Set_Variable("vLightDiffuse", &LightDesc->vDiffuse, sizeof(_float4));
	m_pModelCom->Set_Variable("vLightAmbient", &LightDesc->vAmbient, sizeof(_float4));
	m_pModelCom->Set_Variable("vLightSpecular", &LightDesc->vSpecular, sizeof(_float4));

	m_pModelCom->Set_Variable("vCameraPosition", &GET_GAMEINSTANCE->Get_CamPosition(), sizeof(_vector));

	_uint iNumMaterials = m_pModelCom->Get_NumMaterials();

	for (_uint i = 0; i < iNumMaterials; ++i)
	{
		if (FAILED(m_pModelCom->Set_ShaderResourceView("g_DiffuseTexture", i, aiTextureType::aiTextureType_DIFFUSE)))
			return E_FAIL;
		//if (FAILED(m_pModelCom->Set_ShaderResourceView("g_DiffuseTexture", i, aiTextureType::aiTextureType_NORMALS)))
		//	return E_FAIL;
		//if (FAILED(m_pModelCom->Set_ShaderResourceView("g_DiffuseTexture", i, aiTextureType::aiTextureType_SPECULAR)))
		//	return E_FAIL;

		m_pModelCom->Render_Model(i, 9);
	}


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
