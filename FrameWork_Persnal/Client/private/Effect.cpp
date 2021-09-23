#include "stdafx.h"
#include "..\public\Effect.h"

CEffect::CEffect(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: CGameObject(pDevice, pDevice_Context)
{
}

CEffect::CEffect(const CEffect & rhs)
	: CGameObject(rhs)
{
}

HRESULT CEffect::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CEffect::NativeConstruct(void * pArg)
{
	Ready_Component(pArg);

	return S_OK;
}

_int CEffect::Tick(_float TimeDelta)
{
	return _int();
}

_int CEffect::Late_Tick(_float TimeDelta)
{
	return m_pRendererCom->Add_GameObjectToRenderer(ERenderGroup::Alpha, this);
}

HRESULT CEffect::Render()
{
	Render_Mesh();

	return S_OK;
}

void CEffect::Set_Rotate(_fmatrix Matrix)
{
	m_pMovementCom->Set_Rotate(Matrix);
}

void CEffect::Set_Position(_fvector vPosition)
{
	m_pMovementCom->Set_State(EState::Position, vPosition);
}

void CEffect::Set_Rotate_Axis(_fvector vRatate, _float fRadian)
{
	m_pMovementCom->Set_RotateAxis(vRatate, fRadian);
}

HRESULT CEffect::Ready_Component(void * pArg)
{
	HRESULT hr = S_OK;

	EFFECT_DESC Data;
	memcpy(&Data, pArg, sizeof(EFFECT_DESC));

	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom);
	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Movement"), TEXT("Com_Movement"), (CComponent**)&m_pMovementCom, &Data.Move_Desc);

	if (EEffectType::Texture == Data.eEffectType)
	{
		hr = CGameObject::Add_Component((_uint)Data.eResourceLevel, Data.szResourceName, TEXT("Com_Textures"), (CComponent**)&m_pTexturesCom);
		hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom);
	}

	else if(EEffectType::Mesh == Data.eEffectType)
		hr = CGameObject::Add_Component((_uint)Data.eResourceLevel, Data.szResourceName, TEXT("Com_Model"), (CComponent**)&m_pModelCom);

	
	m_iShaderPass = Data.iShaderPass;

	if (S_OK != hr)
		MSG_BOX("");

	return hr;
}

HRESULT CEffect::Render_Mesh()
{
	if (nullptr == m_pModelCom)
		return S_OK;

	m_pModelCom->Bind_VIBuffer();

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

		m_pModelCom->Render_Model(i, m_iShaderPass);
	}

	return S_OK;
}

HRESULT CEffect::Render_Texture()
{
	return E_NOTIMPL;
}

CGameObject * CEffect::Clone_GameObject(void * pArg)
{
	return nullptr;
}

void CEffect::Free()
{
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pMovementCom);
	Safe_Release(m_pTexturesCom);
	Safe_Release(m_pVIBufferCom);
	__super::Free();
}
