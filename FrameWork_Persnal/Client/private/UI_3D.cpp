#include "stdafx.h"
#include "..\public\UI_3D.h"

CUI_3D::CUI_3D(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
{
}

CUI_3D::CUI_3D(const CUI_3D & rhs)
	: CGameObject(rhs)
{
}

HRESULT CUI_3D::NativeConstruct_Prototype()
{
	__super::NativeConstruct_Prototype();

	return S_OK;
}

HRESULT CUI_3D::NativeConstruct(void * pArg)
{
	__super::NativeConstruct(pArg);

	SetUp_Default_Component(pArg);

	return S_OK;
}

_int CUI_3D::Tick(_float TimeDelta)
{
	

	return _int();
}

_int CUI_3D::Late_Tick(_float TimeDelta)
{
	return _int();
}

HRESULT CUI_3D::Render()
{
	if (nullptr == m_pBufferRectCom)
		return E_FAIL;

	m_pBufferRectCom->Set_Variable("WorldMatrix",	&XMMatrixTranspose(m_pMovementCom->Get_WorldMatrix()), sizeof(_matrix));
	m_pBufferRectCom->Set_Variable("ViewMatrix",	&XMMatrixTranspose(GET_VIEW_SPACE), sizeof(_matrix));
	m_pBufferRectCom->Set_Variable("ProjMatrix",	&XMMatrixTranspose(GET_PROJ_SPACE), sizeof(_matrix));

	return S_OK;
}

HRESULT CUI_3D::SetUp_Default_Component(void * pArg)
{
	memcpy(&m_UI3D_Desc, pArg, sizeof(UI3D_DESC));

	HRESULT hr = S_OK;

	/* For.Renderer */
	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom);

	/* For.VIBuffer */
	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_VIBuffer_Rect"), TEXT("Com_VIBuffer_Rect"), (CComponent**)&m_pBufferRectCom);

	/* For.Transform */
	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Movement"), TEXT("Com_Movement"), (CComponent**)&m_pMovementCom, &m_UI3D_Desc.Movement_Desc);

	/* For.Texture */
	hr = CGameObject::Add_Component((_uint)m_UI3D_Desc.eLevel, m_UI3D_Desc.szTextureName, TEXT("Com_Texture"), (CComponent**)&m_pTextureCom);


	if (S_OK != hr)
		MSG_BOX("CUI_3D::SetUp_Default_Component Failed");

	return hr;
}

void CUI_3D::BillBoarding()
{
	// 카메라 right 방향 * 타겟 X크기
	//_vector vCam = XMVector4Normalize(GET_CAMERA_RIGHT);
	//
	//_vector vNewRight = vCam;
	//_vector vRightUp_Cross	= XMVector3Cross(vNewRight, XMVectorSet(0.f, 1.f, 0.f, 0.f));
	//_vector vNewLook		= XMVector4Normalize(vRightUp_Cross);
	//_vector vNewUp			= XMVector4Normalize(XMVector3Cross(vNewLook, vNewRight)) ;
	//
	//m_pMovementCom->Set_State(EState::Right, vNewRight * m_pMovementCom->Get_Scale(EState::Right));
	//m_pMovementCom->Set_State(EState::Look, vNewLook * m_pMovementCom->Get_Scale(EState::Look));
	//m_pMovementCom->Set_State(EState::Up, vNewUp * m_pMovementCom->Get_Scale(EState::Up));
	//
	//_float fAngle = acosf(XMVectorGetX(XMVector3Dot(XMVector3Normalize(vNewLook), XMVector3Normalize(XMVector3Cross(vNewUp, vNewRight)))));	
	//
	//m_pMovementCom->Set_RotateAxis(XMVectorSet(0.f, 1.f, 0.f, 0.f), fAngle);


	_vector vCamLook = XMVector3Normalize(GET_CAMERA_LOOK);
	_float fAngle = acosf(XMVectorGetX(XMVector3Dot(vCamLook, XMVector3Normalize(m_pMovementCom->Get_State(EState::Look)))));
	_matrix RotateMatrix = XMMatrixRotationAxis(XMVectorSet(0.f, 1.f, 0.f, 0.f), fAngle);

	// 리셋시켰다가 다시 돌리자 
	
	m_pMovementCom->Set_State(EState::Look, XMVector3TransformNormal(m_pMovementCom->Get_State(EState::Look), RotateMatrix));
	m_pMovementCom->Set_State(EState::Right, XMVector3Normalize(XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), m_pMovementCom->Get_State(EState::Look))) * m_pMovementCom->Get_Scale(EState::Right));
	m_pMovementCom->Set_State(EState::Up, XMVector3Normalize(XMVector3Cross(m_pMovementCom->Get_State(EState::Look), m_pMovementCom->Get_State(EState::Right))* m_pMovementCom->Get_Scale(EState::Right)));

}

void CUI_3D::Free()
{
	__super::Free();
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pMovementCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pBufferRectCom);
}
