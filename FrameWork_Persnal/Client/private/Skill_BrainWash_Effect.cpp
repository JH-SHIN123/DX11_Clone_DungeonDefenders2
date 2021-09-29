#include "stdafx.h"
#include "..\public\Skill_BrainWash_Effect.h"

CSkill_BrainWash_Effect::CSkill_BrainWash_Effect(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: CBullet(pDevice, pDevice_Context)
{
}

CSkill_BrainWash_Effect::CSkill_BrainWash_Effect(const CSkill_BrainWash_Effect & rhs)
	: CBullet(rhs)
{
}

HRESULT CSkill_BrainWash_Effect::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CSkill_BrainWash_Effect::NativeConstruct(void * pArg)
{
	__super::NativeConstruct(pArg);

	Ready_Component(pArg);

	Set_Pivot(XMVectorSet(0.01f, 0.01f, 0.01f, 0.f));

	return S_OK;
}

_int CSkill_BrainWash_Effect::Tick(_float TimeDelta)
{
	_int iReturn = 0;
	if (iReturn = __super::Tick(TimeDelta))
		return iReturn;

	m_pMovementCom->RotateTick(TimeDelta);

	return iReturn;
}

_int CSkill_BrainWash_Effect::Late_Tick(_float TimeDelta)
{
	if (0.f >= m_fLifeTime)
		return OBJECT_DEAD;

	return m_pRendererCom->Add_GameObjectToRenderer(ERenderGroup::Alpha, this);
}

HRESULT CSkill_BrainWash_Effect::Render()
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

	_uint iNumMaterials = m_pModelCom->Get_NumMaterials();

	for (_uint i = 0; i < iNumMaterials; ++i)
	{
		if (FAILED(m_pModelCom->Set_ShaderResourceView("g_DiffuseTexture", i, aiTextureType::aiTextureType_DIFFUSE)))
			return E_FAIL;
		m_pModelCom->Render_Model(i, 1);
	}

	return S_OK;
}

HRESULT CSkill_BrainWash_Effect::Ready_Component(void * pArg)
{
	HRESULT hr = S_OK;



	if (S_OK != hr)
		MSG_BOX("CSkill_BrainWash_Effect::Ready_Component Failed!");

	return hr;
}

CSkill_BrainWash_Effect * CSkill_BrainWash_Effect::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
{
	CSkill_BrainWash_Effect*		pInstance = new CSkill_BrainWash_Effect(pDevice, pDevice_Context);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX("Failed to Creating Instance (CSkill_BrainWash_Effect) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CSkill_BrainWash_Effect::Clone_GameObject(void * pArg)
{
	CSkill_BrainWash_Effect*		pInstance = new CSkill_BrainWash_Effect(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Cloned Instance (CSkill_BrainWash_Effect) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CSkill_BrainWash_Effect::Free()
{
	__super::Free();
}
