#include "stdafx.h"
#include "..\public\Ring_Explosion.h"

CRing_Explosion::CRing_Explosion(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: CGameObject(pDevice, pDevice_Context)
{
}

CRing_Explosion::CRing_Explosion(const CRing_Explosion & rhs)
	: CGameObject(rhs)
{
}

HRESULT CRing_Explosion::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CRing_Explosion::NativeConstruct(void * pArg)
{
	Ready_Component(pArg);


	Set_Pivot(XMVectorSet(0.1f, 0.1f, 0.1f, 0.f));

	return S_OK;
}

_int CRing_Explosion::Tick(_float TimeDelta)
{
	m_Desc.fLifeTime -= TimeDelta;

	_vector vScale_Cur = XMVectorZero();

	vScale_Cur.m128_f32[0] = m_pMovementCom->Get_Scale(EState::Right);
	vScale_Cur.m128_f32[1] = m_pMovementCom->Get_Scale(EState::Up);
	vScale_Cur.m128_f32[2] = m_pMovementCom->Get_Scale(EState::Look);

	_vector vScale = XMLoadFloat3(&m_Desc.vScale) - vScale_Cur;

	vScale_Cur += vScale * 0.25f * TimeDelta;

	m_pMovementCom->Set_Scale_LinearRotate(vScale_Cur);


	return _int();
}

_int CRing_Explosion::Late_Tick(_float TimeDelta)
{
	if (0.f >= m_Desc.fLifeTime)
	{
		return OBJECT_DEAD;
	}

	return m_pRendererCom->Add_GameObjectToRenderer(ERenderGroup::Alpha, this);
}

HRESULT CRing_Explosion::Render()
{

	if (nullptr == m_pModelCom)
		return S_OK;

	m_pModelCom->Bind_VIBuffer();

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

	_float4 vColor = { 3.f,0.5f, 1.f,m_Desc.fLifeTime };
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
}

HRESULT CRing_Explosion::Ready_Component(void * pArg)
{
	memcpy(&m_Desc, pArg, sizeof(RING_EXPLOSION_DESC));

	HRESULT hr = S_OK;

	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom);
	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Movement"), TEXT("Com_Movement"), (CComponent**)&m_pMovementCom, &m_Desc.Move_Desc);

	hr = CGameObject::Add_Component((_uint)ELevel::Stage1, m_Desc.szModelName, TEXT("Com_Model"), (CComponent**)&m_pModelCom);

	if (hr)
		MSG_BOX("CRing_Explosion::Ready_Component");

	return S_OK;
}

CRing_Explosion * CRing_Explosion::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
{
	CRing_Explosion*		pInstance = new CRing_Explosion(pDevice, pDevice_Context);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX("Failed to Create Instance (CRing_Explosion) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CRing_Explosion::Clone_GameObject(void * pArg)
{
	CRing_Explosion*		pInstance = new CRing_Explosion(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Cloned Instance (CRing_Explosion) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CRing_Explosion::Free()
{
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pMovementCom);
	Safe_Release(m_pModelCom);

	__super::Free();
}
