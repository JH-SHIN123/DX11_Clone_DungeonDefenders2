#include "stdafx.h"
#include "..\public\Terrain.h"
#include "Loading.h"
#include "GameInstance.h"

CTerrain::CTerrain(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: CGameObject(pDevice, pDevice_Context)
{

}

CTerrain::CTerrain(const CTerrain & rhs)
	: CGameObject(rhs)
{

}

HRESULT CTerrain::NativeConstruct_Prototype()
{
	CGameObject::NativeConstruct_Prototype();

	
	
	//_matrix			PivotMatrix = XMMatrixScaling(0.05f, 0.05f, 0.05f) *
	//XMMatrixRotationX(XMConvertToRadians(90.0f)) *
	//XMMatrixRotationY(XMConvertToRadians(90.0f));


	return S_OK;
}

HRESULT CTerrain::NativeConstruct(void * pArg)
{
	CGameObject::NativeConstruct(pArg);

	if (FAILED(Ready_Component()))
		return E_FAIL;

	Set_Pivot_Rotate_Radian(XMVectorSet(0.f, 0.f, 90.f, 0.f));

	return S_OK;
}

_int CTerrain::Tick(_float TimeDelta)
{
	//if(true == m_IsFirst)
	//	Set_Pivot_Rotate_Radian(XMVectorSet(90.f, 270.f, 90.f, 0.f));
	//m_IsFirst = false;

	//if (GET_KEY_INPUT(DIK_NUMPAD0))
	//{
	//	if (bNo[0] == false)
	//	{
	//		_int i = 0;
	//		vPivot.x += 90.f;
	//		Set_Pivot_Rotate_Radian_New(XMVectorSet(vPivot.x, vPivot.y, vPivot.z, 0.f));
	//		bNo[0] = true;
	//	}
	//}
	//else
	//	bNo[0] = false;
	//
	//if (GET_KEY_INPUT(DIK_NUMPAD1))
	//{
	//	if (bNo[1] == false)
	//	{
	//		vPivot.y += 90.f;
	//		Set_Pivot_Rotate_Radian_New(XMVectorSet(vPivot.x, vPivot.y, vPivot.z, 0.f));
	//		bNo[1] = true;
	//	}
	//}
	//else
	//	bNo[1] = false;
	//
	//if (GET_KEY_INPUT(DIK_NUMPAD2))
	//{
	//	if (bNo[2] == false)
	//	{
	//		vPivot.z += 90.f;
	//		Set_Pivot_Rotate_Radian_New(XMVectorSet(vPivot.x, vPivot.y, vPivot.z, 0.f));
	//		bNo[2] = true;
	//	}
	//}
	//else
	//	bNo[2] = false;
	//
	//if (GET_KEY_INPUT(DIK_NUMPAD3))
	//{
	//	vPivot.x = 0.f;
	//	vPivot.y = 0.f;
	//	vPivot.z = 0.f;
	//	Set_Pivot_Rotate_Radian_New(XMVectorSet(vPivot.x, vPivot.y, vPivot.z, 0.f));
	//}

	return _int();

}

_int CTerrain::Late_Tick(_float TimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;

	return m_pRendererCom->Add_GameObjectToRenderer(ERenderGroup::Priority_Second, this);
}

HRESULT CTerrain::Render()
{
	if (nullptr == m_pModelCom)
		return E_FAIL;

	/* 현재 파이프라인에 정점, 인덱스 버퍼를 셋한다. */
	m_pModelCom->Bind_VIBuffer();

	m_pModelCom->Set_Variable("g_PivotMatrix", &XMMatrixTranspose(XMLoadFloat4x4(&m_PivotMatrix)), sizeof(_matrix));
	m_pModelCom->Set_Variable("WorldMatrix", &XMMatrixTranspose(m_pTransformCom->Get_WorldMatrix()), sizeof(_matrix));
	m_pModelCom->Set_Variable("ViewMatrix", &XMMatrixTranspose(GET_VIEW_SPACE), sizeof(_matrix));
	m_pModelCom->Set_Variable("ProjMatrix", &XMMatrixTranspose(GET_PROJ_SPACE), sizeof(_matrix));

	_uint iNumMaterials = m_pModelCom->Get_NumMaterials();
	for (_uint i = 0; i < iNumMaterials; ++i)
	{
		if (FAILED(m_pModelCom->Set_ShaderResourceView("g_DiffuseTexture", i, aiTextureType::aiTextureType_DIFFUSE)))
			return E_FAIL;

		m_pModelCom->Render_Model(i, 1);
	}


	return S_OK;
}

HRESULT CTerrain::Ready_Component()
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

	if (FAILED(CGameObject::Add_Component((_uint)ELevel::Stage1, TEXT("Component_Mesh_Level_1"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;


	_matrix			PivotMatrix = XMMatrixScaling(0.05f, 0.05f, 0.05f) *
		XMMatrixRotationX(XMConvertToRadians(90.0f)) *
		XMMatrixRotationY(XMConvertToRadians(90.0f));

	m_pTransformCom->Set_WorldMatrix(PivotMatrix);

	return S_OK;
}


CTerrain * CTerrain::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
{
	CTerrain*		pInstance = new CTerrain(pDevice, pDevice_Context);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX("Failed to Creating Instance (CTerrain) ");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CTerrain::Clone_GameObject(void * pArg)
{
	CTerrain*		pInstance = new CTerrain(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Cloned Instance (CTerrain) ");
		Safe_Release(pInstance);
	}

	return pInstance;
}
void CTerrain::Free()
{
	CGameObject::Free();

	Safe_Release(m_pModelCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTransformCom);
}
