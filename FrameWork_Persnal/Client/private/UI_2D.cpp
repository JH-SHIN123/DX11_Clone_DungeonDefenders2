#include "stdafx.h"
#include "..\public\UI_2D.h"

CUI_2D::CUI_2D(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
{
}

CUI_2D::CUI_2D(const CUI_2D & rhs)
	: CGameObject(rhs)
{
}

HRESULT CUI_2D::NativeConstruct_Prototype()
{
	__super::NativeConstruct_Prototype();

	return S_OK;
}

HRESULT CUI_2D::NativeConstruct(void * pArg)
{
	__super::NativeConstruct(pArg);

	SetUp_Default_Component(pArg);

	return S_OK;
}

_int CUI_2D::Tick(_float TimeDelta)
{
	return _int();
}

_int CUI_2D::Late_Tick(_float TimeDelta)
{
	return _int();
}

HRESULT CUI_2D::Render()
{
	if (nullptr == m_pBufferRectCom)
		return UPDATE_ERROR;

	m_pBufferRectCom->Set_Variable("WorldMatrix", &XMMatrixTranspose(m_pMovementCom->Get_WorldMatrix()), sizeof(_matrix));
	m_pBufferRectCom->Set_Variable("ViewMatrix", &XMMatrixTranspose(GET_INDENTITY_MATRIX), sizeof(_matrix));
	m_pBufferRectCom->Set_Variable("ProjMatrix", &XMMatrixTranspose(GET_ORTHO_SPACE), sizeof(_matrix));

	return S_OK;
}

HRESULT CUI_2D::SetUp_Default_Component(void * pArg)
{
	HRESULT hr = S_OK;

	UI2D_DESC* pData = new UI2D_DESC;
	lstrcpy(pData->szTextureName, L"Component_Texture_Failed");

	if (nullptr != pArg)
		memcpy(pData, pArg, sizeof(UI2D_DESC));

	/* For.Renderer */
	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom);

	/* For.VIBuffer */
	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_VIBuffer_Rect"), TEXT("Com_VIBuffer_Rect"), (CComponent**)&m_pBufferRectCom);

	/* For.Transform */
	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Movement"), TEXT("Com_Movement"), (CComponent**)&m_pMovementCom, &pData->Movement_Desc);

	/* For.Texture */
	hr = CGameObject::Add_Component((_uint)pData->eLevel, pData->szTextureName, TEXT("Com_Texture"), (CComponent**)&m_pTextureCom);
	
	if (hr != S_OK)
		MSG_BOX("SetUp_Default_Component is Failed (UI_2D)");


	m_UI2D_Desc = *pData;
	Safe_Delete(pData);
	return hr;
}

void CUI_2D::Free()
{
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pBufferRectCom);
	Safe_Release(m_pMovementCom);
	Safe_Release(m_pRendererCom);

	__super::Free();
}
