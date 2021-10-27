#include "stdafx.h"
#include "..\public\Mesh_Effect.h"


CMesh_Effect::CMesh_Effect(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: CGameObject(pDevice, pDevice_Context)
{
}

CMesh_Effect::CMesh_Effect(const CMesh_Effect & rhs)
	: CGameObject(rhs)
{
}

HRESULT CMesh_Effect::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CMesh_Effect::NativeConstruct(void * pArg)
{
	Ready_Component(pArg);

	_vector vDir = XMLoadFloat3(&m_vGoDir);

	if (0.f < XMVectorGetX(XMVector3Length(vDir)))
	{
		_vector vPos = m_pMovementCom->Get_State(EState::Position) += vDir;

	}
	Set_Pivot(XMVectorSet(0.1f, 0.1f, 0.1f, 0.f));
	//Set_Pivot_Rotate_Radian(XMVectorSet(180.f, 0.f, 90.f, 0.f));

	return S_OK;
}

_int CMesh_Effect::Tick(_float TimeDelta)
{
	if (true == m_IsDelete_This)
		return OBJECT_DEAD;

	return _int();
}

_int CMesh_Effect::Late_Tick(_float TimeDelta)
{
	Set_Pivot(XMVectorSet(0.01f, 0.01f, 0.01f, 0.f));

	m_fLifeTime -= TimeDelta;
	if (0.f >= m_fLifeTime)
		m_IsDelete_This = true;

	if (true == m_IsDelete_This)
		return OBJECT_DEAD;

	return m_pRendererCom->Add_GameObjectToRenderer(ERenderGroup::Alpha, this);
}

HRESULT CMesh_Effect::Render()
{
	if (nullptr == m_pModelCom)
		return S_OK;

	m_pModelCom->Bind_VIBuffer();

	m_pModelCom->Set_Variable("g_PivotMatrix", &XMMatrixTranspose(XMLoadFloat4x4(&m_PivotMatrix)), sizeof(_matrix));
	m_pModelCom->Set_Variable("WorldMatrix", &XMMatrixTranspose(m_pMovementCom->Get_WorldMatrix()), sizeof(_matrix));
	m_pModelCom->Set_Variable("ViewMatrix", &XMMatrixTranspose(GET_VIEW_SPACE), sizeof(_matrix));
	m_pModelCom->Set_Variable("ProjMatrix", &XMMatrixTranspose(GET_PROJ_SPACE), sizeof(_matrix));

	_uint iNumMaterials = m_pModelCom->Get_NumMaterials();

	_float4 vColor = { 2.f, 1.5f, 0.8f, m_fLifeTime };
	m_pModelCom->Set_Variable("g_vColor", &vColor, sizeof(_float4));

	for (_uint i = 0; i < iNumMaterials; ++i)
	{
		if (FAILED(m_pModelCom->Set_ShaderResourceView("g_DiffuseTexture", i, aiTextureType::aiTextureType_DIFFUSE)))
			return E_FAIL;
		//if (FAILED(m_pModelCom->Set_ShaderResourceView("g_DiffuseTexture", i, aiTextureType::aiTextureType_NORMALS)))
		//	return E_FAIL;
		//if (FAILED(m_pModelCom->Set_ShaderResourceView("g_DiffuseTexture", i, aiTextureType::aiTextureType_SPECULAR)))
		//	return E_FAIL;

		m_pModelCom->Render_Model(i, 11);
	}

	return S_OK;
}

HRESULT CMesh_Effect::Ready_Component(void * pArg)
{
	MESH_EFFECT_DESC Data;
	memcpy(&Data, pArg, sizeof(MESH_EFFECT_DESC));

	HRESULT hr = S_OK;

	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom);
	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Movement"), TEXT("Com_Movement"), (CComponent**)&m_pMovementCom, &Data.MoveState_Desc);
	hr = CGameObject::Add_Component((_uint)ELevel::Stage1, Data.szModelName, TEXT("Com_Model"), (CComponent**)&m_pModelCom);

	if (2 < lstrlen(Data.szTextureName))
		hr = CGameObject::Add_Component((_uint)ELevel::Stage1, Data.szTextureName, TEXT("Com_Texture_0"), (CComponent**)&m_pTextureCom);

	m_vGoDir = Data.vDir;
	m_fLifeTime = Data.fLifeTime;
	m_vTextureSize = Data.vTextureSize;

	if (S_OK != hr)
		MSG_BOX("CMesh_Effect::Ready_Component Failed!");

	return hr;
}

CMesh_Effect * CMesh_Effect::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
{
	CMesh_Effect*		pInstance = new CMesh_Effect(pDevice, pDevice_Context);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX("Failed to Creating Instance (CMesh_Effect) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CMesh_Effect::Clone_GameObject(void * pArg)
{
	CMesh_Effect*		pInstance = new CMesh_Effect(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Cloned Instance (CMesh_Effect) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMesh_Effect::Free()
{
	__super::Free();

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pMovementCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pTextureCom);
}
