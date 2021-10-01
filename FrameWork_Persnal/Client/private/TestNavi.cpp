#include "stdafx.h"
#include "..\public\TestNavi.h"

CTestNavi::CTestNavi(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: CGameObject(pDevice, pDevice_Context)
{

}

CTestNavi::CTestNavi(const CTestNavi & rhs)
	: CGameObject(rhs)
{

}

HRESULT CTestNavi::NativeConstruct_Prototype()
{
	CGameObject::NativeConstruct_Prototype();

	/*

	_matrix			PivotMatrix = XMMatrixScaling(0.05f, 0.05f, 0.05f) *
	XMMatrixRotationX(XMConvertToRadians(90.0f)) *
	XMMatrixRotationY(XMConvertToRadians(90.0f));

	*/

	return S_OK;
}

HRESULT CTestNavi::NativeConstruct(void * pArg)
{
	CGameObject::NativeConstruct(pArg);

	if (FAILED(Ready_Component()))
		return E_FAIL;

	return S_OK;
}

_int CTestNavi::Tick(_float TimeDelta)
{
	return _int();
}

_int CTestNavi::Late_Tick(_float TimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;

	return m_pRendererCom->Add_GameObjectToRenderer(ERenderGroup::NoneAlpha, this);
}

HRESULT CTestNavi::Render()
{
	if (nullptr == m_pModelCom)
		return E_FAIL;

	/* 현재 파이프라인에 정점, 인덱스 버퍼를 셋한다. */
	m_pModelCom->Bind_VIBuffer();

	m_pModelCom->Set_Variable("g_PivotMatrix", &XMMatrixTranspose(XMLoadFloat4x4(&m_PivotMatrix)), sizeof(_matrix));
	m_pModelCom->Set_Variable("WorldMatrix", &XMMatrixTranspose(m_pTransformCom->Get_WorldMatrix()), sizeof(_matrix));
	m_pModelCom->Set_Variable("ViewMatrix", &XMMatrixTranspose(GET_VIEW_SPACE), sizeof(_matrix));
	m_pModelCom->Set_Variable("ProjMatrix", &XMMatrixTranspose(GET_PROJ_SPACE), sizeof(_matrix));


	LIGHT_DESC*		LightDesc = GET_GAMEINSTANCE->Get_LightDesc(0);
	// m_pVIBufferCom->Set_Variable("vLightDirection", &LightDesc.vDirection, sizeof(_float3));
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

HRESULT CTestNavi::Ready_Component()
{
	/* 렌더러컴포넌트를 추가하자.*/
	/* For.Renderer*/
	if (FAILED(CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Transform */
	if (FAILED(CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	/* For.VIBuffer */
	if (FAILED(CGameObject::Add_Component((_uint)ELevel::Stage1, TEXT("Component_VIBuffer_Terrain"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	/* For.Textures */
	if (FAILED(CGameObject::Add_Component((_uint)ELevel::Stage1, TEXT("Component_Texture_Terrain"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component((_uint)ELevel::Stage1, TEXT("Component_Mesh_Level_1_TestNavi"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;


	_matrix			PivotMatrix = XMMatrixScaling(0.05f, 0.05f, 0.05f) *
		XMMatrixRotationX(XMConvertToRadians(90.0f)) *
		XMMatrixRotationY(XMConvertToRadians(90.0f));

	m_pTransformCom->Set_WorldMatrix(PivotMatrix);

	return S_OK;
}


CTestNavi * CTestNavi::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
{
	CTestNavi*		pInstance = new CTestNavi(pDevice, pDevice_Context);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX("Failed to Creating Instance (CTestNavi) ");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CTestNavi::Clone_GameObject(void * pArg)
{
	CTestNavi*		pInstance = new CTestNavi(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Cloned Instance (CTestNavi) ");
		Safe_Release(pInstance);
	}

	return pInstance;
}
void CTestNavi::Free()
{
	CGameObject::Free();

	Safe_Release(m_pModelCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTransformCom);
}
