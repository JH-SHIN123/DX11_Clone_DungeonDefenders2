#include "stdafx.h"
#include "..\public\CrystalCore_Ring.h"

CCrystalCore_Ring::CCrystalCore_Ring(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: CGameObject(pDevice, pDevice_Context)
{
}

CCrystalCore_Ring::CCrystalCore_Ring(const CCrystalCore_Ring & rhs)
	: CGameObject(rhs)
{
}

HRESULT CCrystalCore_Ring::NativeConstruct_Prototype()
{
	return E_NOTIMPL;
}

HRESULT CCrystalCore_Ring::NativeConstruct(void * pArg)
{
	Ready_Component(pArg);

	Set_Pivot(XMVectorSet(0.05f, 0.05f, 0.05f, 0.f));
	Set_Pivot_Rotate_Radian(XMVectorSet(90.f, 0.f, 0.f, 0.f));

	return S_OK;
}

_int CCrystalCore_Ring::Tick(_float TimeDelta)
{
	m_pMovementCom->RotateToAxis_Tick(-TimeDelta, XMVectorSet(0.f, 1.f, 0.f, 0.f));


	return _int();
}

_int CCrystalCore_Ring::Late_Tick(_float TimeDelta)
{
	return _int();
}

HRESULT CCrystalCore_Ring::Render()
{
	if (nullptr == m_pModelCom)
		return E_FAIL;

	m_pModelCom->Bind_VIBuffer();

	m_pModelCom->Set_Variable("g_PivotMatrix", &XMMatrixTranspose(XMLoadFloat4x4(&m_PivotMatrix)), sizeof(_matrix));
	m_pModelCom->Set_Variable("WorldMatrix", &XMMatrixTranspose(m_pMovementCom->Get_WorldMatrix()), sizeof(_matrix));
	m_pModelCom->Set_Variable("ViewMatrix", &XMMatrixTranspose(GET_VIEW_SPACE), sizeof(_matrix));
	m_pModelCom->Set_Variable("ProjMatrix", &XMMatrixTranspose(GET_PROJ_SPACE), sizeof(_matrix));

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

HRESULT CCrystalCore_Ring::Ready_Component(void * pArg)
{
	GAMEOBJ_DESC Data;
	memcpy(&Data, pArg, sizeof(GAMEOBJ_DESC));

	HRESULT hr = S_OK;

	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Movement"), TEXT("Com_Movement"), (CComponent**)&m_pMovementCom, &Data.Movement_Desc);
	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Status"), TEXT("Com_Status"), (CComponent**)&m_pStatusCom, &Data.Status_Desc);

	hr = CGameObject::Add_Component((_uint)ELevel::Stage1, Data.szModelName, TEXT("Com_Model"), (CComponent**)&m_pModelCom);

	// ¸ðµ¨ ÄÄÆ÷³ÍÆ® 
	return S_OK;
}

CCrystalCore_Ring * CCrystalCore_Ring::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context, void* pArg)
{
	CCrystalCore_Ring*		pInstance = new CCrystalCore_Ring(pDevice, pDevice_Context);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Creating Instance (CCrystalCore_Ring) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CCrystalCore_Ring::Clone_GameObject(void * pArg)
{
	return nullptr;
}

void CCrystalCore_Ring::Free()
{
	__super::Free();

	Safe_Release(m_pModelCom);
	Safe_Release(m_pStatusCom);
	Safe_Release(m_pMovementCom);
}
